#include <SHT1X.h>

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
  calAverage(10);  // 10秒毎の平均を出力
  printOut();
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
void calAverage(int num)    // num秒間の平均を計算
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
