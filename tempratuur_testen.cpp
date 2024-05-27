const int thermistorPin = A0;
const int Rfixed = 10000;  // Fixed resistor value in the voltage divider
const float BETA = 3950.0; // Beta value
const float R0 = 10000.0;  // Resistance at 25°C
const float T0 = 298.15;   // 25°C in Kelvin

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(thermistorPin);
  float Vout = analogValue * (5.0 / 1023.0);
  float Rth = Rfixed * (5.0 / Vout - 1.0);

  float tempK = (1.0 / ((1.0 / T0) + (1.0 / BETA) * log(Rth / R0)));
  float tempC = tempK - 273.15;

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  delay(1000); // Wait a second before the next reading
}
