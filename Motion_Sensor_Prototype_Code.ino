int ledPin = 8;                  // LED
int pirPin = 2;                   // PIR Out pin
int pirStat = 0;                  // PIR status
int buzzer = 9;
int pirPin2 = 3;
int pirStat2 = 0;

void setup() {
  pinMode(ledPin, OUTPUT);        // declare LED as output
  pinMode(pirPin, INPUT);         // declare sensor as input
  pinMode(buzzer, OUTPUT);
  pinMode(pirPin2, INPUT);
  Serial.begin(9600);
  
}

void loop(){
 pirStat = digitalRead(pirPin); 
 pirStat2 = digitalRead(pirPin2);
 if (pirStat == HIGH || pirStat2 == HIGH) {             // if motion detected
  //  Serial.println("Motion Detected!");
  //  digitalWrite(ledPin, HIGH);      // turn LED ON
  //  delay(1000);                      // wait for a second
  //  digitalWrite(ledPin, LOW);       // turn LED OFF
  //  delay(1000);                      // wait for a second
  if(pirStat == HIGH){
    digitalWrite(ledPin, HIGH);
    
    Serial.print("Light On\n");
    delay(2000);
    digitalWrite(ledPin, LOW);
  }
  if(pirStat2 == HIGH){
    Serial.print("Intruder Detected...\n");
    for(int i =0; i<2; i++){
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(500);
    }
  }
 } 
 else {
   digitalWrite(ledPin, LOW);       // LED stays off if no motion detected
   Serial.println("No Motion Detected!");
   delay(1000);
 }
} 
