void setup(void) {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("LFC Project");
}
uint8_t choice;
void loop(void) {
  // put your main code here, to run repeatedly:
if (Serial.available() > 0) {
    choice = Serial.read();
    Serial.println(choice);
}
}
