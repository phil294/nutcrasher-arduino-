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
#define PIN_SPEAKER 10

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

typedef struct Note {
  long freq;
  char const * name;
  int duration;
} Note;
struct Note newNote(char const* name, int freq) {
  Note note;
  note.freq = freq;
  note.name = name;
  return note;
}
        /* piezo play */
        void playNote(struct Note note, float speed) {
          Serial.print(note.freq);
          Serial.print("Hz: ");
          Serial.print(note.name);
          Serial.print(note.duration);
          Serial.println("ms");
        
          bool silent = false;
          bool dumb = false;
          
          float freq = note.freq;
          float dur = note.duration; // 400
          float delai = note.duration * 1.3; // 400*1.3=520
          dur *= speed;
          delai *= speed;
          if(silent) {
            delai += dur - 10; // 910
            dur = 10; // 10
          }
          if(dumb || freq <= 0) {
              delay(dur);
            } else {
              tone(PIN_SPEAKER, freq, dur);
          }
          delay(delai);
          //noTone(piezo);
        }

/* all notes */
Note NOTES[] {
  newNote("0", 0), newNote("B0", 31), newNote("C1", 33), newNote("CS1", 35), newNote("D1", 37), newNote("DS1", 39), newNote("E1", 41), newNote("F1", 44), newNote("FS1", 46), newNote("G1", 49), newNote("GS1", 52), newNote("A1", 55), newNote("AS1", 58), newNote("B1", 62), newNote("C2", 65), newNote("CS2", 69), newNote("D2", 73), newNote("DS2", 78), newNote("E2", 82), newNote("F2", 87), newNote("FS2", 93), newNote("G2", 98), newNote("GS2", 104), newNote("A2", 110), newNote("AS2", 117), newNote("B2", 123), newNote("C3", 131), newNote("CS3", 139), newNote("D3", 147), newNote("DS3", 156), newNote("E3", 165), newNote("F3", 175), newNote("FS3", 185), newNote("G3", 196), newNote("GS3", 208), newNote("A3", 220), newNote("AS3", 233), newNote("B3", 247), newNote("C4", 262), newNote("CS4", 277), newNote("D4", 294), newNote("DS4", 311), newNote("E4", 330), newNote("F4", 349), newNote("FS4", 370), newNote("G4", 392), newNote("GS4", 415), newNote("A4", 440), newNote("AS4", 466), newNote("B4", 494), newNote("C5", 523), newNote("CS5", 554), newNote("D5", 587), newNote("DS5", 622), newNote("E5", 659), newNote("F5", 698), newNote("FS5", 740), newNote("G5", 784), newNote("GS5", 831), newNote("A5", 880), newNote("AS5", 932), newNote("B5", 988), newNote("C6", 1047), newNote("CS6", 1109), newNote("D6", 1175), newNote("DS6", 1245), newNote("E6", 1319), newNote("F6", 1397), newNote("FS6", 1480), newNote("G6", 1568), newNote("GS6", 1661), newNote("A6", 1760), newNote("AS6", 1865), newNote("B6", 1976), newNote("C7", 2093), newNote("CS7", 2217), newNote("D7", 2349), newNote("DS7", 2489), newNote("E7", 2637), newNote("F7", 2794), newNote("FS7", 2960), newNote("G7", 3136), newNote("GS7", 3322), newNote("A7", 3520), newNote("AS7", 3729), newNote("B7", 3951), newNote("C8", 4186), newNote("CS8", 4435), newNote("D8", 4699), newNote("DS8", 4978)
};

Note makeNote(const char* name, int dur) {
  if(dur < 100) {
    dur = 1000 / dur;
  }
  for(int i=0; i<90; i++) {
    if(strcmp(name, NOTES[i].name) == 0) {
      Note ret;
      ret.duration = dur;
      ret.name = NOTES[i].name;
      ret.freq = NOTES[i].freq;
      int freq = ret.freq;
      return ret;
    }
  }
}

Note song_maus[] = {
  makeNote("F5", 400),
  makeNote("A5", 300),
  makeNote("F5", 100),
  makeNote("C5", 400),
  makeNote("C5", 400),

  makeNote("C5", 300),
  makeNote("F5", 100),
  makeNote("C5", 300),
  makeNote("A4", 100),
  makeNote("F4", 150),
  makeNote("0", 200),
  makeNote("F4", 50),
  makeNote("E4", 200),
  makeNote("0", 200),

  makeNote("F4", 400),
  makeNote("A4", 400),
  makeNote("C5", 200),
  makeNote("D5", 200),
  makeNote("0", 400),
  
  makeNote("F5", 400),
  makeNote("0", 400),
  makeNote("0", 200),
  makeNote("C5", 200),
  makeNote("A4", 400),

  makeNote("D5", 200),
  makeNote("D5", 400),
  makeNote("D5", 900),
  makeNote("A4", 100),

  makeNote("C5", 600),
  makeNote("C5", 200),
  makeNote("A4", 200),
  makeNote("C5", 400),
  makeNote("D5", 600),

  makeNote("D5", 400),
  makeNote("D5", 600),
  makeNote("A4", 200),

  makeNote("C5", 400),
  makeNote("C5", 200),
  makeNote("C5", 200),
  makeNote("A4", 200),
  makeNote("C5", 400),
  makeNote("D5", 200),

  makeNote("0", 200),
  makeNote("F5", 400),
  makeNote("F5", 200),
  makeNote("D5", 400),
  makeNote("F5", 400),

  makeNote("GS5", 400),
  makeNote("F5", 400),
  makeNote("D5", 200),
  makeNote("F5", 400),
  makeNote("G5", 2600),
  makeNote("0", 800),

  makeNote("F5", 400),
  makeNote("A5", 300),
  makeNote("F5", 100),
  makeNote("C5", 400),
  makeNote("C5", 400),

  makeNote("C5", 300),
  makeNote("F5", 100),
  makeNote("C5", 300),
  makeNote("A4", 100),
  makeNote("F4", 150),
  makeNote("0", 200),
  makeNote("F4", 50),
  makeNote("E4", 200),
  makeNote("0", 200),

  makeNote("F4", 400),
  makeNote("A4", 400),
  makeNote("C5", 200),
  makeNote("D5", 200),
  makeNote("0", 400),

  makeNote("F5", 800)
};
int song_maus_length = sizeof( song_maus) / sizeof(struct Note);
float song_maus_speed = 1.0;

Note song_imperial_march[] = {
  makeNote("A4", 400),
  makeNote("A4", 400),
  makeNote("A4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 800),
  makeNote("E5", 400),
  makeNote("E5", 400),
  makeNote("E5", 400),
  makeNote("F5", 300),
  makeNote("C5", 100),
  makeNote("GS4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 800),
  makeNote("A5", 400),
  makeNote("A4", 300),
  makeNote("A4", 100),
  makeNote("A5", 400),
  makeNote("GS5", 300),
  makeNote("G5", 100),
  makeNote("FS5", 100),
  makeNote("F5", 100),
  makeNote("FS5", 100),
  makeNote("0", 300),
  makeNote("D5", 200),
  makeNote("G5", 400),
  makeNote("FS5", 300),
  makeNote("F5", 100),
  makeNote("E5", 100),
  makeNote("DS5", 100),
  makeNote("E5", 100),
  makeNote("0", 300),
  makeNote("E4", 200),
  makeNote("G4", 400),
  makeNote("E4", 300),
  makeNote("C5", 100),
  makeNote("A4", 400),
  makeNote("F4", 300),
  makeNote("C5", 100),
  makeNote("A4", 800)
};
int song_imperial_march_length = sizeof( song_imperial_march) / sizeof(struct Note);
float song_imperial_march_speed = 1.3;

Note *song;
int song_length;
float song_speed;
int song_cur_i = 0;
bool play = false;

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

  song = song_imperial_march;
  song_length = song_imperial_march_length;
  song_speed = song_imperial_march_speed;
  song_cur_i = 0;
  play = false; // auf true ändern  = bei beginn wird die sendung mit der maus abgespielt
}

// the loop routine runs over and over again forever:
void loop() {
  // BUTTONS // todo https://www.arduino.cc/en/Reference/AttachInterrupt
  int status_ext = digitalRead(PIN_BUTTON_EXT);
  int status_ret = digitalRead(PIN_BUTTON_RET);
  // MOTOR
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
  // LEDS
  if(motor_moving) {
    static_wheel(leds_1, LED_1_COUNT);
    static_wheel(leds_2, LED_2_COUNT);
  } else {
    color_wipe(0, leds_1, LED_1_COUNT);
    color_wipe(0, leds_2, LED_2_COUNT);
  }
  // SOUND
   if (play) {
      Serial.print("no. "); Serial.println(song_cur_i);   
      playNote(song[song_cur_i], song_speed);
      song_cur_i++;
        if(song_cur_i >= song_length) {
          Serial.println("end");
          play = false;
      }
  }
  delay(1);
}


























