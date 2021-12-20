#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2) ;
Servo servo;

uint8_t a = 1; //state

int dist = 7;//cm
int i = 0;//angle
int cur = 0;//cursor pos

int duration = 0;
int distance = 0;
int Time1 = 0; 
int Time2 = 0;

void setup() {
  //ultrasonic
  pinMode(0, INPUT); //echo_D3
  pinMode(2, OUTPUT); //trigger_D4

  ///motor controller
  pinMode(13, OUTPUT);//IN1_D7
  pinMode(15, OUTPUT);//IN2_D8

  //Buzzer
  pinMode(16, OUTPUT);//D0


  //push air
  pinMode(12, OUTPUT);//D6
  
  //servo motor
  servo.attach(14);//D5
  servo.write(0);
  delay(1500);

  
  //LCD Display
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Vaccine Injector");
  delay(1000);
  lcd.clear();

  //initial state
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
  
}

void loop() {
  digitalWrite(2, LOW);
  delayMicroseconds(2);
  digitalWrite(2, HIGH);    //trigger
  delayMicroseconds(2);
  digitalWrite(2, LOW);

  duration = pulseIn(0, HIGH);
  distance = 332*(duration/10000); //cm*2
  distance = (distance/2); //cm

  if(distance < dist && distance > 0){
    lcd.print("Hand detected!");
    
    digitalWrite(16, HIGH);//beep_on
    delay(1000);
    digitalWrite(16, LOW);//beep_off
    
    delay(600);
    
    digitalWrite(13, HIGH); //r_motor on
    delay(750);
    
    digitalWrite(13, LOW); //r_motor off
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("injecting");
    
    digitalWrite(16, HIGH);//beep_on
    delay(1000);
    digitalWrite(16, LOW);//beep_off

    while(1){
      i++;
      servo.write(i); //push
      delay(2);
      if(i==90){
        break;        
      }
    }

    digitalWrite(12, HIGH); // if ! Use analogWrite
    delay(1200);
    digitalWrite(12, LOW);
    delay(400);
    
    while(1){
      i--;
      servo.write(i); //release
      delay(2);
      if(i==0){
        break;
      }
    }

    digitalWrite(16, HIGH);//beep_on
    delay(1000);
    digitalWrite(16, LOW);//beep_off
    
    delay(1000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Done!");
    lcd.setCursor(0, 1);
    lcd.print("Have a good day");
    delay(1500);
    
    digitalWrite(15, HIGH);//motor return
    delay(1000);

    digitalWrite(15, LOW);

    digitalWrite(16, HIGH);//beep_on
    delay(1000);
    digitalWrite(16, LOW);//beep_off

    lcd.clear();
    delay(100);
  }

  lcd.setCursor(0, 0);
  lcd.print("Vaccine Injector");

  Time1 = millis();

  if(a==1){
    Time2 = Time1;
    a = 0;
  }

  
}
