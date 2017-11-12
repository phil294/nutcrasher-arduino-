#include <Adafruit_NeoPixel.h>

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
#define PIN_LED_1_DI 8
#define PIN_LED_2_DI 9
#define LED_1_COUNT 8
#define LED_2_COUNT 12

bool motor_moving = false;
void motor_reset() {
//  Serial.println("reset");
  digitalWrite(PIN_MOTOR_X_POWER,LOW);
  digitalWrite(PIN_MOTOR_X_GROUND,LOW);
  digitalWrite(PIN_MOTOR_I_POWER,LOW);
  digitalWrite(PIN_MOTOR_I_GROUND,LOW);
}
void motor_left() {
//  Serial.println("left");
  motor_reset();
  digitalWrite(PIN_MOTOR_X_POWER, HIGH);
  digitalWrite(PIN_MOTOR_I_GROUND, HIGH);
  motor_moving = true;
}
void motor_right() {
//  Serial.println("right");
  motor_reset();
  digitalWrite(PIN_MOTOR_I_POWER, HIGH);
  digitalWrite(PIN_MOTOR_X_GROUND, HIGH);
  motor_moving = true;
}
void motor_stop() {
//  Serial.println("stop");
  motor_reset();
  // todo evtl power+power high
  motor_moving = false;
}

Adafruit_NeoPixel leds_1 = Adafruit_NeoPixel(LED_1_COUNT, PIN_LED_1_DI, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds_2 = Adafruit_NeoPixel(LED_2_COUNT, PIN_LED_2_DI, NEO_GRB + NEO_KHZ800);

/*void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<LED_1_COUNT; j++) {
    for(i=0; i< LED_1_COUNT; i++) {
      leds_1.setPixelColor(i, Wheel(((i * 256 / LED_1_COUNT) + j) & 255));
    }
    leds_1.show();
    delay(wait);
  }
}*/
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel leds_ref) {
  if(WheelPos < 85) {
   return leds_ref.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return leds_ref.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return leds_ref.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void static_wheel(Adafruit_NeoPixel leds_ref, int LEDS_COUNT) {
  int j;
  for(j=0; j<LEDS_COUNT; j++) {
    leds_ref.setPixelColor(j, Wheel(((j * 256 / LEDS_COUNT) + j) & 255, leds_ref));
  }
  leds_ref.show();
}
void color_wipe(uint32_t c, Adafruit_NeoPixel leds_ref, int LEDS_COUNT) {
  for(uint16_t i=0; i<LEDS_COUNT; i++) {
      leds_ref.setPixelColor(i, c);
  }
  leds_ref.show();
}

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_MOTOR_X_POWER,OUTPUT);
  pinMode(PIN_MOTOR_X_GROUND,OUTPUT);
  pinMode(PIN_MOTOR_I_POWER,OUTPUT);
  pinMode(PIN_MOTOR_I_GROUND,OUTPUT);
  pinMode(PIN_BUTTON_EXT,INPUT);
  pinMode(PIN_BUTTON_RET,INPUT);
  pinMode(PIN_LED_1_DI,OUTPUT);
  pinMode(PIN_LED_1_DI,OUTPUT);

  leds_1.begin();
  leds_1.show();
  leds_2.begin();
  leds_2.show();
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
  Serial.println(motor_moving);
  if(motor_moving) {
    static_wheel(leds_1, LED_1_COUNT);
    static_wheel(leds_2, LED_2_COUNT);
  } else {
    color_wipe(0, leds_1, LED_1_COUNT);
    color_wipe(0, leds_2, LED_2_COUNT);
  }
  
  delay(1);
}


























