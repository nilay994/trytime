// units will stay milliseconds..

void setup() {
  // put your setup code here, to run once:

}

void rateloop() {
  static uint16_t count = 0;
  unsigned long curr_time = millis();
  static unsigned long prev_time = curr_time;

  if ((curr_time - prev_time) > 10) {
    count = count + 1;
    Serial.write('$');
    Serial.write(HEX, count);
    Serial.write('*');
    prev_time = curr_time;
  }
  
}

void loop() {
  rateloop();
  // put your main code here, to run repeatedly:

}
