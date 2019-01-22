#include <SHT1X.h>

int TIME = 30;  // 平均を計算する間隔(秒)
float TEMP = 24;  // LEDが切り替わる気温の閾値(度)


// 気温, 湿度を保存
float tempC = 0;
float humidity = 0;
// 平均用
float ave_tempC = 0;
float ave_humidity = 0;

//Create an instance of the SHT1X sensor
SHT1x sht15(A4, A5);//Data, SCK

//delacre output pins for powering the sensor
int power = A3;
int gnd = A2;

// LEDのピン
int led_green = 12;
int led_red = 13;

void setup()
{
  Serial.begin(9600); // Open serial connection to report values to host

  pinMode(power, OUTPUT);
  pinMode(gnd, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);

  digitalWrite(power, HIGH);
  digitalWrite(gnd, LOW);
  digitalWrite(led_green, HIGH);
  digitalWrite(led_red, HIGH);
}
//-------------------------------------------------------------------------------------------
void loop()
{
  calAverage(TIME);  // 指定された秒数毎に平均を計算
  printOut();
  setLED(TEMP);  // 気温が閾値以下だと赤LEDを点灯
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
  Serial.print(ave_tempC);
  Serial.println("C");
  Serial.print(" Humidity = ");
  Serial.print(ave_humidity); 
  Serial.println("%");
}
//-------------------------------------------------------------------------------------------
void calAverage(int num)    // num秒毎に平均を計算
{
  // 一時保存用
  float tmp_tempC = 0;
  float tmp_humidity = 0;

  // 1秒毎に足し合わせ, 計num回計算
  for (int i=0; i<num; i++)
  {
    readSensor();
    tmp_tempC += tempC;
    tmp_humidity += humidity;
    delay(1000);
  }
  
  // numで割って平均を保存
  ave_tempC = tmp_tempC / num;
  ave_humidity = tmp_humidity / num;
}
//-------------------------------------------------------------------------------------------
// 室温がtemp度以下だと赤色, それ以外は緑色のLEDを光らせる
void setLED(float temp)
{
  if (ave_tempC <= temp)
  {
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, HIGH);
  }
  else
  {
    digitalWrite(led_green, HIGH);
    digitalWrite(led_red, LOW);
  }
}
