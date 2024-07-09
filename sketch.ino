#include <Servo.h>

Servo mg90sServo;

#define trigPin_1 6
#define trigPin_2 13
#define echoPin_1 7
#define echoPin_2 12
#define greenLed 8
#define yellowLed 9
#define redLed 10
#define buzzerPin 11
#define servoPin 5 

void setup() {
  pinMode(trigPin_1, OUTPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  mg90sServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  int duration_1, duration_2, distance_1, distance_2;
  int far_distance = 50;
  int near_distance = 10;

  for (int angle = 0; angle <= 180; angle += 5) {
    mg90sServo.write(angle);
    delay(100); // Delay for servo movement
    duration_1 = getDistance(trigPin_1, echoPin_1);
    delay(60);  // Delay to avoid interference
    duration_2 = getDistance(trigPin_2, echoPin_2);

    distance_1 = duration_1 * 0.034 / 2;
    distance_2 = duration_2 * 0.034 / 2;
    Serial.println(String(angle) + "," + distance_1 + "," + (180 + angle) + "," + distance_2);
    updateLEDsAndBuzzer(distance_1, distance_2, far_distance, near_distance);
  }

  for (int angle = 180; angle >= 0; angle -= 5) {
    mg90sServo.write(angle);
    delay(100); // Delay for servo movement
    duration_1 = getDistance(trigPin_1, echoPin_1);
    delay(60);  // Delay to avoid interference
    duration_2 = getDistance(trigPin_2, echoPin_2);

    distance_1 = duration_1 * 0.034 / 2;
    distance_2 = duration_2 * 0.034 / 2;
    Serial.println(String(angle) + "," + distance_1 + "," + (180 + angle) + "," + distance_2);
    updateLEDsAndBuzzer(distance_1, distance_2, far_distance, near_distance);
  }

  delay(100); // Delay for overall loop
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}

void updateLEDsAndBuzzer(int distance_1, int distance_2, int far_distance, int near_distance) {
  if (distance_1 <= near_distance || distance_2 <= near_distance) {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
    tone(buzzerPin, 2000); 
    delay(50);
    noTone(buzzerPin);
    digitalWrite(redLed, LOW);
  } else if (distance_1 >= far_distance &&  distance_2 >= far_distance) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, LOW);
    noTone(buzzerPin);
  } else {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, LOW);
    tone(buzzerPin, 1000); 
    delay(50);
    noTone(buzzerPin);
    digitalWrite(yellowLed, LOW);
  }
}
