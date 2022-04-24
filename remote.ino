#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int i = 2; //( "Student's identity (0-49)" )
int t = 60;

RF24 radio(7, 8);
const byte address[][6] = {"00", "01"}; //reading pipe, writing pipe
int j = 1;
int k ;
int z ;
unsigned long previousMillis ;

void setup() {
  pinMode(3, INPUT_PULLUP); // for selecting option "A"
  pinMode(4, INPUT_PULLUP); // for selecting option "B"
  pinMode(5, INPUT_PULLUP); // for selecting option "C"
  pinMode(6, INPUT_PULLUP); // for selecting option "D"

  pinMode(9, OUTPUT); // LED for option A
  pinMode(10, OUTPUT); // LED for option B
  pinMode(2, OUTPUT); // LED for option C
  pinMode(A0, OUTPUT); // LED for option D

  pinMode(A1, OUTPUT); // "A1 to A5" timing LEDs
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(2, LOW);
  digitalWrite(A0, LOW);

  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);

  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);

  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.setRetries(15, 15);
  radio.setChannel(i);
}

void loop() {

  radio.openReadingPipe(1, address[0]);
  radio.startListening();
  char text1[32] = "";

  if (radio.available() && j == 1) {
    while (radio.available()) {
      char text1[32] = "";
      radio.read(&text1, sizeof(text1));
      z = text1[0];
      j = 0;
      k = 0;
      previousMillis = millis();
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      digitalWrite(A4, HIGH);
      digitalWrite(A5, HIGH);
    }
  }
  if (k == z) {
    j = 1; // reseting for next test
  }

  if (j == 0 && k < z) {

    const long interval = t * 1000;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      k = ++k;
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(2, LOW);
      digitalWrite(A0, LOW);

      digitalWrite(A1, LOW);
      delay (1000);
      if (k < z) {
        digitalWrite(A1, HIGH);
        digitalWrite(A2, HIGH);
        digitalWrite(A3, HIGH);
        digitalWrite(A4, HIGH);
        digitalWrite(A5, HIGH);
      }
    }
    if (currentMillis - previousMillis >= (interval / 5)) {
      digitalWrite(A5, LOW);
    }
    if (currentMillis - previousMillis >= ((interval / 5) * 2)) {
      digitalWrite(A4, LOW);
    }
    if (currentMillis - previousMillis >= ((interval / 5) * 3)) {
      digitalWrite(A3, LOW);
    }
    if (currentMillis - previousMillis >= ((interval / 5) * 4)) {
      digitalWrite(A2, LOW);
    }

    radio.openWritingPipe(address[1]);
    radio.stopListening();

    if (digitalRead(3) == LOW) {
      const char text[] = "A";
      radio.write(&text, sizeof(text));
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(2, LOW);
      digitalWrite(A0, LOW);
    }
    if (digitalRead(4) == LOW) {
      const char text[] = "B";
      radio.write(&text, sizeof(text));
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(2, LOW);
      digitalWrite(A0, LOW);
    }
    if (digitalRead(5) == LOW) {
      const char text[] = "C";
      radio.write(&text, sizeof(text));
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(2, HIGH);
      digitalWrite(A0, LOW);
    }
    if (digitalRead(6) == LOW) {
      const char text[] = "D";
      radio.write(&text, sizeof(text));
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(2, LOW);
      digitalWrite(A0, HIGH);
    }
  }
}
