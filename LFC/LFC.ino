/* Arduino UNO/Leonardo pinout
  D2
  ~D3
  D4
  ~D5
  ~D6
  D7
  D8
  ~D9
  ~D10
  D3, D5 for motor enble (PWM capable)
  D6, D7, D8, D9 for motor A1,A2,B1,B2
  D4 and D10 (and maybe D2) inputs for track sensor
*/
//#include <Arduino.h>

//constexpr uint8_t G_Motor_PWM =50;
constexpr uint8_t MOTOR_A_EN  = 6;
constexpr uint8_t MOTOR_B_EN =5;
constexpr uint8_t MOTOR_A_D1 =3;
constexpr uint8_t MOTOR_A_D2 =7;
constexpr uint8_t MOTOR_B_D1 =8;
constexpr uint8_t MOTOR_B_D2 =9;
constexpr uint8_t SENSE_L_D =4;
//#define SENSE_M_D  2//currently unused
constexpr uint8_t SENSE_R_D =10;
//sensor's analog outputs; remains to be utilized
//#define SENSE_L_A A0
//#define SENSE_M_A  A1
//#define SENSE_R_A A2

//I am planning on making this a strut or something; unless you are satisfied with what you see
#define MOTOR_A 0b00010100
#define MOTOR_B 0b00001000
#define MOTOR_FWD 0b0010010 //motor forward
#define MOTOR_REV 0b01010100 //motor reverse
#define MOTOR_NHI 0b00100110 //motor neutral high
#define MOTOR_NLO 0b00101110 //motor neutral low

int16_t motor_A_Pwr;
int16_t motor_B_Pwr;
bool motor_A_Fwd;
bool motor_B_Fwd;
int choice;


int motorControl(uint8_t , uint8_t , uint8_t );

void setup
(void)
{


TCCR0B = TCCR0B & B11111000 | B00000101; // for PWM frequency of 61.04 Hz
  //InitTimersSafe();
   //bool success = 1;//SetPinFrequencySafe(PWM1, frequency);
  
   //if(!success){
   // exit(0);
   // }
   Serial.begin(9600);
  //configure pin direction
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);
  pinMode(MOTOR_A_D1, OUTPUT);
  pinMode(MOTOR_A_D2, OUTPUT);
  pinMode(MOTOR_B_D1, OUTPUT);
  pinMode(MOTOR_B_D2, OUTPUT);
  
  //turn motor pin off
  analogWrite(MOTOR_A_EN, 0);
  analogWrite(MOTOR_B_EN, 0);
  digitalWrite(MOTOR_A_D1, LOW);
  digitalWrite(MOTOR_A_D2, LOW);
  digitalWrite(MOTOR_B_D1, LOW);
  digitalWrite(MOTOR_B_D2, LOW);
  motor_A_Pwr = 0;
  motor_B_Pwr = 0;
  motor_A_Fwd = true;
  motor_B_Fwd = true;
  choice = 0;
  //while (!Serial);
  Serial.println("LFC Project");
}


void loop
(void)
{
  //check sensor for line direction
  /* NOT IMPLEMENTED
  	if (digitalRead(SENSE_L_D) == LOW) {
  		//decrease left motor speed by 10%?
  		//increase right motor speed by 10%?
  	}
  	if (digitalRead(SENSE_M_D) == LOW) {
  		//dont do anything?
  	}
  	if (digitalRead(SENSE_R_D) == LOW) {
  		//decrease right motor speed by 10%?
  		//increase left motor speed by 10%?
  	}
  */


  //
  // KEYBOARD OVERRIDE CONTROL
  //

  //Q: MOTOR A FORWARD    A: MOTOR A REVERSE
  //W: MOTOR A FAST       S: MOTOR A SLOW
  //E: MOTOR B FORWARD    D: MOTOR B REVERSE
  //R: MOTOR B FAST       F: MOTOR B SLOW


  if (Serial.available() > 0) {
    choice = Serial.read();
    Serial.println(choice);
    if (choice == 113) { //Q
      motor_A_Fwd = true;
       Serial.print("MOTOR A forward");
    }
    if (choice == 97) { //A
      motor_A_Fwd = false;
       Serial.print("MOTOR A forward");
    }
    if (choice == 119) { //W
      motor_A_Pwr = motor_A_Pwr + 10;
      Serial.print(motor_A_Pwr);
    }
    if (choice == 115) { //S
      motor_A_Pwr = motor_A_Pwr - 10;
      Serial.print(motor_A_Pwr);
    }

    if (choice == 101) { //e
      motor_B_Fwd = true;
      Serial.print("MOTOR B forward");
    }
    if (choice == 100) { //d
      motor_B_Fwd = false;
       Serial.print("MOTOR B backward");
    }
    if (choice == 114) { //r
      motor_B_Pwr = motor_B_Pwr + 10;
      Serial.print(motor_B_Pwr);
    }
    if (choice == 102) { //f
      motor_B_Pwr = motor_B_Pwr - 10;
      Serial.print(motor_B_Pwr);
    }

    if (motor_A_Pwr > 255) motor_A_Pwr = 255;
    if (motor_A_Pwr < 0) motor_A_Pwr = 0;
    if (motor_B_Pwr > 255) motor_B_Pwr = 255;
    if (motor_B_Pwr < 0) motor_B_Pwr = 0;

    motorControl(MOTOR_A, motor_A_Fwd ? MOTOR_FWD : MOTOR_REV, motor_A_Pwr);
    motorControl(MOTOR_B, motor_B_Fwd ? MOTOR_FWD : MOTOR_REV, motor_B_Pwr);

  }
}
int motorControl
(uint8_t sel, uint8_t dir, int16_t speed)
{
  if (sel == MOTOR_A) {
    analogWrite(MOTOR_A_EN, 0);
    switch (dir) {
      case MOTOR_FWD:
        digitalWrite(MOTOR_A_D1, HIGH);
        digitalWrite(MOTOR_A_D2, LOW);
        break;
      case MOTOR_REV:
        digitalWrite(MOTOR_A_D2, HIGH);
        digitalWrite(MOTOR_A_D1, LOW);
        break;
      case MOTOR_NHI:
        digitalWrite(MOTOR_A_D1, HIGH);
        digitalWrite(MOTOR_A_D2, HIGH);
        break;
      default: //MOTOR_NLO
        digitalWrite(MOTOR_A_D1, LOW);
        digitalWrite(MOTOR_A_D2, LOW);
    }
    analogWrite(MOTOR_A_EN, speed);
  }
  if (sel == MOTOR_B) {
    analogWrite(MOTOR_B_EN, 0);
    switch (dir) {
      case MOTOR_FWD:
        digitalWrite(MOTOR_B_D1, HIGH);
        digitalWrite(MOTOR_B_D2, LOW);
        break;
      case MOTOR_REV:
        digitalWrite(MOTOR_B_D2, HIGH);
        digitalWrite(MOTOR_B_D1, LOW);
        break;
      case MOTOR_NHI:
        digitalWrite(MOTOR_B_D1, HIGH);
        digitalWrite(MOTOR_B_D2, HIGH);
        break;
      default: //MOTOR_NLO
        digitalWrite(MOTOR_B_D1, LOW);
        digitalWrite(MOTOR_B_D2, LOW);
    }
    analogWrite(MOTOR_B_EN, speed);
  }
  return dir;
}
