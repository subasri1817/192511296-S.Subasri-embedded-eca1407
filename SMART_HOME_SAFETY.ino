
#define GAS_PIN A0
#define PIR_PIN 2
#define TRIG_PIN 8
#define ECHO_PIN 9
#define BUZZER 10
#define LED 11

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  Serial.println("SMART HOME SAFETY SYSTEM");
  Serial.println("------------------------");
}

void loop() {
  // Read gas sensor
  int gas = analogRead(GAS_PIN);

  // Read PIR motion sensor
  int motion = digitalRead(PIR_PIN);

  // Ultrasonic distance measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  float distance = -1;

  if (duration > 0) {
    distance = duration * 0.0343 / 2.0;
  }

  // Gas alert
  if (gas > 500) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  // LED indicates motion
  digitalWrite(LED, motion == HIGH ? HIGH : LOW);

  // Display readings
  Serial.print("Gas Level: ");
  Serial.println(gas);

  Serial.print("Motion: ");
  if (motion == HIGH) {
    Serial.println("DETECTED");
  } else {
    Serial.println("NOT DETECTED");
  }

  Serial.print("Distance: ");
  if (distance >= 0) {
    Serial.print(distance, 1);
    Serial.println(" cm");
  } else {
    Serial.println("No echo received");
  }

  // Display safety status
  if (gas > 500) {
    Serial.println("STATUS: GAS ALERT!");
  } else if (motion == HIGH) {
    Serial.println("STATUS: OCCUPANCY DETECTED");
  } else {
    Serial.println("STATUS: SYSTEM SAFE");
  }

  Serial.println("------------------------");
  delay(1000);
}
