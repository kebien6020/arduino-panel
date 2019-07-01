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
