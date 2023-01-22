void setup() {
  // put your setup code here, to run once:
TCCR0B = TCCR0B & B11111000 | B00000101; // for PWM frequency of 61.04 Hz
pinMode(6, OUTPUT);
pinMode(5, OUTPUT);
analogWrite(6, 100);
analogWrite(5, 100);
}

void loop() {
  // put your main code here, to run repeatedly:

}
