/*
    - Reads weight using HX711 load cell
    - Controls a servo motor
    - Drives two DC motors using L298N driver
    - Accepts serial commands for movement and servo control
*/

#include <Servo.h>
#include "HX711.h"

// --- HX711 Load Cell Setup ---
#define DOUT A1   // HX711 data pin
#define CLK  A0   // HX711 clock pin
HX711 scale;
float calibration_factor = -7050.0; // adjust this for accurate readings

// --- Servo Setup ---
Servo servo1;
#define SERVO1_PIN 9

// --- L298N Motor Driver Setup ---
int ENA = 5;  // PWM for motor A
int IN1 = 8;
int IN2 = 7;

int ENB = 6;  // PWM for motor B
int IN3 = 4;
int IN4 = 3;

char command; // Serial command variable

void setup() {
  Serial.begin(9600);

  // Load cell setup
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare(); // Reset scale to 0

  // Servo setup
  servo1.attach(SERVO1_PIN);
  servo1.write(90); // initial position
  delay(2000);

  // Motor pin setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.println("✅ System Ready!");
  Serial.println("Commands:");
  Serial.println("F = forward, B = backward, L = left, R = right, S = stop, X = servo move");
}

void loop() {
  // --- Load Cell Reading ---
  float weight = scale.get_units(5); // average of 5 readings
  Serial.print("Weight: ");
  Serial.print(weight, 2);
  Serial.println(" kg");

  // --- Serial Commands ---
  if (Serial.available()) {
    command = Serial.read();

    switch (command) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': stopMotors(); break;
      case 'X': moveServo(); break;
      default: Serial.println("⚠️ Invalid command!"); break;
    }
  }
}

// --- Motor Control Functions ---
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  Serial.println(" Moving Forward");
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  Serial.println(" Moving Backward");
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
  Serial.println(" Turning Left");
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
  Serial.println("Turning Right");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println(" Motors Stopped");
}

void moveServo() {
  Serial.println(" Moving Servo...");
  for (int pos = 0; pos <= 180; pos += 10) {
    servo1.write(pos);
    delay(100);
  }
  for (int pos = 180; pos >= 0; pos -= 10) {
    servo1.write(pos);
    delay(100);
  }
  Serial.println("Servo Movement Complete");
}

