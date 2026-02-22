#define BLYNK_TEMPLATE_ID "TMPL3QgOrrGAJ"
#define BLYNK_TEMPLATE_NAME "Crowd Control System"
#define BLYNK_AUTH_TOKEN "RyGceXP7q1HWGRjiUfSvNELtrnfEOFut"

/*#define BLYNK_TEMPLATE_ID "TMPL3kYLuN_xv"
#define BLYNK_TEMPLATE_NAME "Smart Crowd Control System"
#define BLYNK_AUTH_TOKEN "r2h2ufNXXmu3Kv--ku3gEVv7I5kA5_Fi"*/

// Library includes
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// ---------------- Pin Definitions ----------------
#define ENTRY_SENSOR D5    // IR sensor at entry door
#define EXIT_SENSOR  D6    // IR sensor at exit door
#define GREEN_LED    D7
#define RED_LED      D8
#define BUZZER       D4
#define RESET_BUTTON D3    // Push button for reset

// ---------------- Variables ----------------
int currentCount = 0;
int maxSeats = 10;  // Change as per requirement

char auth[] = BLYNK_AUTH_TOKEN;  // From Blynk App
char ssid[] = "DESKTOP-E43F4J7 2439";    // WiFi SSID
char pass[] = "4W5d53;8"; // WiFi Password

// LCD setup (I2C address 0x27, 16x2 display)
//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Pin modes
  pinMode(ENTRY_SENSOR, INPUT_PULLUP);
  pinMode(EXIT_SENSOR, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  // LCD init
 /* lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Crowd Control");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");*/

  // Serial
  Serial.begin(115200);
  //Serial.print("esp8266 is working");

  // Blynk & WiFi
  Blynk.begin(auth, ssid, pass);

  // Default states
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  Blynk.run(); // Run Blynk

  // Entry sensor
  if (digitalRead(ENTRY_SENSOR) == LOW) {
    currentCount++;
    Serial.print("IN -> Count: ");
    Serial.println(currentCount);
    Blynk.virtualWrite(V0, currentCount); // Send to Blynk App
    //updateLCD();
    delay(500); // Debounce
  }

  // Exit sensor
  if (digitalRead(EXIT_SENSOR) == LOW) {
    if (currentCount > 0) currentCount--;
    Serial.print("OUT -> Count: ");
    Serial.println(currentCount);
    Blynk.virtualWrite(V0, currentCount);
    //updateLCD();
    delay(500);
  }

  // Reset button
  if (digitalRead(RESET_BUTTON) == LOW) {
    currentCount = 0;
    Serial.println("Count Reset!");
    Blynk.virtualWrite(V0, currentCount);
    //updateLCD();
    delay(500);
  }

  // Control LEDs and buzzer
  if (currentCount >= maxSeats) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    Blynk.virtualWrite(V2, "FULL"); // Send status
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V1, "AVAILABLE");
  }
  delay(1000);
}

// ---------------- Functions ----------------
/*void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Count: ");
  lcd.print(currentCount);
  lcd.setCursor(0, 1);
  if (currentCount >= maxSeats) {
    lcd.print("Status: FULL");
  } else {
    lcd.print("Seats OK");
  }
}*/
