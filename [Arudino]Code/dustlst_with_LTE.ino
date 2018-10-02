#include "Arduino.h"
#include <skt_lte_m1.h>
#include <DHT11.h>

#include <LedControl.h>
#include <pm2008_i2c.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
PM2008_I2C pm2008_i2c;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SKTP_HOST_IP    "test.sktiot.com"
#define SKTP_PORT       1883
#define SKTP_DEV_TOKEN  "bc6dfcb0c34311e8b1dc"
#define SKTP_SERVICE_ID "893e1b0712a11e8bdb583278"
#define SKTP_DEV_ID     "DUST1"
float total_grade = 0;
float data = 230 ;
DHT11 dht11(8);
 LedControl lc=LedControl(12,11,10,1);
#define CDS_PIN  1      // A1 for get cds value

#define CDS_OFFSET  300 // 300

unsigned long lastSendTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60000L; // delay between updates, in milliseconds

typedef struct
{
    int   dust;
} sensorData;

sensorData dust_prev_sensor_data;
sensorData dust_curr_sensor_data;

//The setup function is called once at startup of the sketch
void setup()
{
    Serial.begin(115200);
    Serial3.begin(115200);
    lcd.begin(); // lcd를 사용을 시작합니다.
  lcd.backlight(); 
   lc.clearDisplay(0);
     lc.shutdown(0,false);
  lc.setIntensity(0,8);
    if(LTE.init(&Serial3) != true){
        Serial.println("Fail to initialize LTE Module. Please Check your H/W Status");
        // don't continue
        while(true);
    }

    if(LTE.begin() != true)
    {
        Serial.println("Fail to attach the LTE Network");
        // don't continue
        while(true);
    }

    Serial.println("You're connected to the network");
    Serial.println(LTE.getIMEI());
    Serial.println(LTE.getSwVersion());
}

// The loop function is called in an endless loop
void loop()
{
    float temp, humi;
 
  dht11.read(humi, temp);
  delay(DHT11_RETRY_DELAY);
  lcd.clear();
  get_dustDensity();
  dustCheckLed(data);
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
    char data[500]={0,};
    char cmd_type[20]={0,};
    uint32_t rpc_id;
    sensorData cur_sensor_data;

    if( LTE.getStatusSKTP() == false ){
        Serial.println("Starting connection to SKT ThingPlug server");
        LTE.connectSKTP((char*)SKTP_HOST_IP, SKTP_PORT, (char*)SKTP_DEV_TOKEN, (char*)SKTP_SERVICE_ID, (char*)SKTP_DEV_ID);
        lastSendTime = millis();
    }
    else{
        if( LTE.recvDataSKTP(cmd_type, data,500,&rpc_id) > 0){
            if(strncmp(cmd_type,"tp_reboot",strlen(cmd_type)) == 0){
                // For two way RPC
                Serial.println("Device Reboot");

                // AT+SKTPRES command is not implemented from Woori-net
                LTE.sendRespSKTP(cmd_type, rpc_id, 0); // 0: Success
                delay(2000);
                //softwareReset();
            }
            else if(strncmp(cmd_type,"tp_reset",strlen(cmd_type)) == 0){
                Serial.println("Device Reset");
                Serial.println(data);
                Serial.println(rpc_id);
            }
            else if(strncmp(cmd_type,"tp_user",strlen(cmd_type)) == 0){
                Serial.println("Device User Command");
                Serial.println(data);
                Serial.println(rpc_id);
            }
        }

        if( getSensorData() ){
            lastSendTime = millis();
            sendSensorData();
        }
        else if( millis() - lastSendTime > postingInterval) {
            lastSendTime = millis();
            sendSensorData();
        }
    }
}

bool getSensorData(void)
{
    //Serial.println("Collecting temperature data.");

    int cdsValue = analogRead(CDS_PIN);

    dust_curr_sensor_data.dust = data;

    if( abs(dust_prev_sensor_data.dust -  dust_curr_sensor_data.dust)  > data ){
        return true;
    }

    return false;
}

void sendSensorData(void)
{
    char payload[100];

    Serial.print("dust1 : ");
    Serial.println(dust_curr_sensor_data.dust);

    dust_prev_sensor_data.dust = dust_curr_sensor_data.dust;

    sprintf(payload, "{\"dust1\":%d}", dust_curr_sensor_data.dust);
    Serial.println(payload);

    LTE.sendDataSKTP(SktLteM1Drv::TELEMETRY, payload, SktLteM1Drv::JSON);
    //LTE.sendDataSKTP("telemetry", payload, 0);
}

void softwareReset()
{
    asm volatile ("  jmp 0");
}

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
}
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


