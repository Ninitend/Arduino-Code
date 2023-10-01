/* ~Bosse    2023-10-1 */


// fading led with fancier code


int red_led = 9;
int brightness = 0;
int fade_amount = 3;
int fade_delay = 30;

void setup() {
  pinMode(red_led, OUTPUT);
}


void loop() {
  analogWrite(red_led, brightness);

  brightness = brightness + fade_amount;

  if (brightness <= 0 || brightness >= 100) {
    fade_amount = - fade_amount;
  }

  delay(fade_delay);
}
