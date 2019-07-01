/**
 * Configuracion general del programa
 */

// Pines
constexpr int PIN_BATERIA = A0;
constexpr int PIN_SALIDA = 5;
constexpr int PIN_DISPLAY_SDA = 6;
constexpr int PIN_DISPLAY_SCL = 7;
// Constantes de promedios
constexpr int MUESTRAS_PROMEDIO_BATERIA = 100;
constexpr int TIEMPO_ENTRE_MUESTRAS_BATERIA = 10;
constexpr int MUESTRAS_PROMEDIO_SALIDA = 100;
constexpr int TIEMPO_ENTRE_SALIDAS = 10;
// Constantes de tiempo
constexpr int TIEMPO_ENTRE_MSG_SERIAL = 1000;
constexpr int TIEMPO_ENTRE_ACT_DISPLAY = 250;

template<typename T, int SIZE, typename A>
class Average {
  T data[SIZE];
  T* current;
public:
  Average() : current(data) {}
  
  void add_val(T val)
  {
    *current = val;
    ++current;
    if (current >= (data + SIZE)) current = data;
  }

  T get_val()
  {
    A sum = 0;
    for (auto val : data) {
      sum += val;
    }
    return sum / SIZE;
  }
};

Average<int, MUESTRAS_PROMEDIO_BATERIA, long long> prom_bateria;
Average<double, MUESTRAS_PROMEDIO_SALIDA, double> prom_salida;

auto siguiente_lectura_bateria = 0ul;
auto siguiente_salida = 0ul;
auto siguiente_msg_serial = 0ul;
auto siguiente_act_display = 0ul;

void setup() {
  // Configurar modo de cada pin
  pinMode(PIN_BATERIA, INPUT);
  pinMode(PIN_SALIDA, OUTPUT);
  pinMode(PIN_DISPLAY_SDA, OUTPUT);
  pinMode(PIN_DISPLAY_SCL, OUTPUT);
  // Inicializar interfaz serial
  Serial.begin(9600);
}

void loop() {
  auto ahora = millis();

  // Recolectar datos de entrada
  if (ahora >= siguiente_lectura_bateria) {
    siguiente_lectura_bateria += TIEMPO_ENTRE_MUESTRAS_BATERIA;
    
    prom_bateria.add_val(analogRead(PIN_BATERIA));
  }

  // Calculos
  auto lectura_bat = prom_bateria.get_val();
  auto bateria = lectura_bat * 70.0 / 1023.0 - 7.0;
  constexpr auto x1 = 28.0, y1 = 1.0, x2 = 29.0, y2 = 0.0;
  constexpr auto m = (y2 - y1)/(x2 - x1);
  constexpr auto b = y2 - m * x2;
  auto salida_calc =  m * bateria + b;
  if (salida_calc < 0.0) salida_calc = 0.0;
  if (salida_calc > 1.0) salida_calc = 1.0;

  // Promedio salida
  auto salida = 0.0;
  if (ahora >= siguiente_salida) {
    siguiente_salida += TIEMPO_ENTRE_SALIDAS;
    
    prom_salida.add_val(salida_calc);
    salida = prom_salida.get_val();
  }

  // Sobreescritura de salida
  if (bateria >= 29.0) {
    salida = 0.0;
  }
  
  analogWrite(PIN_SALIDA, static_cast<int>(salida * 255.0));

  // Mensajes
  if (ahora >= siguiente_msg_serial) {
    siguiente_msg_serial += TIEMPO_ENTRE_MSG_SERIAL;

    Serial.print("Bateria: ");
    Serial.print(bateria);

    Serial.print(" -- Salida: ");
    Serial.print(salida);

    Serial.print(" -- Salida calc: ");
    Serial.println(salida_calc);
  }

  if (ahora >= siguiente_act_display) {
    siguiente_act_display += TIEMPO_ENTRE_ACT_DISPLAY;

    // TODO: Actualizar display
  }

}
