// Imię i Nazwisko: Lukasz Gajerski
// Grupa: Czwartek 11:15
// Zadanie: 2.3 sterowanie LED z terminala

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3
#define DEFAULT_BLINK_DELAY 250
const String TURN_ON_LED_CMD = "LED ON";
const String TURN_OFF_LED_CMD = "LED OFF";
const String BLINK_LED_CMD = "LED BLINK";

String userMsg;
int blinkDelay = DEFAULT_BLINK_DELAY ;
int usedLedPin = LED_RED;
bool ledIsBlinking = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  if (ledIsBlinking)
    blinkLed(usedLedPin, blinkDelay);

  if (Serial.available() > 0) {
    userMsg = Serial.readStringUntil('\n');
    checkCommands();
  }
}

void checkCommands() {
  bool foundedCorrectCmd = false;
  checkTurnOnLedCmd(foundedCorrectCmd);
  checkTurnOffLedCmd(foundedCorrectCmd);
  checkLedBlinkCmd(foundedCorrectCmd);
  if (!foundedCorrectCmd)
    Serial.println("Unable to recognize any known command");

}

void checkTurnOnLedCmd(bool& foundedCorrectCmd ) {
  if (userMsg.indexOf(TURN_ON_LED_CMD) != -1) {
    foundedCorrectCmd = true;
    if (commandDontHaveUnnecessarySigns(TURN_ON_LED_CMD)) {
      ledIsBlinking = false;
      digitalWrite(usedLedPin, HIGH);
      Serial.println("LED turned on");
    } else Serial.println("Unknown command. Do you mean: LED ON ?");
  }
}

bool commandDontHaveUnnecessarySigns(const String& command) {
  int nextIndex = userMsg.indexOf(command) + command.length();
  return (userMsg.length() == nextIndex || userMsg.charAt(nextIndex) == 32 || userMsg.charAt(nextIndex) == '\0');
}

void checkTurnOffLedCmd(bool& foundedCorrectCmd ) {
  if (userMsg.indexOf(TURN_OFF_LED_CMD) != -1) {
    foundedCorrectCmd = true;
    if (commandDontHaveUnnecessarySigns(TURN_OFF_LED_CMD)) {
      digitalWrite(usedLedPin, LOW);
      ledIsBlinking = false;
      Serial.println("LED turned off");
    } else Serial.println("Unknown command. Do you mean: LED OFF ?");
  }
}

void checkLedBlinkCmd(bool& foundedCorrectCmd ) {
  int index = userMsg.indexOf(BLINK_LED_CMD);
  if (index != -1) {
    foundedCorrectCmd = true;

    index += (BLINK_LED_CMD.length() + 1);
    char number[userMsg.length() - index + 1];
    int digitsQty = readDigitsFromMsg(index, userMsg, number);

    if (number[0] == '0' && digitsQty == 1) //is only one sign
      blinkDelay = 0;
    else {
      blinkDelay = strtol(number, NULL, 10);
      ledIsBlinking = true;
      if (blinkDelay <= 0)
        blinkDelay = DEFAULT_BLINK_DELAY;
    }

    Serial.print("Blinked LED ");
    Serial.println(blinkDelay);
  }
}

//Return amounts of digits in buffer
int readDigitsFromMsg(int startedIndex, String& msg,  char* digitsBuffer) {
  int offset = 0;
  int index = startedIndex;
  while (index < msg.length() && charIsDigit(msg.charAt(index))) {
    digitsBuffer[offset] = userMsg.charAt(index);
    offset++;
    index++;
  }
  return offset;
}

bool charIsDigit(char sign) {
  return (sign >= 48 && sign <= 57);
}

void blinkLed(int ledPin, int blinkDelayTime) {
  digitalWrite(ledPin, HIGH);
  delay(blinkDelayTime);
  digitalWrite(ledPin, LOW);
  delay(blinkDelayTime);
}
