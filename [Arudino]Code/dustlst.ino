#include <DHT11.h>

#include <LedControl.h>
#include <pm2008_i2c.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
PM2008_I2C pm2008_i2c;
LiquidCrystal_I2C lcd(0x27, 16, 2);
//----------------------------------------------------------------------------------------------------------------------
float sensitivity = 0.1;
float pre_dustDensity = 0;
float total_grade = 0;
float data = 230 ;

//----------------------------------------------------------------------------------------------------------------------

DHT11 dht11(8);
 //LedControl lc=LedControl(12,11,10,1);

//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
float get_dustDensity() {
  
  uint8_t ret = pm2008_i2c.read();
  uint8_t pm2p5_grade = 0;
  uint8_t pm10p_grade = 0;
  

  if (ret == 0) {
    // PM 1.0
   
    if (pm2008_i2c.pm2p5_grimm < 16) {
      pm2p5_grade = 1;
    } else if (pm2008_i2c.pm2p5_grimm < 51) {
      pm2p5_grade = 2;
    } else if (pm2008_i2c.pm2p5_grimm < 101) {
      pm2p5_grade = 3;
    } else {
      pm2p5_grade = 4;
    }

    // PM 10

    if (pm2008_i2c.pm10_grimm < 31) {
      pm10p_grade = 1;
    } else if (pm2008_i2c.pm10_grimm < 81) {
      pm10p_grade = 2;
    } else if (pm2008_i2c.pm10_grimm < 151) {
      pm10p_grade = 3;
    } else {
      pm10p_grade = 4;
    }
    // abcdef
    // Get worst grade
    data = max(pm10p_grade, pm2p5_grade);
  return data;
}
}
 /*
void dustCheckLed(float data) {
  //----------------------------------
  
byte veryGood_bmp[8] =
{ B00000000,
  B01000010,
  B10100101,
  B00000000,
  B00000000,
  B01000010,
  B00111100,
  B00000000
};
byte good_bmp[8] =
{ B00000000,
  B00000000,
  B00100100,
  B00100100,
  B00000000,
  B01111110,
  B00111100,
  B00000000
};
byte normal_bmp[8] =
{ B00000000,
  B00000000,
  B00100100,
  B00100100,
  B00000000,
  B01111110,
  B00000000,
  B00000000
};
byte bad_bmp[8] =
{ B00000000,
  B00000000,
  B01100110,
  B00000000,
  B00000000,
  B00111100,
  B01000010,
  B00000000
};
byte veryBad_bmp[8] =
{ B00000000,
  B00000000,
  B01000010,
  B00100100,
  B00000000,
  B00111100,
  B01000010,
  B00000000
};
//----------------------
  if (data > 201) {
    //매우 나쁨
  lc.setRow(0,0,veryBad_bmp[0]);
  lc.setRow(0,1,veryBad_bmp[1]);
  lc.setRow(0,2,veryBad_bmp[2]);
  lc.setRow(0,3,veryBad_bmp[3]);
  lc.setRow(0,4,veryBad_bmp[4]);
  lc.setRow(0,5,veryBad_bmp[5]);
  lc.setRow(0,6,veryBad_bmp[6]);
  lc.setRow(0,7,veryBad_bmp[7]);
  
  }
  else if (data > 120) {
    //나쁨
  lc.setRow(0,0,bad_bmp[0]); 
  lc.setRow(0,1,bad_bmp[1]);
  lc.setRow(0,2,bad_bmp[2]);
  lc.setRow(0,3,bad_bmp[3]);
  lc.setRow(0,4,bad_bmp[4]);
  lc.setRow(0,5,bad_bmp[5]);
  lc.setRow(0,6,bad_bmp[6]);
  lc.setRow(0,7,bad_bmp[7]);
  
  }
  else if (data > 80) {
    //약간 나쁨
  lc.setRow(0,0,normal_bmp[0]);
  lc.setRow(0,1,normal_bmp[1]);
  lc.setRow(0,2,normal_bmp[2]);
  lc.setRow(0,3,normal_bmp[3]);
  lc.setRow(0,4,normal_bmp[4]);
  lc.setRow(0,5,normal_bmp[5]);
  lc.setRow(0,6,normal_bmp[6]);
  lc.setRow(0,7,normal_bmp[7]);
  
  }
  else if (data > 30) {
  lc.setRow(0,0,good_bmp[0]);
  lc.setRow(0,1,good_bmp[1]);
  lc.setRow(0,2,good_bmp[2]);
  lc.setRow(0,3,good_bmp[3]);
  lc.setRow(0,4,good_bmp[4]);
  lc.setRow(0,5,good_bmp[5]);
  lc.setRow(0,6,good_bmp[6]);
  lc.setRow(0,7,good_bmp[7]);
  }
  else {
  lc.setRow(0,0,veryGood_bmp[0]);
  lc.setRow(0,1,veryGood_bmp[1]);
  lc.setRow(0,2,veryGood_bmp[2]);
  lc.setRow(0,3,veryGood_bmp[3]);
  lc.setRow(0,4,veryGood_bmp[4]);
  lc.setRow(0,5,veryGood_bmp[5]);
  lc.setRow(0,6,veryGood_bmp[6]);
  lc.setRow(0,7,veryGood_bmp[7]);
  }
}*/
//----------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
 lcd.begin(); // lcd를 사용을 시작합니다.
  lcd.backlight(); 
//   lc.clearDisplay(0);
  //   lc.shutdown(0,false);
  //lc.setIntensity(0,8);
}  
 
//----------------------------------------------------------------------------------------------------------------------
void loop() {
 
  float temp, humi;
 
  dht11.read(humi, temp);
  delay(DHT11_RETRY_DELAY);
  lcd.clear();
  
  get_dustDensity();
  //dustCheckLed(data);
  Serial.print(temp);
  lcd.setCursor(0, 0);
  lcd.print("Dust:");
  lcd.print(data);
  lcd.print("mg");
 
  lcd.setCursor(0, 1);
  lcd.print("T:");
  
  lcd.print(temp);
 
  lcd.print(" H:");
  lcd.print(humi);
}
