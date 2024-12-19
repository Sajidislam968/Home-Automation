#define MQ2pin 7

int sensorValue;  //variable to store sensor value

void setup() {
	Serial.begin(9600); // sets the serial port to 9600
	Serial.println("MQ2 warming up!");
	delay(4000); // allow the MQ2 to warm up
  pinMode(13, OUTPUT);
}

void loop() {
	sensorValue = digitalRead(MQ2pin); // read digital output pin
	Serial.print("Digital Output: ");
	Serial.print(sensorValue);
	
	// Determine the status
	if (sensorValue) {
		Serial.println("  |  Smoke: -");
	} else {
		Serial.println("  |  Smoke: Detected!");
    digitalWrite(13, HIGH);
    delay(2000);
    digitalWrite(13, LOW);
	}
	
	delay(2000); // wait 2s for next reading
}