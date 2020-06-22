//#include <string.h>
//#include <stdint.h>

unsigned long timer = 0;

typedef struct __attribute__((packed)) {
  float pot;
  uint8_t but0:1;
  uint8_t but1:1;
  uint8_t but2:1;
  uint8_t but3:1;
  uint8_t but4:1;
} input_dev_t;

char str[100] = {0};
#define POTPIN 35
#define BUT0PIN 34

void setup() {
  Serial.begin(115200);
  pinMode(POTPIN, INPUT);
  
  // pull up
  pinMode(BUT0PIN, INPUT);
  digitalWrite(BUT0PIN, HIGH);
  
  timer = micros();
}

void loop() {
  static int cnt = 1;
  cnt ++;
  float pot = cnt;
  uint8_t but0 = (uint8_t) digitalRead(BUT0PIN);
  
  input_dev_t input_dev1 = {.pot = pot, .but0 = but0, .but1 = 1, .but2 = 1, .but3 = 0, .but4 = 1};
  // sprintf(str, "sizeof(input_dev_t): %ld B\n", sizeof(input_dev_t));
  // Serial.println(str);
  
  unsigned char input_dev_str[5];
  memcpy(input_dev_str, &input_dev1, sizeof(input_dev_t));

  Serial.write('$');
  for (int i = 0; i < sizeof(input_dev_t); i++) {
    Serial.write(input_dev_str[i]);
  }
  Serial.write('*');
  
  // for (int i = 0; i < sizeof(input_dev_t); i++) {
  //  sprintf(str, "0x%02x,", input_dev_str[i]);
  //  Serial.print(str);
  // }
  // Serial.print("\n");

  // Backproject
  // input_dev_t input_dev2 = {0};
  // memcpy(&input_dev2, &input_dev_str, sizeof(input_dev_t));
  // sprintf(str, "input_dev2.pot: %f, input_dev2.but0: %d, input_dev2.but3: %d\n", input_dev2.pot, input_dev2.but0, input_dev2.but3);
  // Serial.print(str);
  
  // 7 bytes = 70 bits
  // takes (70,000/115,200) ms to be sent out == 600 microsec. 1ms delay is enough to space-out packets
  delay(1);
}
