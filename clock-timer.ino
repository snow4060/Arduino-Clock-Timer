#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <uptime.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned int hour = 0;
unsigned int minute = 0;

int get_min(){
  return minute + hour*60;
}

bool is_pressed(const int& button){
  for (int i = 0; i < 200; i++){
    if (!digitalRead(button)){         
      delay(200);
      if (!digitalRead(button)){
        return true;
      }
    }
    delayMicroseconds(100);
  }
  return false;
}

void pomodoro(){
  unsigned int starting_time;
  lcd.clear();
  while(1){
    for(int i = 0; i < 4; i++){ //4 sets of work and break
      uptime::calculateUptime();
      starting_time = uptime::getMinutes();
      while(uptime::getMinutes() - starting_time < 25){
        lcd.setCursor(0, 0);
        lcd.print(25-uptime::getMinutes());
        lcd.setCursor(0, 1);
        lcd.print("work");
        delay(100);
        lcd.clear();
        uptime::calculateUptime();
        if(is_pressed(2)){
          return;
        }
      }
      uptime::calculateUptime();
      starting_time = uptime::getMinutes();
      while(uptime::getMinutes() - starting_time < 5 && !is_pressed(2)){
        lcd.setCursor(0, 0);
        lcd.print(5-uptime::getMinutes());
        lcd.setCursor(0, 1);
        lcd.print("short break");
        delay(100);
        lcd.clear();
        uptime::calculateUptime();
      }
      if(is_pressed(2)){
          return;
        }
    }
    uptime::calculateUptime();
    starting_time = uptime::getMinutes();
    while(uptime::getMinutes() - starting_time < 20 && !is_pressed(2)){ //1 longer break
      lcd.setCursor(0, 0);
      lcd.print(20-uptime::getMinutes());
      lcd.setCursor(0, 1);
      lcd.print("long break");
      delay(100);
      lcd.clear();
      uptime::calculateUptime();
    }
    if(is_pressed(2)){
      return;
    }

  }
}

void update_time(){
  uptime::calculateUptime();
  hour += uptime::getHours();
  minute += uptime::getMinutes();
  hour %= 24;
  minute %= 60;
}

void print_time(){
  //cursor is at 0, 0
  if(hour < 10){
    lcd.print("0");
    lcd.print(hour);
  }
  else{
    lcd.print(hour);
  }
  lcd.print(":");
  if(minute < 10){
    lcd.print("0");
    lcd.print(minute);
  }
  else{
    lcd.print(minute);
  }
}

void setup(){
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  

  pinMode(2, INPUT); //button for turning on/off pomodoro mode
  pinMode(3, INPUT); //button for minute
  pinMode(4, INPUT); //button for hour
  Serial.begin(9600);
  
}

void loop()
{  
  

  if(is_pressed(3)){
    minute++;
    Serial.println("minute++");
  }
  if(is_pressed(4)){
    hour++;
    Serial.println("hour++");
  }

  print_time();
  


  if(is_pressed(2)){ //start pomodoro mode
    pomodoro();
  }
  lcd.setCursor(0, 0);

  delay(50);

  lcd.clear();

  update_time();



}
