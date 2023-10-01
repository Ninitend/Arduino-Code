/* ~Bosse    2023-10-1 */


const int button = 2;
const int red_led = 9;
const int yellow_led = 10;
const int green_led = 11;
int button_state;

void setup() {
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(9600);
}



void loop() {
  button_state = digitalRead(button);

  if (button_state == 0) {
    digitalWrite(green_led, HIGH);
    digitalWrite(red_led, LOW);
  }

  if (button_state == 1) {
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
  }


  Serial.println(button_state);
}
