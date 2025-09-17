#ifndef BT_H
#define BT_H

#include <NimBLEDevice.h>
#include<imu.h>
#include<memory.h>
const NimBLEAdvertisedDevice* advDevice;
bool doConnect = false;
uint32_t scanTimeMs = 5000;  // scan time in ms, 0 = forever

#define SERVICE_UUID        "374e714b-0838-4d2e-8ae3-fb28c585474b"
#define CHARACTERISTIC_UUID "41a1a8a8-695c-44fd-aa60-5f258be92169"

char hs_flag[10]="HStrike,1";
uint8_t M_ack=0;
// Global pointer to remote characteristic (for writing later)
NimBLERemoteCharacteristic* pRemoteChr = nullptr;

void notifyCB(NimBLERemoteCharacteristic* pChr, uint8_t* pData, size_t length, bool isNotify) {

 String strMsg((char*)pData, length);
    // Serial.printf(" Received string: %s\n", strMsg.c_str());
    int sepIndex = strMsg.indexOf(','); 
    if (sepIndex != -1) {
        // Format = key:value
        String key = strMsg.substring(0, sepIndex);
        int valStr = strMsg.substring(sepIndex + 1).toInt();
   if(key=="cal" && valStr==1)
   {
    configureThresholds();
   }
   if(key=="HStrike" && valStr==1)
   {
    M_ack=1;
    // Serial.println("master ack");
   }
   if(key=="read" && valStr==2)
   {
    flash.read=1;
   }
   if(key=="vibtime" && valStr>200)
   {
    vib_time=valStr;
    master_flag.update_vibtime=1;
   }
   if(key=="vibtimegap" && valStr>200)
   {
    vib_gap_time=valStr;
    master_flag.update_vibtimegap=1;
   }
   if(key=="vibcount" && valStr>=1)
    {
    count.totalvib=valStr;
    slave_flag.update_vib_num=1;
    }
    if(key=="maxdelay" && valStr>2000)
    {
    Recordtime.HS_delay=valStr;
    master_flag. update_freez_time=1;
    }

}
}
// ---------- Client Callbacks ----------
class ClientCallbacks : public NimBLEClientCallbacks {
    void onConnect(NimBLEClient* pClient) override {
        Serial.println("Connected to server");
    }
    void onDisconnect(NimBLEClient* pClient, int reason) override {
        Serial.printf("Disconnected, reason = %d. Restarting scan...\n", reason);
        pRemoteChr = nullptr;  // Reset pointer
        NimBLEDevice::getScan()->start(scanTimeMs, false, true);
    }
} clientCallbacks;

// ---------- Scan Callbacks ----------
class ScanCallbacks : public NimBLEScanCallbacks {
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
        Serial.printf("Found device: %s\n", advertisedDevice->toString().c_str());
        if (advertisedDevice->isAdvertisingService(NimBLEUUID(SERVICE_UUID))) {
            Serial.println("Target service found! Stopping scan.");
            NimBLEDevice::getScan()->stop();
            advDevice = advertisedDevice;
            doConnect = true;
        }
    }
} scanCallbacks;

// ---------- Connect to Server ----------
bool connectToServer() {
    NimBLEClient* pClient = NimBLEDevice::createClient();
    Serial.println("Created client");

    pClient->setClientCallbacks(&clientCallbacks, false);

    if (!pClient->connect(advDevice)) {
        Serial.println("Failed to connect");
        NimBLEDevice::deleteClient(pClient);
        return false;
    }

    Serial.printf("Connected to: %s\n", pClient->getPeerAddress().toString().c_str());

    NimBLERemoteService* pSvc = pClient->getService(SERVICE_UUID);
    if (!pSvc) {
        Serial.println("Service not found!");
        pClient->disconnect();
        return false;
    }

    pRemoteChr = pSvc->getCharacteristic(CHARACTERISTIC_UUID);
    if (!pRemoteChr) {
        Serial.println("Characteristic not found!");
        pClient->disconnect();
        return false;
    }

    if (pRemoteChr->canNotify()) {
        if (!pRemoteChr->subscribe(true, notifyCB)) {
            Serial.println("Subscribe failed");
            pClient->disconnect();
            return false;
        }
        Serial.println("Subscribed to notifications");
    }

    return true;
}

void HandleBle()
{
    if (M_ack==1)
    {
        Recordtime.ACK_time=millis();
        count.vib=0;
        M_ack=0;
        Serial.println("Master HS : "+String(Recordtime.ACK_time));
    }
    if (slave_flag.HS==1)
    {
    if (pRemoteChr->canWrite()) {
        pRemoteChr->writeValue((uint8_t*)&hs_flag, strlen(hs_flag), true);   
    slave_flag.HS=0;
    }
    }
    if ( flash.read==1)
    {
        Read_data();
    if (pRemoteChr->canWrite()) {
    pRemoteChr->writeValue((uint8_t*)&sentdata, strlen(sentdata), true);   
    flash.read=0;
    }
    }
    if(master_flag.update_vibtime==1)
    {
       Write_config_data(); 
       Serial.println("vibtime: "+String(vib_time)); 
       master_flag.update_vibtime=0;
    }
    if(master_flag.update_vibtimegap==1)
    {
       Write_config_data(); 
       Serial.println("vibtimegap: "+String(vib_gap_time)); 
       master_flag.update_vibtimegap=0;
    }
    if(slave_flag.update_vib_num==1)
    {
       Write_config_data(); 
       Serial.println("Number of vibration: "+String(count.totalvib)); 
       slave_flag.update_vib_num=0;
    }
    if(master_flag. update_freez_time==1)
    {
       Write_config_data(); 
       Serial.println("Max freezing time: "+String(Recordtime.HS_delay)); 
       master_flag. update_freez_time=0;
    }

}
#endif