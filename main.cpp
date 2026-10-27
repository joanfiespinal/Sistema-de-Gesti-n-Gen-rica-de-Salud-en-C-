
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <numeric>

using namespace std;

// Sistema academico de gestion de salud.
// Las cinco categorias de impacto son una clasificacion estadistica del proyecto,
// no niveles clinicos ni criterios para diagnosticar pacientes.

class Paciente {
public:
    int id{};
    string nombre;
    int edad{};
    int idDiagnostico{};
    int idTratamiento{};
    int idARS{};
    int idCentro{};
};

class CentroSalud {
public:
    int id{};
    string nombre;
    string direccion;
};

class Medico {
public:
    int id{};
    string nombre;
    string especialidad;
};

class Diagnostico {
public:
    int id{};
    string nombre;
    string descripcion;
};

class Tratamiento {
public:
    int id{};
    string nombre;
    string descripcion;
};

class ARS {
public:
    int id{};
    string nombre;
    double porcentajeCobertura{};
};

class Consulta {
public:
    int id{};
    int idPaciente{};
    int idCentro{};
    int idMedico{};
    double costoTotal{};
    double coberturaARS{};
    double pagoPaciente{};

    double diferenciaPendiente() const {
        return max(0.0, costoTotal - coberturaARS - pagoPaciente);
    }
};

// Contenedores STL
vector<Paciente> pacientes;
vector<CentroSalud> centrosSalud;
vector<Medico> medicos;
vector<Diagnostico> diagnosticos;
vector<Tratamiento> tratamientos;
vector<ARS> aseguradoras;
vector<Consulta> consultas;

map<int, Paciente> pacientesPorId;
map<int, CentroSalud> centrosPorId;
map<int, Medico> medicosPorId;
map<int, Diagnostico> diagnosticosPorId;
map<int, Tratamiento> tratamientosPorId;
map<int, ARS> arsPorId;
map<int, Consulta> consultasPorId;

string minusculas(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(),
              [](unsigned char c) {
                  return static_cast<char>(tolower(c));
              });
    return texto;
}

void limpiarEntrada() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int leerEntero(const string& mensaje, int minimo = 0) {
    int valor;

    while (true) {
        cout << mensaje;

        if (cin >> valor && valor >= minimo) {
            limpiarEntrada();
            return valor;
        }

        cout << "Entrada invalida. Introduce un entero mayor o igual que "
             << minimo << ".\n";

        cin.clear();
        limpiarEntrada();
    }
}

double leerDinero(const string& mensaje) {
    double valor;

    while (true) {
        cout << mensaje;

        if (cin >> valor && valor >= 0) {
            limpiarEntrada();
            return valor;
        }

        cout << "Entrada invalida. Introduce un valor numerico igual o mayor que cero.\n";
        cin.clear();
        limpiarEntrada();
    }
}

string leerTexto(const string& mensaje) {
    string texto;

    do {
        cout << mensaje;
        getline(cin, texto);

        if (texto.empty()) {
            cout << "El texto no puede estar vacio.\n";
        }
    } while (texto.empty());

    return texto;
}

// Plantilla generica para mostrar diferentes colecciones vector<T>.
template <typename T, typename Funcion>
void mostrarColeccion(
    const vector<T>& coleccion,
    Funcion imprimir,
    const string& mensajeVacio
) {
    if (coleccion.empty()) {
        cout << mensajeVacio << '\n';
        return;
    }

    for (const auto& elemento : coleccion) {
        imprimir(elemento);
    }
}

// ==================== CENTROS DE SALUD ====================

void registrarCentro() {
    CentroSalud c;

    c.id = leerEntero("ID del centro: ", 1);

    if (centrosPorId.count(c.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    c.nombre = leerTexto("Nombre del centro: ");
    c.direccion = leerTexto("Direccion: ");

    centrosSalud.push_back(c);
    centrosPorId[c.id] = c;

    cout << "Centro registrado.\n";
}

void mostrarCentros() {
    cout << "\n--- CENTROS DE SALUD ---\n";

    mostrarColeccion(centrosSalud, [](const CentroSalud& c) {
        cout << "ID: " << c.id
             << " | " << c.nombre
             << " | Direccion: " << c.direccion << '\n';
    }, "No hay centros registrados.");
}

// ==================== MEDICOS ====================

void registrarMedico() {
    Medico m;

    m.id = leerEntero("ID del medico: ", 1);

    if (medicosPorId.count(m.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    m.nombre = leerTexto("Nombre del medico: ");
    m.especialidad = leerTexto("Especialidad: ");

    medicos.push_back(m);
    medicosPorId[m.id] = m;

    cout << "Medico registrado.\n";
}

void mostrarMedicos() {
    cout << "\n--- MEDICOS ---\n";

    mostrarColeccion(medicos, [](const Medico& m) {
        cout << "ID: " << m.id
             << " | " << m.nombre
             << " | Especialidad: " << m.especialidad << '\n';
    }, "No hay medicos registrados.");
}

// ==================== DIAGNOSTICOS ====================

void registrarDiagnostico() {
    Diagnostico d;

    d.id = leerEntero("ID del diagnostico: ", 1);

    if (diagnosticosPorId.count(d.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    d.nombre = leerTexto(
        "Nombre del diagnostico (ej. Obesidad, Cancer, Diabetes): "
    );
    d.descripcion = leerTexto("Descripcion: ");

    diagnosticos.push_back(d);
    diagnosticosPorId[d.id] = d;

    cout << "Diagnostico registrado.\n";
}

void mostrarDiagnosticos() {
    cout << "\n--- DIAGNOSTICOS ---\n";

    mostrarColeccion(diagnosticos, [](const Diagnostico& d) {
        cout << "ID: " << d.id
             << " | " << d.nombre
             << " | " << d.descripcion << '\n';
    }, "No hay diagnosticos registrados.");
}

// ==================== TRATAMIENTOS ====================

void registrarTratamiento() {
    Tratamiento t;

    t.id = leerEntero("ID del tratamiento: ", 1);

    if (tratamientosPorId.count(t.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    t.nombre = leerTexto("Nombre del tratamiento: ");
    t.descripcion = leerTexto("Descripcion: ");

    tratamientos.push_back(t);
    tratamientosPorId[t.id] = t;

    cout << "Tratamiento registrado.\n";
}

void mostrarTratamientos() {
    cout << "\n--- TRATAMIENTOS ---\n";

    mostrarColeccion(tratamientos, [](const Tratamiento& t) {
        cout << "ID: " << t.id
             << " | " << t.nombre
             << " | " << t.descripcion << '\n';
    }, "No hay tratamientos registrados.");
}

// ==================== ARS ====================

void registrarARS() {
    ARS a;

    a.id = leerEntero("ID de la ARS: ", 1);

    if (arsPorId.count(a.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    a.nombre = leerTexto("Nombre de la ARS: ");

    do {
        a.porcentajeCobertura =
            leerDinero("Porcentaje de cobertura (0-100): ");

        if (a.porcentajeCobertura > 100) {
            cout << "El porcentaje no puede superar 100.\n";
        }
    } while (a.porcentajeCobertura > 100);

    aseguradoras.push_back(a);
    arsPorId[a.id] = a;

    cout << "ARS registrada.\n";
}

void mostrarARS() {
    cout << "\n--- ASEGURADORAS ---\n";

    mostrarColeccion(aseguradoras, [](const ARS& a) {
        cout << "ID: " << a.id
             << " | " << a.nombre
             << " | Cobertura: " << a.porcentajeCobertura << "%\n";
    }, "No hay ARS registradas.");
}

// ==================== PACIENTES ====================

void registrarPaciente() {
    if (centrosSalud.empty() ||
        diagnosticos.empty() ||
        tratamientos.empty() ||
        aseguradoras.empty()) {
        cout << "Primero registra por lo menos un centro, diagnostico, tratamiento y ARS.\n";
        return;
    }

    Paciente p;

    p.id = leerEntero("ID del paciente: ", 1);

    if (pacientesPorId.count(p.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    p.nombre = leerTexto("Nombre completo: ");
    p.edad = leerEntero("Edad: ", 0);

    mostrarCentros();
    p.idCentro = leerEntero("ID del centro: ", 1);

    if (!centrosPorId.count(p.idCentro)) {
        cout << "Centro inexistente.\n";
        return;
    }

    mostrarDiagnosticos();
    p.idDiagnostico = leerEntero("ID del diagnostico: ", 1);

    if (!diagnosticosPorId.count(p.idDiagnostico)) {
        cout << "Diagnostico inexistente.\n";
        return;
    }

    mostrarTratamientos();
    p.idTratamiento = leerEntero("ID del tratamiento: ", 1);

    if (!tratamientosPorId.count(p.idTratamiento)) {
        cout << "Tratamiento inexistente.\n";
        return;
    }

    mostrarARS();
    p.idARS = leerEntero("ID de la ARS: ", 1);

    if (!arsPorId.count(p.idARS)) {
        cout << "ARS inexistente.\n";
        return;
    }

    pacientes.push_back(p);
    pacientesPorId[p.id] = p;

    cout << "Paciente registrado.\n";
}

void mostrarPacientes() {
    cout << "\n--- PACIENTES ---\n";

    mostrarColeccion(pacientes, [](const Paciente& p) {
        cout << "\nID: " << p.id
             << " | Nombre: " << p.nombre
             << " | Edad: " << p.edad
             << "\nCentro: " << centrosPorId.at(p.idCentro).nombre
             << "\nDiagnostico: " << diagnosticosPorId.at(p.idDiagnostico).nombre
             << "\nTratamiento: " << tratamientosPorId.at(p.idTratamiento).nombre
             << "\nARS: " << arsPorId.at(p.idARS).nombre << '\n';
    }, "No hay pacientes registrados.");
}

void pacientesPorCentro() {
    if (centrosSalud.empty()) {
        cout << "No hay centros registrados.\n";
        return;
    }

    mostrarCentros();

    int id = leerEntero("ID del centro: ", 1);

    if (!centrosPorId.count(id)) {
        cout << "Centro inexistente.\n";
        return;
    }

    int total = 0;

    for (const auto& p : pacientes) {
        if (p.idCentro == id) {
            cout << "ID: " << p.id
                 << " | " << p.nombre
                 << " | Edad: " << p.edad << '\n';
            ++total;
        }
    }

    cout << "Total de pacientes del centro: " << total << '\n';
}

void pacientesPorDiagnostico() {
    if (diagnosticos.empty()) {
        cout << "No hay diagnosticos registrados.\n";
        return;
    }

    mostrarDiagnosticos();

    int id = leerEntero("ID del diagnostico: ", 1);

    if (!diagnosticosPorId.count(id)) {
        cout << "Diagnostico inexistente.\n";
        return;
    }

    int total = 0;

    for (const auto& p : pacientes) {
        if (p.idDiagnostico == id) {
            cout << "ID: " << p.id << " | " << p.nombre << '\n';
            ++total;
        }
    }

    cout << "Total encontrados: " << total << '\n';
}

void pacientesPorTratamiento() {
    if (tratamientos.empty()) {
        cout << "No hay tratamientos registrados.\n";
        return;
    }

    mostrarTratamientos();

    int id = leerEntero("ID del tratamiento: ", 1);

    if (!tratamientosPorId.count(id)) {
        cout << "Tratamiento inexistente.\n";
        return;
    }

    int total = 0;

    for (const auto& p : pacientes) {
        if (p.idTratamiento == id) {
            cout << "ID: " << p.id << " | " << p.nombre << '\n';
            ++total;
        }
    }

    cout << "Total encontrados: " << total << '\n';
}

// ==================== CONSULTAS MEDICAS ====================

void registrarConsulta() {
    if (pacientes.empty() || medicos.empty()) {
        cout << "Primero registra al menos un paciente y un medico.\n";
        return;
    }

    Consulta c;

    c.id = leerEntero("ID de la consulta: ", 1);

    if (consultasPorId.count(c.id)) {
        cout << "Ese ID ya existe.\n";
        return;
    }

    mostrarPacientes();

    c.idPaciente = leerEntero("ID del paciente: ", 1);

    if (!pacientesPorId.count(c.idPaciente)) {
        cout << "Paciente inexistente.\n";
        return;
    }

    mostrarMedicos();

    c.idMedico = leerEntero("ID del medico: ", 1);

    if (!medicosPorId.count(c.idMedico)) {
        cout << "Medico inexistente.\n";
        return;
    }

    const Paciente& p = pacientesPorId.at(c.idPaciente);
    c.idCentro = p.idCentro;
    c.costoTotal = leerDinero("Costo total de la consulta: RD$: ");

    const ARS& a = arsPorId.at(p.idARS);

    c.coberturaARS =
        c.costoTotal * a.porcentajeCobertura / 100.0;

    double saldo = c.costoTotal - c.coberturaARS;

    cout << fixed << setprecision(2)
         << "Cobertura estimada de la ARS (" << a.porcentajeCobertura
         << "%): RD$ " << c.coberturaARS
         << "\nMonto restante para el paciente: RD$ " << saldo << '\n';

    do {
        c.pagoPaciente =
            leerDinero("Pago realizado por el paciente: RD$: ");

        if (c.pagoPaciente > saldo) {
            cout << "El pago no puede superar RD$ " << saldo << ".\n";
        }
    } while (c.pagoPaciente > saldo);

    consultas.push_back(c);
    consultasPorId[c.id] = c;

    cout << "Consulta registrada. Diferencia pendiente: RD$ "
         << c.diferenciaPendiente() << '\n';
}

void mostrarConsultas() {
    cout << fixed << setprecision(2) << "\n--- CONSULTAS ---\n";

    mostrarColeccion(consultas, [](const Consulta& c) {
        cout << "\nConsulta: " << c.id
             << " | Paciente: " << pacientesPorId.at(c.idPaciente).nombre
             << " | Medico: " << medicosPorId.at(c.idMedico).nombre
             << " | Centro: " << centrosPorId.at(c.idCentro).nombre
             << "\nCosto total: RD$ " << c.costoTotal
             << " | Cobertura ARS: RD$ " << c.coberturaARS
             << " | Pago paciente: RD$ " << c.pagoPaciente
             << " | Diferencia pendiente: RD$ " << c.diferenciaPendiente() << '\n';
    }, "No hay consultas registradas.");
}

void resumenConsultasPorCentro() {
    if (centrosSalud.empty()) {
        cout << "No hay centros registrados.\n";
        return;
    }

    cout << fixed << setprecision(2)
         << "\n--- RESUMEN DE CONSULTAS POR CENTRO ---\n";

    for (const auto& centro : centrosSalud) {
        int cantidad = 0;
        double costo = 0;
        double cobertura = 0;
        double pagos = 0;
        double pendiente = 0;

        for (const auto& c : consultas) {
            if (c.idCentro == centro.id) {
                ++cantidad;
                costo += c.costoTotal;
                cobertura += c.coberturaARS;
                pagos += c.pagoPaciente;
                pendiente += c.diferenciaPendiente();
            }
        }

        cout << "\nCentro: " << centro.nombre
             << "\nCantidad de consultas: " << cantidad
             << "\nCosto total: RD$ " << costo
             << "\nCobertura ARS: RD$ " << cobertura
             << "\nPago de pacientes: RD$ " << pagos
             << "\nDiferencia pendiente: RD$ " << pendiente << '\n';

        if (cantidad > 0) {
            cout << "Costo promedio por consulta: RD$ "
                 << costo / cantidad << '\n';
        }
    }
}

// ==================== ESTADISTICAS Y CINCO NIVELES ====================

string nivelImpacto(double porcentaje) {
    // Clasificacion estadistica del ejercicio; no es un criterio clinico.
    if (porcentaje < 5.0) return "Muy bajo";
    if (porcentaje < 10.0) return "Bajo";
    if (porcentaje < 20.0) return "Moderado";
    if (porcentaje < 35.0) return "Alto";
    return "Muy alto";
}

void estadisticasGenerales() {
    cout << fixed << setprecision(2)
         << "\n--- ESTADISTICAS GENERALES ---\n";

    cout << "Pacientes: " << pacientes.size()
         << "\nCentros: " << centrosSalud.size()
         << "\nMedicos: " << medicos.size()
         << "\nDiagnosticos: " << diagnosticos.size()
         << "\nTratamientos: " << tratamientos.size()
         << "\nARS: " << aseguradoras.size()
         << "\nConsultas: " << consultas.size() << '\n';

    double costoTotal = 0;
    double coberturaTotal = 0;
    double pagoTotal = 0;
    double pendienteTotal = 0;

    for (const auto& c : consultas) {
        costoTotal += c.costoTotal;
        coberturaTotal += c.coberturaARS;
        pagoTotal += c.pagoPaciente;
        pendienteTotal += c.diferenciaPendiente();
    }

    cout << "\nCosto global acumulado: RD$ " << costoTotal
         << "\nCobertura global de ARS: RD$ " << coberturaTotal
         << "\nPagos globales de pacientes: RD$ " << pagoTotal
         << "\nDiferencia global pendiente: RD$ " << pendienteTotal << '\n';

    if (!consultas.empty()) {
        cout << "Costo promedio global por consulta: RD$ "
             << costoTotal / consultas.size() << '\n';
    } else {
        cout << "No hay consultas para calcular el promedio global.\n";
    }

    if (!pacientes.empty()) {
        cout << "Promedio de consultas por paciente: "
             << static_cast<double>(consultas.size()) / pacientes.size() << '\n';
    } else {
        cout << "No hay pacientes para calcular ese promedio.\n";
    }

    if (!centrosSalud.empty()) {
        cout << "Promedio de consultas por centro: "
             << static_cast<double>(consultas.size()) / centrosSalud.size() << '\n';
    } else {
        cout << "No hay centros para calcular ese promedio.\n";
    }
}

bool esObesidad(const string& nombre) {
    return minusculas(nombre).find("obesidad") != string::npos;
}

bool esCancer(const string& nombre) {
    string n = minusculas(nombre);
    return n.find("cancer") != string::npos ||
           n.find("cáncer") != string::npos;
}

void estadisticasEnfermedades() {
    cout << fixed << setprecision(2)
         << "\n--- ESTADISTICAS POBLACIONALES DE ENFERMEDADES ---\n";

    int total = static_cast<int>(pacientes.size());

    if (total == 0) {
        cout << "No hay pacientes registrados.\n";
        return;
    }

    int obesidad = 0;
    int cancer = 0;
    int otros = 0;
    map<int, int> conteo;

    for (const auto& p : pacientes) {
        ++conteo[p.idDiagnostico];

        const string& nombre =
            diagnosticosPorId.at(p.idDiagnostico).nombre;

        if (esObesidad(nombre)) {
            ++obesidad;
        } else if (esCancer(nombre)) {
            ++cancer;
        } else {
            ++otros;
        }
    }

    auto imprimir = [total](const string& nombre, int cantidad) {
        double porcentaje = 100.0 * cantidad / total;

        cout << nombre << ": " << cantidad
             << " paciente(s), " << porcentaje
             << "% | Nivel estadistico: " << nivelImpacto(porcentaje) << '\n';
    };

    cout << "Poblacion registrada: " << total << " paciente(s)\n";

    imprimir("Obesidad", obesidad);
    imprimir("Cancer", cancer);
    imprimir("Otras enfermedades", otros);

    cout << "\nDetalle por diagnostico:\n";

    for (const auto& par : conteo) {
        imprimir(diagnosticosPorId.at(par.first).nombre, par.second);
    }

    cout << "\nNiveles usados: Muy bajo (<5%), Bajo (5-<10%), "
         << "Moderado (10-<20%), Alto (20-<35%), Muy alto (35% o mas). "
         << "Son niveles estadisticos del proyecto.\n";
}

void estadisticasPorCentro() {
    cout << fixed << setprecision(2)
         << "\n--- ESTADISTICAS POR CENTRO DE SALUD ---\n";

    if (centrosSalud.empty()) {
        cout << "No hay centros registrados.\n";
        return;
    }

    for (const auto& centro : centrosSalud) {
        int total = 0;
        int obesidad = 0;
        int cancer = 0;
        int otros = 0;

        for (const auto& p : pacientes) {
            if (p.idCentro == centro.id) {
                ++total;

                const string& nombre =
                    diagnosticosPorId.at(p.idDiagnostico).nombre;

                if (esObesidad(nombre)) {
                    ++obesidad;
                } else if (esCancer(nombre)) {
                    ++cancer;
                } else {
                    ++otros;
                }
            }
        }

        cout << "\nCentro: " << centro.nombre
             << " | Pacientes: " << total << '\n';

        if (total == 0) {
            cout << "Sin pacientes registrados en este centro.\n";
            continue;
        }

        auto imprimir = [total](const string& nombre, int cantidad) {
            double porcentaje = 100.0 * cantidad / total;

            cout << nombre << ": " << cantidad
                 << " (" << porcentaje << "%) | Nivel: "
                 << nivelImpacto(porcentaje) << '\n';
        };

        imprimir("Obesidad", obesidad);
        imprimir("Cancer", cancer);
        imprimir("Otras enfermedades", otros);
    }
}

// ==================== MENU PRINCIPAL ====================

void menu() {
    cout << "\n========== SISTEMA DE GESTION GENERICA DE SALUD ==========\n"
         << " 1. Registrar paciente\n"
         << " 2. Registrar centro de salud\n"
         << " 3. Mostrar pacientes\n"
         << " 4. Mostrar centros\n"
         << " 5. Buscar pacientes por centro\n"
         << " 6. Buscar pacientes por diagnostico\n"
         << " 7. Buscar pacientes por tratamiento\n"
         << " 8. Registrar consulta\n"
         << " 9. Mostrar consultas\n"
         << "10. Resumen de consultas por centro\n"
         << "11. Estadisticas generales y promedios\n"
         << "12. Registrar medico\n"
         << "13. Mostrar medicos\n"
         << "14. Registrar diagnostico\n"
         << "15. Mostrar diagnosticos\n"
         << "16. Registrar tratamiento\n"
         << "17. Mostrar tratamientos\n"
         << "18. Registrar ARS\n"
         << "19. Mostrar ARS\n"
         << "20. Estadisticas poblacionales de enfermedades\n"
         << "21. Estadisticas por centro de salud\n"
         << " 0. Salir\n";
}

int main() {
    cout << fixed << setprecision(2);

    int opcion;

    do {
        menu();
        opcion = leerEntero("Selecciona una opcion: ", 0);

        switch (opcion) {
            case 1: registrarPaciente(); break;
            case 2: registrarCentro(); break;
            case 3: mostrarPacientes(); break;
            case 4: mostrarCentros(); break;
            case 5: pacientesPorCentro(); break;
            case 6: pacientesPorDiagnostico(); break;
            case 7: pacientesPorTratamiento(); break;
            case 8: registrarConsulta(); break;
            case 9: mostrarConsultas(); break;
            case 10: resumenConsultasPorCentro(); break;
            case 11: estadisticasGenerales(); break;
            case 12: registrarMedico(); break;
            case 13: mostrarMedicos(); break;
            case 14: registrarDiagnostico(); break;
            case 15: mostrarDiagnosticos(); break;
            case 16: registrarTratamiento(); break;
            case 17: mostrarTratamientos(); break;
            case 18: registrarARS(); break;
            case 19: mostrarARS(); break;
            case 20: estadisticasEnfermedades(); break;
            case 21: estadisticasPorCentro(); break;
            case 0:
                cout << "Sistema finalizado. Recuerda que los datos no se guardan al cerrar el programa.\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }

    } while (opcion != 0);

    return 0;
}
