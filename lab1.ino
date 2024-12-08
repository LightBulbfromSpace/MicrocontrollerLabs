int redLEDPin = 6;
int blueLEDPin = 4;
int buttonPin = 2;

bool isLEDsTurned = true;

void setup() {
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  attachInterrupt(
    digitalPinToInterrupt(buttonPin),
    changeState,
    FALLING
  );

}

void loop() {
  if (isLEDsTurned) {
    blink(redLEDPin);
    blink(blueLEDPin);
  }
}

void blink(int LEDPin) {
  digitalWrite(LEDPin, HIGH);
  delay(500);
  digitalWrite(LEDPin, LOW);
}

void changeState() {
  isLEDsTurned = !isLEDsTurned;
}
