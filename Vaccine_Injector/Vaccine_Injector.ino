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
  pinMode(6, INPUT); //echo
  pinMode(7, OUTPUT); // trigger

  ///motor controller
  pinMode(2, OUTPUT);//IN1
  pinMode(3, OUTPUT);//IN2


  //push air
  pinMode(5, OUTPUT);//pwm 
  
  //servo motor
  servo.attach(10);  
  servo.write(0);
  delay(1500);

  
  //LCD Display
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Vaccine Injector");
  delay(1000);
  lcd.clear();

  //initial state
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  
}

void loop() {
  digitalWrite(7, LOW);
  delayMicroseconds(2);
  digitalWrite(7, HIGH);    //trigger
  delayMicroseconds(2);
  digitalWrite(7, LOW);

  duration = pulseIn(6, HIGH);
  distance = 332*(duration/10000); //cm*2
  distance = (distance/2); //cm

  if(distance < dist && distance > 0){
    lcd.print("Hand detected!");
    delay(1500);
    
    digitalWrite(2, HIGH); //r_motor on
    delay(750);
    
    digitalWrite(2, LOW); //r_motor off
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("injecting");

    while(1){
      i++;
      servo.write(i); //push
      delay(2);
      if(i==90){
        break;        
      }
    }

    digitalWrite(5, HIGH); // if ! Use analogWrite
    delay(1200);
    digitalWrite(5, LOW);
    delay(400);
    
    while(1){
      i--;
      servo.write(i); //release
      delay(2);
      if(i==0){
        break;
      }
    }

    
    
    delay(1000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Done!");
    lcd.setCursor(0, 1);
    lcd.print("Have a good day");
    delay(1500);
    
    digitalWrite(3, HIGH);//motor return
    delay(1000);

    digitalWrite(3, LOW);

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

  
  
  if((Time1 - Time2) == 800){
    lcd.clear();
    lcd.setCursor(cur, 1);
    lcd.print("Save the world");
    cur++;
    a = 1;
    if(cur==15){
      cur = 0;
    }
     
   }
  
}
