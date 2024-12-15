
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define muteButton 3
#define buzzer 4
bool muteState = false;
unsigned long mutePushed = 0;
const float maxTemp = 90.0;
float temp = 0;
const unsigned long MuteTime = 36E5;
// Zmienna do przechowywania czasu ostatniego odbioru poprawnej wartości
unsigned long lastReceivedTime = 0;

// Czas opóźnienia (1 minuta = 60000 ms)
const unsigned long waitTime = 6E4;
// Inicjalizacja wyświetlacza LCD: Adres I2C (zazwyczaj 0x27 lub 0x3F), kolumny i wiersze
LiquidCrystal_I2C lcd(0x27, 16, 2);
void alert(bool state) {
  switch (state) {
    case 1:

      unsigned char i, j;  // define variables

      for (i = 0; i < 80; i++)  // Wen a frequency sound
      {
        digitalWrite(buzzer, HIGH);  // send voice
        delay(1);                    // Delay 1ms
        digitalWrite(buzzer, LOW);   // do not send voice
        delay(1);                    // delay ms
      }
      for (i = 0; i < 100; i++)  // Wen Qie out another frequency sound
      {
        digitalWrite(buzzer, HIGH);  // send voice
        delay(2);                    // delay 2ms
        digitalWrite(buzzer, LOW);   // do not send voice
        delay(2);                    // delay 2ms
      }
      delay(1E3);
      break;
    case 0:
      digitalWrite(buzzer, LOW);
      break;
  }
}
void buttonISR() {
  muteState = true;
  mutePushed = millis();
}
void (*resetFunc)(void) = 0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(muteButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(muteButton), buttonISR, RISING);
  Serial.begin(9600);
  lcd.init();              // Inicjalizacja wyświetlacza LCD
  lcd.backlight();         // Włączenie podświetlenia
  lcd.clear();             // Wyczyszczenie wyświetlacza
  lcd.setCursor(0, 0);     // Ustawienie kursora w lewym górnym rogu
  lcd.print("Czekam...");  // Wiadomość początkowa
}

void loop() {
  //if (Serial.available()) {
    String receivedText = Serial.readString();  // Odbieranie tekstu przez UART
    // Szukanie "Temp" w odebranym łańcuchu
    int tempIndex = receivedText.indexOf("Temp:");

    if (tempIndex != -1) {  // Jeśli "Temp" zostało znalezione
      // Pobierz wartość po "Temp"
      digitalWrite(LED_BUILTIN, HIGH);
      String tempValue = receivedText.substring(tempIndex + 5);
      tempValue.trim();
      temp = tempValue.toFloat();
      // Wyczyszczenie LCD i wyświetlenie temperatury
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      lcd.setCursor(0, 1);
      char formattedTemp[10];
      dtostrf(temp, 4, 1, formattedTemp);
      lcd.print(formattedTemp);
      lcd.print("\337C");  // Wyświetl wartość z formatowaniem
      lastReceivedTime = millis();
      digitalWrite(LED_BUILTIN, HIGH);
      
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      if (millis() - lastReceivedTime > waitTime) {
        // Jeśli brak "Temp", wyświetl "Czekam..."
        digitalWrite(LED_BUILTIN, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Czekam...");
      }
    }
      if (temp >= maxTemp && muteState == false) {

        //Serial.println("Alarm! Max temp!");
        alert(1);
        //readDataToSend();
        //esp_now_send(NULL, (uint8_t *) &outgoingSetpoints, sizeof(outgoingSetpoints));
      } else if (temp >= maxTemp && muteState == true) {
        if ((millis() - mutePushed) <= MuteTime) {

          alert(0);
        } else {
          muteState = false;
        }
      } else {
        muteState = false;
        alert(0);
      }
    
 // }
}
