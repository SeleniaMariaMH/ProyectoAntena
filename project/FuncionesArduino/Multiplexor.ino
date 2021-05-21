// Función multiplexor:

const int selector = 6;      // A activo a nivel bajo, B a nivel alto
const int enable = 5;        // G activo a nivel bajo (enable)

void setup() 
{
  pinMode(selector, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, LOW);        // Enable activo.
  digitalWrite(selector, LOW);      // Habilitamos comunicaciones con A (Arduino)
}

void loop() 
{
  
}

void switchUSART(const int selector, const int enable, int tiempo)
{
  digitalWrite(selector, HIGH);     // Habilitamos comunicaciones con B (GPS)
  delay(tiempo);                    // Comunicamos durante el tiempo especificado (ms)
  digitalWrite(selector, LOW);      // Habilitamos comunicaciones con A (Arduino)
}
