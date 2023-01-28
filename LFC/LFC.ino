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
constexpr uint8_t MOTOR_B_EN = 5;
constexpr uint8_t MOTOR_A_D1 = 3;
constexpr uint8_t MOTOR_A_D2 = 7;
constexpr uint8_t MOTOR_B_D1 = 8;
constexpr uint8_t MOTOR_B_D2 = 9;

//constexpr uint8_t SENSE_R_D =10;
//constexpr uint8_t SENSE_L_D =4;

constexpr uint8_t SENSE_L_A = A0;
constexpr uint8_t SENSE_M_A = A1;
constexpr uint8_t SENSE_R_A = A2;

//I am planning on making this a strut or something; unless you are satisfied with what you see
constexpr uint8_t MOTOR_A = 0b00010100;
constexpr uint8_t MOTOR_B = 0b00001000;

int16_t senseLInput;
int16_t senseMInput;
int16_t senseRInput;

int16_t senseLCal;
int16_t senseBCal;

int16_t motor_A_Pwr;
int16_t motor_B_Pwr;
//bool motor_A_Fwd;
//bool motor_B_Fwd;
//int choice;
constexpr uint8_t InitPower = 50;

int motorControl(uint8_t, int16_t);

void setup
(void)
{

  TCCR0B = TCCR0B & B11111000 | B00000101; // for PWM frequency of 61.04 Hz

  //Serial.begin(9600); //for serial "debug"
  //configure pin direction
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);
  pinMode(MOTOR_A_D1, OUTPUT);
  pinMode(MOTOR_A_D2, OUTPUT);
  pinMode(MOTOR_B_D1, OUTPUT);
  pinMode(MOTOR_B_D2, OUTPUT);
  pinMode(SENSE_L_A, INPUT);
  pinMode(SENSE_M_A, INPUT);
  pinMode(SENSE_R_A, INPUT);
  pinMode(13, OUTPUT);

  //turn motor pin off
  analogWrite(MOTOR_A_EN, 0);
  analogWrite(MOTOR_B_EN, 0);
  digitalWrite(MOTOR_A_D1, LOW);
  digitalWrite(MOTOR_A_D2, LOW);
  digitalWrite(MOTOR_B_D1, LOW);
  digitalWrite(MOTOR_B_D2, LOW);
  digitalWrite(13, HIGH);
  delay(3000);
  digitalWrite(13, LOW);
  delay(1000);
  senseBCal = analogRead(SENSE_L_A);
  senseLCal = analogRead(SENSE_M_A);
  senseBCal += analogRead(SENSE_R_A);
  digitalWrite(13, HIGH);
  senseBCal /= 2;
  motor_A_Pwr = InitPower;
  motor_B_Pwr = InitPower;
  //motor_A_Fwd = true;
  //motor_B_Fwd = true;
  //choice = 0;
  //while (!Serial);
  //Serial.println("LFC Project");
}

#define ADJ_MARGIN 100

void loop
(void)
{


  delay(100);//crude loop time control
  senseLInput = analogRead(SENSE_L_A);
  senseMInput = analogRead(SENSE_M_A);
  senseRInput = analogRead(SENSE_R_A);

  //sensor should read high when not detecting the line


  //if center sensor detect color lighter than line
  if (senseMInput > (senseLCal + ADJ_MARGIN)) {
    //car is not on line
    //left sensor detect color darker than background
    if (senseLInput < (senseBCal - ADJ_MARGIN)) {
      motor_B_Pwr += 10;
      motor_A_Pwr -= 10;
    }
    //right sensor detect color darker than background
    if (senseRInput < (senseBCal - ADJ_MARGIN)) {
      motor_B_Pwr += 10;
      motor_A_Pwr -= 10;
    }
  }

  //
  // KEYBOARD OVERRIDE CONTROL
  //

  //Q: MOTOR A FORWARD    A: MOTOR A REVERSE
  //W: MOTOR A FAST       S: MOTOR A SLOW
  //E: MOTOR B FORWARD    D: MOTOR B REVERSE
  //R: MOTOR B FAST       F: MOTOR B SLOW

  /*
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
  */
  if (motor_A_Pwr > 255) motor_A_Pwr = 255;
  if (motor_A_Pwr < -255) motor_A_Pwr = -255;
  if (motor_B_Pwr > 255) motor_B_Pwr = 255;
  if (motor_B_Pwr < -255) motor_B_Pwr = -255;

  motorControl(MOTOR_A, motor_A_Pwr);
  motorControl(MOTOR_B, motor_B_Pwr);

}

int motorControl
(uint8_t sel, int16_t M_speed)
{
  if (sel == MOTOR_A) {
    analogWrite(MOTOR_A_EN, 0);
    if (M_speed > 0) {

      digitalWrite(MOTOR_A_D1, HIGH);
      digitalWrite(MOTOR_A_D2, LOW);
    }
    if (M_speed < 0) {
      digitalWrite(MOTOR_A_D2, HIGH);
      digitalWrite(MOTOR_A_D1, LOW);
    }
    analogWrite(MOTOR_A_EN, M_speed);
  }
  if (sel == MOTOR_B) {
    analogWrite(MOTOR_B_EN, 0);
    if (M_speed > 0) {
      digitalWrite(MOTOR_A_D1, HIGH);
      digitalWrite(MOTOR_A_D2, LOW);
    }
    if (M_speed < 0) {
      digitalWrite(MOTOR_A_D2, HIGH);
      digitalWrite(MOTOR_A_D1, LOW);
    }
    analogWrite(MOTOR_B_EN, M_speed);
  }
  return sel;
}
