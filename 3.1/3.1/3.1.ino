#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Adres wyświetlacza LCD 0x27.

#define CLEAR_LCD_DELAY 500
const int maxVoltage = 5;
const int maxAdcValue = 1024;
char data[32];

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  printTitle();
  lcd.setCursor(0, 1);
  int adcValue = analogRead(A0);
  printVoltages(getVoltages(adcValue));
  lcd.print("  ");
  printAdc(adcValue);
  delay(CLEAR_LCD_DELAY);
}

void printTitle() {
  lcd.setCursor(0, 0);
  lcd.print("Miernik A0");
}

float getVoltages(int adcValue) {
  return float(adcValue) * float(maxVoltage) / float(maxAdcValue);
}

void printVoltages(float voltages) {
  sprintf(data, "V=%.2f", voltages);
  lcd.print(data);
}

void printAdc(int adcValue) {
  lcd.print("ADC=");
  sprintf(data, "%3d", adcValue);
  lcd.print(data);
}
