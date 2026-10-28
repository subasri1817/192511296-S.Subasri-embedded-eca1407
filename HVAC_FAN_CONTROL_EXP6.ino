
/*
  Project: HVAC_FAN_CONTROL
  Board: Arduino UNO
  Sensor: LM35 Temperature Sensor
  Output: DC Fan using PWM
  Control: Timer1 Interrupt
*/

#define TEMP_SENSOR A0
#define FAN_PIN 9

volatile byte fanPWM = 0;
volatile byte pwmCounter = 0;

float temperature = 0.0;

void setup() {
  Serial.begin(9600);

  pinMode(TEMP_SENSOR, INPUT);
  pinMode(FAN_PIN, OUTPUT);

  // Configure Timer1 for approximately 8-bit
  // software PWM using a timer interrupt.
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 16 MHz / 64 / (249 + 1) = 1000 Hz
  OCR1A = 249;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  interrupts();

  Serial.println("HVAC FAN CONTROL SYSTEM");
  Serial.println("-----------------------");
}

ISR(TIMER1_COMPA_vect) {
  pwmCounter++;

  if (pwmCounter < fanPWM) {
    PORTB |= (1 << PB1);   // D9 HIGH
  } else {
    PORTB &= ~(1 << PB1);  // D9 LOW
  }
}

void loop() {
  // Read analog temperature sensor
  int adcValue = analogRead(TEMP_SENSOR);

  // Convert ADC value to voltage
  float voltage = adcValue * (5.0 / 1023.0);

  // LM35 output: 10 mV per degree Celsius
  temperature = voltage * 100.0;

  // Select fan speed according to temperature
  byte speed;

  if (temperature < 25.0) {
    speed = 0;
  } else if (temperature < 30.0) {
    speed = 100;
  } else if (temperature < 35.0) {
    speed = 180;
  } else {
    speed = 255;
  }

  // Update PWM duty cycle safely
  noInterrupts();
  fanPWM = speed;
  interrupts();

  Serial.print("ADC Value: ");
  Serial.println(adcValue);

  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  Serial.print("Fan PWM: ");
  Serial.print(speed);
  Serial.print(" / 255  (");
  Serial.print((speed * 100UL) / 255);
  Serial.println("%)");

  Serial.println("-----------------------");

  delay(1000);
}
