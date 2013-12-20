#include <LiquidCrystal.h>

#define POTENCIOMETER_PIN A5
#define START_BTN 8
#define STOP_BTN 9
#define OUT_PIN1 10
#define BEEPER_PIN A0
#define LCD_RW 2
#define LCD_E 3
#define LCD_D1 4
#define LCD_D2 5
#define LCD_D3 6
#define LCD_D4 7


LiquidCrystal lcd(LCD_RW, LCD_E, LCD_D1, LCD_D2, LCD_D3, LCD_D4);

void setup() {
  pinMode(POTENCIOMETER_PIN, INPUT);
  pinMode(OUT_PIN1, OUTPUT);
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.print("Exposure time:");
}

void loop() {
  int pot = analogRead(POTENCIOMETER_PIN);
  lcd.setCursor(0, 1);
  ShowTime(pot);
  delay(100);
  
  if (digitalRead(START_BTN) == LOW) StartExposure(pot);  
}

void StartExposure(int sek)
{
  lcd.clear();
  lcd.print("EXPOSING!"); 
  lcd.setCursor(0, 1);
  lcd.print("Remaining");
  digitalWrite(OUT_PIN1, HIGH);  
  
  for (int i = 0; i < sek; i++)
  {
    int timeLeft = sek - i;
    lcd.setCursor(10, 0);
    lcd.print((int)(((double)i / (double)sek) * 100.0));
    lcd.print("%");
    lcd.setCursor(10, 1);
    ShowTime(timeLeft);
    for (int t = 0; t < 10; t++)
    {
      if (digitalRead(STOP_BTN) == LOW)
      {
        digitalWrite(OUT_PIN1, LOW);
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Exposure time:");
        return;
      }
      delay(100);
    }
  }

  digitalWrite(OUT_PIN1, LOW);  
  
  lcd.clear();
  lcd.print("DONE!");
  
  // play 3x beep at 2000 Hz
  for (int x = 0; x < 3; x++)
  {
    tone(BEEPER_PIN, 2000);
    delay(400);
    noTone(BEEPER_PIN);
    delay(100);
  }
  
  delay(3000);
  
  lcd.clear();
  lcd.print("Exposure time:");
}

void ShowTime(int sek)
{
  char strOut[3];
  formatTimeDigits(strOut, sek / 60);
  lcd.print(strOut);
  lcd.print(":");
  formatTimeDigits(strOut, sek % 60);
  lcd.print(strOut);  
}

void formatTimeDigits(char strOut[3], int num)
{
  strOut[0] = '0' + (num / 10);
  strOut[1] = '0' + (num % 10);
  strOut[2] = '\0';
}
