/* ~Bosse    2023-10-1 */

const int red_led = 9;
const int yellow_led = 10;
const int green_led = 11;
int pause = 100;


void shine(int led_pin) {
  analogWrite(led_pin, 100);
  delay(pause);
  digitalWrite(led_pin, LOW);
  delay(0);
}

void setup() {
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
}

void loop() {
  shine(red_led);
  shine(yellow_led);
  shine(green_led);
  delay(1000);

  //exit(0);
}
