/*
20171111 phwa
*/

// nur X_P mit I_G kombinieren oder umgekehrt
#define PIN_MOTOR_X_POWER 5
#define PIN_MOTOR_X_GROUND 4
#define PIN_MOTOR_I_POWER 3
#define PIN_MOTOR_I_GROUND 2
#define PIN_BUTTON_EXT 6
#define PIN_BUTTON_RET 7

void motor_reset() {
  Serial.println("reset");
  digitalWrite(PIN_MOTOR_X_POWER,LOW);
  digitalWrite(PIN_MOTOR_X_GROUND,LOW);
  digitalWrite(PIN_MOTOR_I_POWER,LOW);
  digitalWrite(PIN_MOTOR_I_GROUND,LOW);
}
void motor_left() {
  Serial.println("left");
  motor_reset();
  digitalWrite(PIN_MOTOR_X_POWER, HIGH);
  digitalWrite(PIN_MOTOR_I_GROUND, HIGH);
}
void motor_right() {
  Serial.println("right");
  motor_reset();
  digitalWrite(PIN_MOTOR_I_POWER, HIGH);
  digitalWrite(PIN_MOTOR_X_GROUND, HIGH);
}
void motor_stop() {
  Serial.println("stop");
  motor_reset();
  // todo evtl power+power high
}

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_MOTOR_X_POWER,OUTPUT);
  pinMode(PIN_MOTOR_X_GROUND,OUTPUT);
  pinMode(PIN_MOTOR_I_POWER,OUTPUT);
  pinMode(PIN_MOTOR_I_GROUND,OUTPUT);

  pinMode(PIN_BUTTON_EXT,INPUT);
  pinMode(PIN_BUTTON_RET,INPUT);

  motor_right();
}

// the loop routine runs over and over again forever:
void loop() {
  int status_ext = digitalRead(PIN_BUTTON_EXT);
  int status_ret = digitalRead(PIN_BUTTON_RET);
  if(status_ext == HIGH && status_ret == HIGH) {
    motor_stop();
  } else if(status_ext == HIGH) {
    // extend
    motor_right();
  } else if(status_ret == HIGH) {
    // retract
    motor_left();
  } else {
    // stop
    motor_stop();
  }  
  delay(100);
}


























