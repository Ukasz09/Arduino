// Imię i Nazwisko: Lukasz Gajerski
// Grupa: Czwartek 11:15
// Zadanie: 2.2 czas wcisniecia przycisku

#define RED_BUTTON_PIN 2
#define DELAY_FOR_BTN_BOUNCING 20

bool lastRedBtnState = HIGH;
bool timerIsActive = false;
unsigned long startedTime;

void setup() {
  Serial.begin(9600);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (buttonWasClicked(RED_BUTTON_PIN, lastRedBtnState)) {
    timerIsActive = true;
    startedTime = millis();
  }
  if (timerIsActive) {
    if (!buttonIsPressed(RED_BUTTON_PIN)) {
      printResult(millis() - startedTime);
      timerIsActive = false;
    }
  }

}

bool buttonWasClicked(int buttonPin, bool & lastBtnState) {
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

bool buttonIsPressed(int buttonPin) {
  return digitalRead(buttonPin) == LOW;
}

void printResult(unsigned long millisTime) {
  Serial.print("Button pressed by: ");
  char str[16];
  makeTime(millisTime, str);
  Serial.println(str);
}

void makeTime(unsigned long millisTime, char* buff) {
  int sec = millisTime / 1000;
  int millisSec = millisTime - sec * 1000;
  int minutes = sec / 60;
  int hours = minutes / 60;
  makeTime(buff, hours, minutes, sec, millisSec);
}

void makeTime(char* buff, int hours, int minutes, int sec, int milisec) {
  buff[0] = '\0';
  toCharWithTimeFormat(hours, buff);
  strcat(buff, ":");
  toCharWithTimeFormat(minutes, buff);
  strcat(buff, ":");
  toCharWithTimeFormat(sec, buff);
  strcat(buff, ".");
  if (milisec < 100)
    strcat(buff, "0");
  toCharWithTimeFormat(milisec, buff);
}

void toCharWithTimeFormat(int number, char* buff) {
  char temp[4];
  itoa(number, temp, 10);
  if (number < 10)
    strcat(buff, "0");
  strcat(buff, temp);
}
