#ifndef IMU_H
#define IMU_H

#include<Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
extern Adafruit_BNO055 bno;
extern uint8_t Green_zone, ST_zone;
extern int BuzzerPin,Motor1;
extern int vib_time,vib_gap_time;
extern int cal_flag;
typedef struct {
    float pitch;
    float roll;
    float yaw;
    float accelX;
    float accelY;
    float accelZ;
}IMUData;
extern IMUData currentData,oldData;
typedef struct {
    float Threshold_pitch;
    float Thresold_accl;
    float accl;
}Data;
extern Data HS,TO,Stand,Mangnetude;

typedef struct{
    unsigned long HS[100];
    unsigned long TO[100];
    unsigned long watching_you;
    unsigned long ACK_time;
    unsigned long ST_Window;
    unsigned long FZ_Window;
    float Delta_HS;
    float Delta_TO;
    int Array;
    int vib_time;
    int HS_delay;
}Time;
extern Time Recordtime;
typedef struct{
    uint8_t HS: 1;
    uint8_t TO: 1;
    uint8_t vibration: 1;
    uint8_t read: 1;
    uint8_t write: 1;
    uint8_t update_vibtime: 1;
    uint8_t update_vibtimegap: 1;
    uint8_t update_vib_num: 1;
    uint8_t update_freez_time: 1;
    uint8_t update_freez_count: 1;
    uint8_t Game: 1;
}Event;

extern Event slave_flag,master_flag,flash, mode;

typedef struct{
    uint8_t vib;
    uint8_t totalvib;
    uint8_t GT;     //Gettring
}counting;

extern counting count;

extern unsigned long ticktime;
extern char sentdata[100];

typedef struct{
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
    int16_t Accl;     //Gettring
}convert;

extern convert manipulate;

void dataRead(void);
void printEvent1(sensors_event_t* event);
void printEvent2(sensors_event_t* event);
void configureThresholds(void);
void Green_area();
void HeelStrike(void);
void ReadTime(void);
void SOS(void);
void data_manipulation();
#endif