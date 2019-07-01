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

void setup() {
  
}

void loop() {
  
}
