#include <SHT1X.h>
#include "IRremote.h"
IRsend irsend;  //IRsendオブジェクトの生成

int TIME = 60;  // 室温を計算する間隔(秒)
float TEMP = 30;  // LEDを点ける気温の閾値(度)
int KHZ=38; // 信号の周波数は38[kHz]

// 気温, 湿度を保存
float tempC = 0;
float humidity = 0;

//Create an instance of the SHT1X sensor
SHT1x sht15(A4, A5);//Data, SCK

//delacre output pins for powering the sensor
int power = A3;
int gnd = A2;

void setup()
{
  Serial.begin(9600); // Open serial connection to report values to host

  pinMode(power, OUTPUT);
  pinMode(gnd, OUTPUT);

  digitalWrite(power, HIGH);
  digitalWrite(gnd, LOW);
}
//-------------------------------------------------------------------------------------------
void loop()
{
  readSensor();  // 室温を測定
  //printOut();  // シリアルモニタに値を表示
  sendLED();  // 気温が閾値以下だとLEDを点灯
  delay(TIME*1000);  // TIME秒待機
}
//-------------------------------------------------------------------------------------------
void readSensor()
{
  // Read values from the sensor
  tempC = sht15.readTemperatureC();
  humidity = sht15.readHumidity();  
}
//-------------------------------------------------------------------------------------------
void printOut()
{
  Serial.print(" Temp = ");
  Serial.print(tempC);
  Serial.println("C");
  Serial.print(" Humidity = ");
  Serial.print(humidity); 
  Serial.println("%");
}
//-------------------------------------------------------------------------------------------
// 室温がTEMP度以下だとLEDを発光
void sendLED()
{
  if (tempC <= TEMP)
  {
    // 赤外線LEDの信号パターン
    unsigned int ledON[] = {3500,1650, 500,350, 500,1250, 450,400, 450,400, 450,400, 450,400, 500,400, 450,400, 450,400, 450,400, 450,400, 450,400, 500,350, 500,1250, 450,400, 450,400, 450,400, 450,400, 500,400, 450,400, 450,400, 450,1250, 450,1250, 500,1250, 450,400, 450,400, 450,1250, 500,400, 450,400, 450,400, 450,400, 450,400, 450,400, 500,400, 450,400, 450,400, 450,400, 450,400, 450,400, 450,400, 500,400, 450,400, 450,400, 450,400, 450,400, 500,350, 500,400, 450,400, 450,400, 450,400, 450,400, 450,400, 500,350, 500,400, 450,400, 450,400, 450,400, 450,1250, 500,1250, 450,400, 450,400, 450,400, 450,400, 500,400, 450};  // PANASONIC 4004:7200000
    // 赤外線LEDを点灯
    irsend.sendRaw(ledON, sizeof(ledON)/sizeof(ledON[0]), KHZ);
  }
}
