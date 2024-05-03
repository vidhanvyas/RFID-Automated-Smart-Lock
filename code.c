
  
// Include necessary libraries for RFID, SPI, pitches, and LCD
#include <RFID.h>                 
#include <SPI.h>
#include "pitches.h"             
#include <LiquidCrystal_I2C.h>    
 
// Define pins for RFID, Relay, LEDs, and Buzzer
#define SS_PIN 10
#define RST_PIN 9
#define RELAY_PIN 6
#define Red_LED 2
#define Green_LED 3
#define Buzzer 4
 
// Initialize LCD with I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// Initialize RFID with SS and RST pins
RFID rfid(SS_PIN, RST_PIN);
 
// Define authorized RFID tag
int My_RFID_Tag[5] = {0xDE, 0xA1, 0xF0, 0x1D, 0x92};
boolean My_Card = false;
 
// Define melodies and note durations for buzzer
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};
int welcomeMelody[] = {NOTE_C4, NOTE_D4, NOTE_E4};
int welcomeDurations[] = {4, 4, 4};
 
void setup()
{
  // Set pin modes for LEDs, Buzzer, and Relay
  pinMode(Red_LED, OUTPUT);
  pinMode(Green_LED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Set relay to normally closed state
 
  // Initialize serial communication for debugging and LCD for display
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
 
  // Initialize SPI bus and RFID module
  SPI.begin();
  rfid.init();
}
 
void loop()
{
  // Assume card is authorized initially
  My_Card = true;
  digitalWrite(RELAY_PIN, HIGH);  // Lock the door by default
  lcd.clear();
  lcd.print(" Put Your Card  ");
 
  // Check for the presence of an RFID card and read its serial number
  if (rfid.isCard() && rfid.readCardSerial())
  {
    playWelcomeMelody();  // Play a welcoming tune
    displayCardInfo();    // Display card's unique ID
    checkCardAuthorization();  // Check if the card is authorized
  }
  rfid.halt();  // Stop reading the card
}
 
// Function to display the RFID card's unique ID on LCD and Serial monitor
void displayCardInfo()
{
  lcd.clear();
  lcd.print("UNIQUE ID is:- ");
  delay(500);
  lcd.setCursor(0, 1);
  for (int i = 0; i < 5; i++)
  {
    Serial.print(rfid.serNum[i], HEX);
    Serial.print(" ");
    lcd.print(rfid.serNum[i], HEX);
    lcd.print(" ");
  }
  delay(500);
}
 
// Function to check if the read RFID card is authorized
void checkCardAuthorization()
{
  for (int i = 0; i < 5; i++)
  {
    if (My_RFID_Tag[i] != rfid.serNum[i])
    {
      My_Card = false;
      break;
    }
  }
  Serial.println();
  delay(1000);
  My_Card ? grantAccess() : denyAccess();  // Grant or deny access based on card authorization
}
 
// Function to grant access if the card is authorized
void grantAccess()
{
  Serial.println("\nWelcome To Your Room");
  lcd.clear();
  lcd.print("Welcome to Your");
  lcd.setCursor(0, 1);
  lcd.print("Room!");
  delay(500);
  digitalWrite(Green_LED, HIGH);  // Turn on green LED
  delay(1000);
  digitalWrite(Green_LED, LOW);
  delay(1000);
  digitalWrite(Red_LED, HIGH);
  delay(1000);
  digitalWrite(Red_LED, LOW);
  playMelody();  // Play a melody
  openDoor();  // Unlock the door
}
 
// Function to play a melody when access is granted
void playMelody()
{
  for (int i = 0; i < 2; i++)
  {
    for (int thisNote = 0; thisNote < 12; thisNote++)
    {
      int noteDuration = 1000 / noteDurations[thisNote];
      //tone(Buzzer, melody[thisNote], noteDuration);
     // delay(noteDuration * 1.30);
     // noTone(Buzzer);
    }
    delay(500);
  }
}
 
// Function to unlock the door and start a countdown before locking it again
void openDoor()
{
  digitalWrite(RELAY_PIN, LOW);  // Unlock the door
  delay(200);
  lcd.clear();
  for (int i = 10; i > 0; i--)
  {
    lcd.print("Door will close");
    lcd.setCursor(0, 1);
    lcd.print("in ");
    lcd.print(i);
    lcd.print(" Sec HurryUp!");
    delay(100);
    lcd.clear();
  }
  digitalWrite(RELAY_PIN, HIGH);  // Lock the door again
  digitalWrite(Green_LED, LOW);  // Turn off green LED
  delay(200);
  lcd.clear();
  lcd.print("Door is Close");
  lcd.setCursor(0, 1);
  lcd.print("Now!");
  delay(100);
}
 
// Function to deny access if the card is unauthorized
void denyAccess()
{
  Serial.println("\nGet Out of Here !");
  lcd.clear();
  lcd.print("Card NOT FOUND!");
  lcd.setCursor(0, 1);
  lcd.print("Get Out of Here!");
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(Buzzer, HIGH);  // Sound the buzzer
    //digitalWrite(Red_LED, HIGH);  // Turn on red LED
    delay(500);
    digitalWrite(Buzzer, LOW);  // Turn off buzzer
    //digitalWrite(Red_LED, LOW);  // Turn off red LED
    delay(500);
  }
  delay(100);
}
 
// Function to play a welcome melody while the card is being identified
void playWelcomeMelody()
{
  {
   
   // tone(Buzzer, welcomeMelody[i], noteDuration);
    //delay(noteDuration * 1.30);
    //noTone(Buzzer);
  }
}