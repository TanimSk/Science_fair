#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

Servo myservo1;//temp hand 11
Servo myservo2;//max3102 hand 10
Servo myservo3; //medicine hand 9
Servo myservo4; // glass hand 3

//Heart rate
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
float hr = 0.0;


///////////  SPO2 Sensor  ///////////
#define MAX_BRIGHTNESS 255
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte pulseLED = 11; //Must be on PWM pin
byte readLED = 13; //Blinks with each data read




//temperature A0 pin
#define RT0 10000   // Ω
#define B 3977      // K
#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ
float RT, VR, ln, T0, Read;


//keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 8, 7};
byte colPins[COLS] = {4, 2, A1, A2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);



LiquidCrystal_I2C lcd(0x27, 16, 2);
MAX30105 particleSensor;



//Sonar
#define echoPin 6
#define trigPin 5



String Phone_number = "";
String Age = "";
float Temp = 0.0;
bool has_cough = false;
float lens_power = 0.25;



float duration;
float distance;


char key;
String eye_check = "4854";
String seen_value = "";



void setup() {
  Setup_pins();
  myservo1.attach(11);
  myservo2.attach(10);
  myservo3.attach(9);
  myservo4.attach(3);

  myservo1.write(0);
  myservo2.write(55);
  myservo3.write(90);
  myservo4.write(0);
  delay(500);

  while (true) {
    key = customKeypad.getKey();
    if (key == '1') {
      lcd.begin();
      lcd.backlight();
      
      lcd.setCursor(4, 0);
      lcd.print("Started");
      lcd.setCursor(3, 1);
      lcd.print("Medi Robo");
      break;
    }
  }
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  if (distance < 65 && distance > 0) {
    // reset value
    has_cough = false;
    hr = 0.0;
    Temp = 0.0;
    Phone_number = "";
    Age = "";
    lens_power = 0.25;
    seen_value = "";
    

    play_audio("1.mp3"); //welcome audio
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Welcome!");
    delay(5000);
    lcd.clear();

    //////////////// PHONE NUMBER ////////////////
    lcd.print("Phone Number :  ");
    lcd.setCursor(1, 1);
    myservo1.write(0);
    myservo2.write(55);
    myservo3.write(90);
    myservo4.write(0);
    delay(100);

    while (true) {
      key = customKeypad.getKey();
      if (key) {
        lcd.print(key);
        Phone_number += key;
        if (Phone_number.length() == 11) {
          break;
        }
      }
    }
    /////////////-------//////////////


    ////////////////// AGE //////////////////
    lcd.clear();
    lcd.print("Your Age :     #");
    lcd.setCursor(1, 1);
    while (true) {
      key = customKeypad.getKey();
      if (key) {
        if (key == '#') {
          break;
        }
        lcd.print(key);
        Age += key;
      }
    }
    ////////////////// -- //////////////////


    lcd.clear();
    play_audio("2.mp3");  //ask for service and give option
    delay(10000);
    check_up();
    delay(5000);
  }
  delay(5);
}

void check_up() {
  lcd.clear();
  lcd.print("Enter an Option:");
  lcd.setCursor(7, 1);

  while (true) {
    key = customKeypad.getKey();
    if (key) {
      lcd.print(key);
      delay(1000);
      break;
    }
  }

  lcd.clear();
  if (key == 'A') {
    play_audio("3.mp3"); // fever -- put your hand here
    lcd.setCursor(1, 0);
    lcd.print("Fever Checkup");
    lcd.setCursor(2, 1);
    lcd.print("Please wait");
    delay(5000);
    myservo1.write(55);
    delay(500);
    for (int i = 0; i < 50; i++) {
      Read = analogRead(A0);                //analog value Read
      Read = (5.00 / 1023.00) * Read;      //Conversion to voltage
      VR = VCC - Read;
      RT = Read / (VR / R);               //Resistance of RT

      ln = log(RT / RT0);
      Temp = (1 / ((ln / B) + (1 / T0))); //Temperature from sensor

      Temp = ((Temp - 273.15) * 1.8) + 41; //Conversion to Farenheit   32
      //      Serial.println(Temp);
      delay(150);
    }
    lcd.clear();
    lcd.print("Body Temperature");
    lcd.setCursor(5, 1);
    lcd.print(Temp);
    if (Temp > 100) {
      myservo3.write(25);
      delay(500);
      myservo3.write(90);
      delay(500);
      play_audio("4.mp3");           //Please receive your medicine audio
      delay(6000);
    }
    myservo1.write(0);
  }

  else if (key == 'B') {
    has_cough = true;
    lcd.clear();
    lcd.print("Cough Treatment");
    myservo3.write(165);
    delay(500);
    myservo3.write(90);
    delay(500);
    play_audio("5.mp3");          //Please receive your medicine audio
    delay(5000);
  }

  else if (key == 'C') {
    play_audio("6.mp3");      //eye glass ejection audio
    lcd.print("Eye Checkup");
    delay(5000);
    myservo4.write(35);
    delay(200);
    lcd.setCursor(0, 1);
    while (true) {
      for (int c = 0; c < eye_check.length(); c++) {
        while (true) {
          key = customKeypad.getKey();
          if (key) {
            seen_value += key;
            lcd.print(key);
            break;
          }
        }
      }
      if (seen_value == eye_check) {
        play_audio("7.mp3"); //correct glass audio
        delay(4000);
        lcd.clear();
        lcd.print("correct glass");
        break;
      }
      else {
        play_audio("8.mp3"); //take another glass
        delay(3000);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("take another");
        lcd.setCursor(4, 1);
        lcd.print("glass");
        seen_value = "";
        lens_power += 0.25;
        delay(1000);
        lcd.clear();
        lcd.print("Enter Again:");
        lcd.setCursor(0, 1);
      }
    }
  }

  else if (key == 'D') {
    play_audio("9.mp3"); // please put your hand
    delay(6000);
    lcd.setCursor(2, 0);
    lcd.print("Heart Rate");
    lcd.setCursor(2, 1);
    lcd.print("Please Wait");
    delay(100);
    myservo2.write(0);
    delay(200);

    hr = enable_sensor();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("hr: "+ String(hr));
    myservo2.write(55);
  }

  else if (key == '#') {
    lcd.clear();
    lcd.print("Printing report");
    lcd.setCursor(2, 1);
    send_data(Phone_number, Age, Temp, hr, 95, has_cough, lens_power);
    delay(500);
    print_report();
    for (int cnt = 0; cnt < 10; cnt++) {
      lcd.print(".");
      delay(1000);
    }
    play_audio("11.mp3"); // please take your report
    delay(7000);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Thank You!");
    return;
  }

  else {
    lcd.print("wrong key!");
    delay(1000);
    check_up();
  }

  delay(500);
  play_audio("10.mp3"); // ask for further service
  delay(5000);
  check_up();
}
