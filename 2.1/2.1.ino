// Imię i Nazwisko: Lukasz Gajerski
// Grupa: Czwartek 11:15
// Zadanie: 2.1 Licznik na dwa przyciski

#define RED_BUTTON_PIN 2
#define GREEN_BUTTON_PIN 4
#define DELAY_FOR_BTN_BOUNCING 50
#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3
#define BLINK_DELAY 250

bool lastRedBtnState = HIGH;
bool lastGreenBtnState = HIGH;
int count = 0;
int ledBlinkCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  printCounterStatus();
}

void loop() {
  if (needToBlinkLed())
    blinkLed(LED_RED, BLINK_DELAY);
  else if (buttonIsPressed(RED_BUTTON_PIN, lastRedBtnState)) {
    doRedButtonPressedAction();
    if (buttonIsPressed(GREEN_BUTTON_PIN, lastGreenBtnState)) {
      doGreenButtonPressedAction();
      doBothButtonsPressedAction();
    }
  }
  else if (buttonIsPressed(GREEN_BUTTON_PIN, lastGreenBtnState)) {
    doGreenButtonPressedAction();
    if (buttonIsPressed(RED_BUTTON_PIN, lastRedBtnState)) {
      doRedButtonPressedAction();
      doBothButtonsPressedAction();
    }
  }
}

bool needToBlinkLed() {
  return ledBlinkCount > 0;
}

void blinkLed(int ledPin, int blinkDelayTime) {
  digitalWrite(ledPin, HIGH);
  delay(blinkDelayTime);
  digitalWrite(ledPin, LOW);
  delay(blinkDelayTime);
  ledBlinkCount--;
}

bool buttonIsPressed(int buttonPin, bool & lastBtnState) {
  bool currentBtnState = digitalRead(buttonPin);

  if (lastBtnState != currentBtnState) {  //is slope running or falling
    lastBtnState = currentBtnState;

    if (currentBtnState == LOW) {  //is pressed
      delay(DELAY_FOR_BTN_BOUNCING);
      return (digitalRead(buttonPin) == LOW);   //is still pressed
    }
  }
  return false;
}

void doRedButtonPressedAction() {
  if (count > 0) {
    count--;
    printCounterStatus();
  }
}
void doGreenButtonPressedAction() {
  count++;
  printCounterStatus();
}

void printCounterStatus() {
  Serial.print("Counter = ");
  Serial.println(count);
}

void doBothButtonsPressedAction() {
  ledBlinkCount = count;
  Serial.println("LED is blinking");
}
