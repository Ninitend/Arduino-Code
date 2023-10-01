/* ~Bosse    2023-10-1 */

const int sensorMin = 0;
const int sensorMax = 1023;
const int green_led = 10;


void setup() {
  Serial.begin(9600);
  pinMode(green_led, OUTPUT);
}



void loop() {
  int sensor_reading = analogRead(A0);
  int brightness = map(sensor_reading, sensorMin, sensorMax, 255, 0);
  Serial.println(brightness);
  analogWrite(green_led, brightness);
}
