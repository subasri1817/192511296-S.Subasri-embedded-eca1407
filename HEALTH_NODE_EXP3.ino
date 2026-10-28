#include <Wire.h>

// I2C addresses
#define MAX30102_ADDR 0x57
#define LM75_ADDR     0x48

// LM75 temperature sensor
float readTemperature() {
  Wire.beginTransmission(LM75_ADDR);
  Wire.write(0x00);  // Temperature register

  if (Wire.endTransmission(false) != 0) {
    return -999.0;
  }

  Wire.requestFrom(LM75_ADDR, 2);

  if (Wire.available() < 2) {
    return -999.0;
  }

  int msb = Wire.read();
  int lsb = Wire.read();

  int raw = (msb << 8) | lsb;
  raw >>= 7;

  if (raw & 0x100) {
    raw -= 512;
  }

  return raw * 0.5;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("I2C HEALTH MONITOR");
  Serial.println("------------------");

  // Check temperature sensor
  Wire.beginTransmission(LM75_ADDR);

  if (Wire.endTransmission() == 0) {
    Serial.println("LM75 temperature sensor detected");
  } else {
    Serial.println("LM75 sensor not detected");
  }

  // Check pulse sensor
  Wire.beginTransmission(MAX30102_ADDR);

  if (Wire.endTransmission() == 0) {
    Serial.println("MAX30102 pulse sensor detected");
  } else {
    Serial.println("MAX30102 sensor not detected");
  }
}

void loop() {
  // Read temperature
  float temperature = readTemperature();

  // Display temperature
  if (temperature != -999.0) {
    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.println(" C");
  } else {
    Serial.println("Temperature sensor error");
  }

  // Pulse measurement requires MAX30102 initialization,
  // LED control and optical signal processing.
  Serial.println("Pulse: Use MAX30102 library for BPM measurement");

  Serial.println("------------------");

  delay(2000);
}
