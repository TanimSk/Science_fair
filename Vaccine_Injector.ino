#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2) ;
Servo servo;

uint8_t a = 1;

int dist = 10;//cm
int a = 0;//angle
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
  pinMode(11, OUTPUT);//ENA

  //push air
  pinMode(4, OUTPUT);//pwm is preferred
  
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
  
}

void loop() {
  digitalWrite(7, LOW);
  delayMicroseconds(2);
  digitalWrite(7, HIGH);    //trigger
  delayMicroseconds(2);
  digitalWrite(7, LOW);

  duration = pulseIn(6, HIGH);
  distance = 332*duration/10000; //cm

  if(distance < dist && distance > 0){
    lcd.print("Hand detected!");
    delay(1500);
    
    digitalWrite(11, HIGH);
    digitalWrite(2, HIGH); //r_motor on
    delay(1500);
    
    digitalWrite(11, LOW);
    digitalWrite(2, LOW); //r_motor off
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("injecting");

    while(1){
      i++;
      servo.write(i);
      delay(2);
      if(i==90){
        break;        
      }
    }
    
    while(1){
      i--;
      servo.write(i);
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
    
    digitalWrite(11, HIGH);
    digitalWrite(3, HIGH);
    delay(2000);

    digitalWrite(11, LOW);
    digitalWrite(3, LOW);

    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Vaccine Injector")

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
