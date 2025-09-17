#include <Wire.h>
#include<imu.h>
#include<BT.h>
#include<memory.h>
void setup(void)
{
  Serial.begin(115200);
  pinMode(BuzzerPin,OUTPUT);
  pinMode(Motor1,OUTPUT);
  digitalWrite(BuzzerPin, HIGH);
  digitalWrite(Motor1, HIGH);
  delay(200);
  digitalWrite(BuzzerPin, LOW);
  digitalWrite(Motor1, LOW);
if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM init failed");
    while (1);
  }
  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
NimBLEDevice::init("ESP32_Client");
    NimBLEDevice::setPower(3); // 3 dBm
    NimBLEDevice::setMTU(256);

    NimBLEScan* pScan = NimBLEDevice::getScan();
    pScan->setScanCallbacks(&scanCallbacks, false);
    pScan->setInterval(100);
    pScan->setWindow(99);
    pScan->setActiveScan(true);

    pScan->start(scanTimeMs);
    Serial.println("Scanning...");
  delay(1000);
  
  ticktime=millis();
  // Write_config_data();
  Read_data();
  Read_config_data();
}

void loop(void)
{
   if (doConnect) {
        doConnect = false;
        if (connectToServer()) {
            Serial.println("Now receiving notifications...");
        } else {
        Serial.println("Connection failed, restarting scan...");
        NimBLEDevice::getScan()->start(scanTimeMs, false, true);
        } 
    }
Recordtime.watching_you=millis(); 
dataRead();
// Serial.print(",");
// Serial.print(TO.Threshold_pitch);     // Heel Strike
// Serial.print(",");
// Serial.println(Stand.Threshold_pitch);
// Serial.print(",");
// Serial.println(HS.Threshold_pitch);    // Toe off
// Serial.print(",");
// Serial.println(Mangnetude.Thresold_accl);    // Toe off
HeelStrike();
Green_area();
HandleBle();
delay(10);
// digitalWrite(Motor1, LOW);
}