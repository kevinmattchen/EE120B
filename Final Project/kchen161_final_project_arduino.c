/*
 * The IR remote functions are from the following github repository:
 * https://github.com/z3t0/Arduino-IRremote.git
 *
 * I gained my understanding of the IRremote library through the code from the following user:
 * https://github.com/chauhannaman98/ArduinoCodes/blob/master/Finding-IR-Codes/sketch/sketch.ino
 * and his post on the following website:
 * https://www.hackster.io/techmirtz/finding-the-ir-codes-of-any-ir-remote-using-arduino-c7a852
*/

#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <IRremote.h>      //including infrared remote header file

  #define Power       16753245
  #define FuncStop    16769565
  #define VolUp       16736925
  #define VolDown     16754775
  #define Previous    16720605
  #define Next        16761405
  #define PlayPause   16712445
  #define Down        16769055
  #define Up          16748655
  #define Eq          16750695
  #define StRept      16756815
  #define Remote0     16738455
  #define Remote1     16724175
  #define Remote2     16718055
  #define Remote3     16743045
  #define Remote4     16716015
  #define Remote5     16726215
  #define Remote6     16734885
  #define Remote7     16728765
  #define Remote8     16730805
  #define Remote9     16732845
  #define hold        4294967295

int RECV_PIN = 11;        // the pin where you connect the output pin of IR sensor 
IRrecv irrecv(RECV_PIN);
decode_results results;
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(RECV_PIN, INPUT);
  pinMode(1, OUTPUT);
}
 
void loop() {
  if (irrecv.decode(&results)) {
    #if 1
    switch (results.value) {
        case Power: Serial.write('W'); break;
        case FuncStop: Serial.write('F'); break;
        case VolUp: Serial.write('+'); break;
        case VolDown: Serial.write('-'); break;
        case Previous: Serial.write('R'); break;
        case Next: Serial.write('N'); break;
        case PlayPause: Serial.write('P'); break;
        case Down: Serial.write('D'); break;
        case Up: Serial.write('U'); break;
        case Eq: Serial.write('E'); break;
        case StRept: Serial.write('S'); break;
        case Remote0: Serial.write('0'); break;
        case Remote1: Serial.write('1'); break;
        case Remote2: Serial.write('2'); break;
        case Remote3: Serial.write('3'); break;
        case Remote4: Serial.write('4'); break;
        case Remote5: Serial.write('5'); break;
        case Remote6: Serial.write('6'); break;
        case Remote7: Serial.write('7'); break;
        case Remote8: Serial.write('8'); break;
        case Remote9: Serial.write('9'); break;
        case hold: Serial.write('H'); break;
        default: Serial.write('Z'); break;
    }
    irrecv.resume(); 
    #endif
  } else {
      Serial.write('Z');
  }
  delay(50);
}
