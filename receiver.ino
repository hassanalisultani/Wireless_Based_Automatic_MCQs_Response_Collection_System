#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int ns = 10; //( "Number of Students (1-50)" )
int t = 60; // ( "Time for One Question (in seconds)" )

char ans[] = {'A','B','A','C','D','A','B','A','C','D'}; //( "Enter answers in sequence" )
int z = sizeof(ans);

RF24 radio(7, 8);
const byte address[][6] = {"00", "01"}; //writing pipe, reading pipe
int stdn[50]; // array for saving students' results
char j[50]; // array for receiving answers

unsigned long previousMillis ;
const long interval = t * 1000;
char text[32] = "";
int k = 0;
int f = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);

  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.setRetries(15, 15);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);

}

void loop() {
//  while (f == 2) {
//    static uint8_t  putIndex = 0;
//    while (Serial.available()){
//      ans[putIndex] = (char)Serial.read();
//      putIndex++;
//    }
//    
//  }
  while (f == 0) {
    for (int b = 0 ; b < ns ; b++) {
      radio.openWritingPipe(address[0]);
      radio.stopListening();
      radio.setChannel(b);
      const char text1[] = {z};
      radio.write(&text1, sizeof(text1));
    }
    previousMillis = millis();
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    f = 1;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval && k <= z) {
    previousMillis = currentMillis;
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    Serial.print( "\t" );
    for (int e = 0 ; e < ns ; e++) {
      if (j[e] == ans[k]) {
        stdn[e] += 1 ;
      }
    }
    memset(j, 0, sizeof(j));
    k = ++k;
  }

  if (k == z) {
    Serial.print("\n");
    Serial.print("Student's Identity");
    Serial.print("\t");
    Serial.print("Marks");
    Serial.print("\n");
    for (int a = 0 ; a < ns ; a++) {
      Serial.print("\t");
      Serial.print(a);
      Serial.print("\t");
      Serial.print("\t");
      Serial.print(stdn[a]);
      Serial.print("\n");
    }
    k = ++k;
  }

  for (int i = 0; i <= ns; i++) {
    radio.openReadingPipe(1, address[1]);
    radio.startListening();
    radio.setChannel(i);

    if (radio.available()) {
      while (radio.available() && k <= z) {
        radio.read(&text, sizeof(text));
      }
      j[i - 1] = text[0];
    }
  }
}
