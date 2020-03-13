#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define CLEAR_LCD_DELAY 500
#define BUFFOR_SIZE 10
#define RED_BUTTON_PIN 2
#define GREEN_BUTTON_PIN 4
#define DELAY_FOR_BTN_BOUNCING 20
const int maxTextLength = 16;
String buffor[BUFFOR_SIZE];
int nextBufforOffset = 0;
int actualPresentedBufforOffset = nextBufforOffset;
bool lastRedBtnState = HIGH;
bool lastGreenBtnState = HIGH;
int actualFirstLineOffset = 0;
int actualSecondLineOffset = 0;
bool movingInLeftSide = true;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  readUserInput();
  checkButtonsActions();
  printBufforToScreen();
  delay(CLEAR_LCD_DELAY);
}

void checkButtonsActions() {

  if (buttonWasClicked(GREEN_BUTTON_PIN, lastGreenBtnState) ) {
    lcd.clear();
    actualPresentedBufforOffset++;
    if (actualPresentedBufforOffset > nextBufforOffset)
      actualPresentedBufforOffset = nextBufforOffset;
    actualFirstLineOffset = 0;
    actualSecondLineOffset = 0;
  }
  if (buttonWasClicked(RED_BUTTON_PIN, lastRedBtnState)) {
    lcd.clear();
    actualPresentedBufforOffset--;
    if (actualPresentedBufforOffset < 1)
      actualPresentedBufforOffset = 1;
    actualFirstLineOffset = 0;
    actualSecondLineOffset = 0;
  }

}

void readUserInput() {
  if (Serial.available() > 0) {
    String userMsg = Serial.readStringUntil('\n');
    addToBuffor(userMsg);
    lcd.clear();
  }
}

void addToBuffor(String userMsg) {
  if (nextBufforOffset < BUFFOR_SIZE) {
    buffor[nextBufforOffset] = userMsg;
    nextBufforOffset++;
  } else {
    for (int i = 1; i < BUFFOR_SIZE; i++)
      buffor[i - 1] = buffor[i];
    buffor[BUFFOR_SIZE - 1] = userMsg;
  }
  actualPresentedBufforOffset = nextBufforOffset;
  actualFirstLineOffset = actualSecondLineOffset;
  actualSecondLineOffset = 0;
}

void printBufforToScreen() {
  lcd.clear();
  if (actualPresentedBufforOffset > 0) {
    lcd.setCursor(0, 1);
    String textToPrint = buffor[actualPresentedBufforOffset - 1];
    int textLength = textToPrint.length();
    if (textLength > maxTextLength)
      printTextWithMovingEffect(textToPrint, actualFirstLineOffset, movingInLeftSide);
    else lcd.print(textToPrint);
  }

  if (actualPresentedBufforOffset > 1) {
    lcd.setCursor(0, 0);
    String textToPrint = buffor[actualPresentedBufforOffset - 2];
    int textLength = textToPrint.length();
    if (textLength > maxTextLength)
      printTextWithMovingEffect(textToPrint, actualSecondLineOffset, movingInLeftSide);
    else lcd.print(textToPrint);
  }
}

void printTextWithMovingEffect(String& text, int& lineOffset, bool inLeftSide) {
  int textLength = text.length();
  if (lineOffset < textLength - 1 && lineOffset >= 0) {
    int charsToDisplayQty = textLength - lineOffset;
    if (charsToDisplayQty > maxTextLength)
      charsToDisplayQty = maxTextLength;

    Serial.println("offset+charsQty");
    Serial.println(lineOffset);
    Serial.println(charsToDisplayQty);


    if (inLeftSide) {
      int tmpOffset = lineOffset + charsToDisplayQty;
      lcd.print(text.substring(lineOffset, tmpOffset));
      lineOffset++;
    }
    else {
      int tmpOffset = lineOffset - charsToDisplayQty;
      lcd.print(text.substring(tmpOffset, lineOffset));
      lineOffset--;
    }
  } else if (movingInLeftSide) {
    movingInLeftSide = false;
    lineOffset--;
  } else {
    movingInLeftSide = true;
    lineOffset++;
  }

}

bool buttonWasClicked(int buttonPin, bool & lastBtnState) {
  bool currentBtnState = digitalRead(buttonPin);
  if (lastBtnState != currentBtnState) {
    lastBtnState = currentBtnState;
    if (currentBtnState == LOW) {
      delay(DELAY_FOR_BTN_BOUNCING);
      return (digitalRead(buttonPin) == LOW);
    }
  }
  return false;
}
