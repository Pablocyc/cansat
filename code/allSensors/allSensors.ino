// Libraries
#include <MPU6050_tockn.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"
#include "DHT.h"

#define DHTTYPE DHT11 
#define DHTPIN 3
#define ARDBUFFER 16

// Settings sensors
Adafruit_VEML6070 uv = Adafruit_VEML6070();
MPU6050 mpu6050(Wire, 0.02, 0.98);
DHT dht(DHTPIN, DHTTYPE);

/*****PINOUT**********    
// MPU6050
SCL = A5
SDA = A4
INT = 2
// VEML6070
SCL = SCL
SDA = SDA
**********************/

void setup() {
  Serial.begin(115200);
  uv.begin(VEML6070_1_T);
  dht.begin();
  Wire.begin();
  mpu6050.begin();
  mpu6050.setGyroOffsets(-3.68, 0.39, -0.28);
  // mpu6050.calcGyroOffsets(true);     // calibration
}

void loop() {
  // read sensors
  long senUV = ( uv.readUV() / 65535 ) * 100;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  mpu6050.update();
  float ax = mpu6050.getAccX();
  float ay = mpu6050.getAccY();
  float az = mpu6050.getAccZ();
  float gx = mpu6050.getGyroX();
  float gy = mpu6050.getGyroY();
  float gz = mpu6050.getGyroZ();
  char fin = '#';

  Serial.println("UV, hum, temp, accX, accY, accZ, gyroX, gyroY, gyroZ");
  ardprintf("#%l$%f$%f$%f$%f$%f$%f$%f$%f%c", senUV, h, t, ax, ay, az, gx, gy, gz, fin);
  delay(1000);
}

int ardprintf(char *str, ...)
{
  int i, count=0, j=0, flag=0;
  char temp[ARDBUFFER+1];
  for(i=0; str[i]!='\0';i++)  if(str[i]=='%')  count++;

  va_list argv;
  va_start(argv, count);
  for(i=0,j=0; str[i]!='\0';i++)
  {
    if(str[i]=='%')
    {
      temp[j] = '\0';
      Serial.print(temp);
      j=0;
      temp[0] = '\0';

      switch(str[++i])
      {
        case 'd': Serial.print(va_arg(argv, int));
                  break;
        case 'l': Serial.print(va_arg(argv, long));
                  break;
        case 'f': Serial.print(va_arg(argv, double));
                  break;
        case 'c': Serial.print((char)va_arg(argv, int));
                  break;
        case 's': Serial.print(va_arg(argv, char *));
                  break;
        default:  ;
      };
    }
    else 
    {
      temp[j] = str[i];
      j = (j+1)%ARDBUFFER;
      if(j==0) 
      {
        temp[ARDBUFFER] = '\0';
        Serial.print(temp);
        temp[0]='\0';
      }
    }
  };
  Serial.println();
  return count + 1;
}
