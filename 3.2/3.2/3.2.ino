#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Adres wyświetlacza LCD 0x27.

#define CLEAR_LCD_DELAY 500 //czestotliwosc odswiezania wyswietlacza
#define RED_BUTTON_PIN 2
const int maxVoltage = 5;
const int maxAdcValue = 1024;
char data[32];
bool isDisabled = false;
bool lastRedBtnState = HIGH;
bool lcdIsDisabled = false;
//opoznienie dla przycisku celem poprawnego wylapywania klikniec
#define DELAY_FOR_BTN_BOUNCING 20

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (buttonWasClicked(RED_BUTTON_PIN, lastRedBtnState))
    changeLcdBacklight() ; //walaczenie / wylaczenie wyswietlacza
  printResult();
  delay(CLEAR_LCD_DELAY);
}

bool buttonWasClicked(int buttonPin, bool & lastBtnState) {
  bool currentBtnState = digitalRead(buttonPin);

  if (lastBtnState != currentBtnState) {  //zbocze opadajace lub rosnace
    lastBtnState = currentBtnState;

    if (currentBtnState == LOW) {  //jest nacisniety
      delay(DELAY_FOR_BTN_BOUNCING);
      return (digitalRead(buttonPin) == LOW);   //jest nadal nacisniety
    }
  }
  return false;
}

//zmiana podwietlenie wyswietlacza
void changeLcdBacklight() {
  if (lcdIsDisabled) {
    lcdIsDisabled = false;
    lcd.noBacklight();
  } else {
    lcd.backlight();
    lcdIsDisabled = true;
  }
}

void printResult() {
  lcd.clear();
  lcd.setCursor(0, 0);
  printTitle();
  lcd.setCursor(0, 1);
  int adcValue = analogRead(A0);
  float voltages = getVoltages(adcValue);
  printVoltages(voltages);
  printResultForPlotterOnSerial(voltages);
  lcd.print("  ");
  printAdc(adcValue);
}

void printTitle() {
  lcd.setCursor(0, 0);
  lcd.print("Miernik A0");
}

//przeliczenie wartosci adc na volty
float getVoltages(int adcValue) {
  return float(adcValue) * float(maxVoltage) / float(maxAdcValue);
}

void printVoltages(float voltages) {
  sprintf(data, "V=%.2f", voltages); //formatowanie do 2 miejsc po przecinku
  lcd.print(data);
}

//wyswietlanie woltow i stanu podswietlenia na UART celem pokazania wykresu w plotterze
void printResultForPlotterOnSerial(float voltages){
  Serial.print(voltages);
  Serial.print("\t"); 
  Serial.print(3*(int)(lcdIsDisabled));
  Serial.println("");
}

void printAdc(int adcValue) {
  lcd.print("ADC=");
  sprintf(data, "%3d", adcValue); //formatowanie do 3 cyfr (gdy liczba mniejsza to dopelniana spacjami z lewej)
  lcd.print(data);
}
