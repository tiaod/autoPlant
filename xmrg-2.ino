#include <LiquidCrystal.h>
#include <DHT.h>
#include <Time.h>
#include <TimeAlarms.h>
///////////////////////custom settings///////////////////////////////////////
#define CHECK_INTERVAL 15 //检查土壤湿度间隔的秒数(三小时一次)
#define MAX_SOIL_HUMIDITY 600 //土壤的湿度阈值
#define WATER_TIME 5 //单次浇水的秒数
#define IS_MINIMUN_POWER true //是否开启节能模式。用于太阳能电池供电的情况

///////////////////////ping define///////////////////////////////////////////
#define RELAY1 9    //继电器1,节能模式时控制其他模块的电源
#define RELAY2 10   //继电器2,控制水阀
#define DHT11_PIN 2 //温湿度模块
#define SOIL_HUMIDITY_PIN A1 //土壤湿度模块

////////////////////////////////Instanciate objects//////////////////////////////////////////////
DHT dht(DHT11_PIN, DHT11);  //初始化温湿度模块
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);//初始化LCD

void check_soid_humidity(){
  digitalWrite(RELAY1, HIGH);
  Serial.print(F("Check soil humidity....."));
  lcd.setCursor(0,0);
  lcd.print("Checking soil...");
  for(int i =0;i <= 5;i++){//重复检查5次，如果5次都是干旱，则确定要浇水
    if(analogRead(SOIL_HUMIDITY_PIN)<MAX_SOIL_HUMIDITY){
      Serial.println(F("done"));
      return;//检查到没有必要，则退出循环
    }
    else{
      digitalDHT11Display();
      Serial.print(F("dry, "));
      delay(10000);//间隔10秒
    }
  }
  Serial.println();
  Serial.print(F("Soil has been dry, Turn on relay2..."));
  digitalWrite(RELAY2, HIGH);  //进行浇水操作
  delay(WATER_TIME*1000);
  digitalWrite(RELAY2, LOW);  //进行浇水操作
  Serial.println(F("done"));
  return;
}

void LCDprintDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
void digitalRuntimeDisplay(){
  time_t t=now();
  lcd.setCursor(0,0);
  lcd.print(t/86400);
  lcd.print(F("d "));
  LCDprintDigits(t%86400/3600);
  lcd.print(F(":"));
  LCDprintDigits(t%86400%3600/60);
  lcd.print(F(":"));
  LCDprintDigits(t%86400%60);
  lcd.print(F("       "));
}
void digitalDHT11Display(){//只有常规状态才读取温度并且显示，防止卡顿
  lcd.setCursor(0, 1);
  if(digitalRead(RELAY1)==0){
    lcd.print("                ");
    return;//退出
  }
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    
    lcd.print((int)t);
    lcd.print((char)223); //显示o符号
    lcd.print("C "); //显示字母C
    lcd.print((int)h);
    lcd.print("% ");
  }
  lcd.print(analogRead(SOIL_HUMIDITY_PIN));
  lcd.print("      ");
}

void setup() {
  if(!IS_MINIMUN_POWER){
    digitalWrite(RELAY1, HIGH);
  }
  Serial.begin(9600);
  Serial.print(F("init..."));
  lcd.begin(16, 2);
  lcd.print(F("hello, xmrg!"));
  //////////////////////////////DHT11////////////////////////////////////
   
  dht.begin();//DHT11
  //////////////////////////////RELAY1////////////////////////////////////

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  Alarm.timerRepeat(CHECK_INTERVAL, check_soid_humidity); //每间隔一段时间检查一次
  Alarm.timerRepeat(60, digitalDHT11Display); //每分钟更新一次温湿度
  
  Serial.println(F("done."));
}

// the loop function runs over and over again forever
void loop() {
  digitalRuntimeDisplay();
  Alarm.delay(1000); // wait one second between clock display
  //===================================debug===================================================
   if (Serial.available()) {//serial debug
    String read_string = Serial.readStringUntil('\n');

    if (read_string.equals(F("relay1 on"))) {//调试继电器1
      Serial.println(F("Turn on relay1"));
      digitalWrite(RELAY1, HIGH);   // turn the LED on (HIGH is the voltage level)  
      Serial.println(digitalRead(RELAY1));
    }
    else if(read_string.equals(F("relay1 off"))){//调试继电器1
      Serial.println(F("Turn off relay1"));
      digitalWrite(RELAY1, LOW);
    }
    else if (read_string.equals(F("relay2 on"))) {//调试继电器2
      Serial.println(F("Turn on relay2"));
      digitalWrite(RELAY2, HIGH);   // turn the LED on (HIGH is the voltage level)  
    }
    else if(read_string.equals(F("relay2 off"))){//调试继电器2
      Serial.println(F("Turn off relay2"));
      digitalWrite(RELAY2, LOW);
    }
    else if(read_string.equals(F("dht11"))){//调试温湿度模块
      float t = dht.readTemperature();
      float h = dht.readHumidity();
      if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
      } else {
        Serial.print(F("Humidity:"));
        Serial.println(h);
        Serial.print(F("Temperature:"));
        Serial.println(t);
      }
    }
    else if(read_string.equals(F("soil"))){//调试土壤湿度模块
      Serial.println(analogRead(SOIL_HUMIDITY_PIN));
    }
   }
}

