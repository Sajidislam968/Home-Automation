#include <Keypad.h>
#include <ESP32Servo.h>

#define SERVO_PIN  15 
#define ROW_NUM     4
#define COLUMN_NUM  4

char key_layout[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {13, 12, 14, 27};
byte pin_column[COLUMN_NUM] = {26, 25, 33, 32};

Keypad keypad = Keypad( makeKeymap(key_layout), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo servo; // servo motor

const String password_1 = "ABC1234";
//const String password_2 = "5642B";
//const String password_3 = "9765";
String input_password;

int angle = 90;
unsigned long prev_time;

void setup() {
  Serial.begin(9600);
  input_password.reserve(32);
  servo.attach(SERVO_PIN);
  servo.write(90);
  prev_time = millis();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      input_password = "";
    } else if (key == '#') {
      if (input_password == password_1) {
        Serial.println("The correct password! Rotating Servo Motor to 0°");
        angle = 0;
        servo.write(angle);
        prev_time = millis();
      } else {
        Serial.println("The incorrect password! try again");
      }

      input_password = "";
    } else {
      input_password += key;
    }
  }

  if (angle == 0 && (millis() - prev_time) > 5000) {
    angle = 90;
    servo.write(angle);
    Serial.println("Rotating Servo Motor to 90°");
  }
}
