#include<imu.h>
#include<memory.h>
int BuzzerPin=12,Motor1=19;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
IMUData currentData ={0},oldData={0};
unsigned long ticktime=0;
Data HS={0},TO={0},Stand={0}, Mangnetude={0};
uint8_t Green_zone=0,ST_zone=0;
Time Recordtime={0};
Event slave_flag={0}, master_flag={0}, flash={0}, mode={0};
int vib_time=500;
int vib_gap_time=500;
counting count={0,3};
char sentdata[100]={0};
convert manipulate={0};
void dataRead(void)
{
   //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  printEvent1(&orientationData);
  // printEvent(&angVelocityData);
  printEvent2(&linearAccelData);
  // printEvent(&magnetometerData);
  // printEvent(&accelerometerData);
  // printEvent(&gravityData);

  // int8_t boardTemp = bno.getTemp();
  // Serial.println();
  // Serial.print(F("temperature: "));
  // Serial.println(boardTemp);

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  delay(10);
}

void printEvent1(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    // Serial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    // Serial.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    // Serial.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    // Serial.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    // Serial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    // Serial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    Serial.print("Unk:");
  }
  currentData.yaw=x;
  currentData.pitch=y;
  currentData.roll=z;
  // Serial.print("\tx= ");
  // Serial.print(x);
  // Serial.print(",");
  Serial.println(currentData.roll);
  Serial.println(currentData.pitch);
  Serial.println(currentData.yaw);
  // Serial.print(",");
  // Serial.println(z);
}
void printEvent2(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    // Serial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  
  else {
    Serial.print("Unk:");
  }
  currentData.accelX=x;
  currentData.accelY=y;
  currentData.accelZ=z;
  Mangnetude.accl=sqrt(x*x+y*y+z*z);
  // Serial.print("\tx= ");
  // Serial.print(currentData.accelX);
  // Serial.print(",");
//   Serial.print(currentData.accelY);
  // Serial.print(",");
//   Serial.println(currentData.accelZ);
// Serial.print(Mangnetude.accl);
}

void configureThresholds(void)
{
  for(int i =0; i<2; i++)
  {
  digitalWrite(BuzzerPin, HIGH);
  delay(100);
  digitalWrite(BuzzerPin, LOW);
  delay(100);
  }
  float temp_hs_pitch=0,temp_TO_pitch=0;Stand.Threshold_pitch=-5;
  float temp_accl_magData=0;
  int HS_loopCnt=1,TO_loopCnt=1,Accl_loopCnt=1;
  float temp_heelStrike_data=0,temp_TO_data=0,temp_accl_data=0;
  unsigned long Calib_time=millis();
  unsigned long new_tick=Calib_time+1030;
 while(((millis()-Calib_time)< 10000)){
  dataRead();
  if((millis()-Calib_time)< 1000)      //Please stand for 5 second
  {
   Stand.Threshold_pitch=currentData.pitch;
  }
  else if ( millis()-new_tick>100) 
  {
    if (currentData.pitch<Stand.Threshold_pitch-8 && currentData.pitch < temp_heelStrike_data )
    {
      HS_loopCnt+=1;
      temp_hs_pitch+=currentData.pitch;
    }
    if (currentData.pitch>Stand.Threshold_pitch+15 && currentData.pitch > temp_TO_data)
    {
      TO_loopCnt+=1;
      temp_TO_pitch+=currentData.pitch;
    }
    if (Mangnetude.accl>temp_accl_data && Mangnetude.accl>5)
    {
      Accl_loopCnt+=1;
      temp_accl_magData+=Mangnetude.accl;
    }
  }
  temp_heelStrike_data=currentData.pitch;
  temp_TO_data=currentData.pitch;
  temp_accl_data=Mangnetude.accl;
  Serial.println("calibrating");
  Serial.println("");
}
HS.Threshold_pitch=temp_hs_pitch/HS_loopCnt;                 // Toe off
TO.Threshold_pitch=temp_TO_pitch/TO_loopCnt;                // Heel strike
Mangnetude.Thresold_accl=temp_accl_magData/Accl_loopCnt;    // Heel strike accl
save_data();
}
void Green_area()
{
    if ((currentData.pitch < (HS.Threshold_pitch+5)) || (currentData.pitch >(TO.Threshold_pitch-5)))
    {
      Green_zone=1;ST_zone=0;count.GT=0;
        // Serial.println("check1");
    }
    if ((currentData.pitch>((HS.Threshold_pitch)) && currentData.pitch<(TO.Threshold_pitch)) && ST_zone==0)
    {
      ST_zone=1;  Recordtime.ST_Window=millis();
      // Serial.println("check2");
    }

    if ((millis()-Recordtime.ST_Window>5000)&&ST_zone==1)
    { 
      // Serial.println("check3");
      if ((Mangnetude.accl<5 && Mangnetude.accl>=1))
    {
      count.GT+=1;
      Serial.println("Count GT: "+String(count.GT));
      if (count.GT>70){
      for (int i =0; i<3; i++){SOS();}
      count.GT=0;}
      }
    }
}
void ReadTime(void)
{
  Recordtime.HS[Recordtime.Array]=millis();
  if (Recordtime.Array > 0) { 
    Recordtime.Delta_HS=(Recordtime.HS[Recordtime.Array]-Recordtime.HS[Recordtime.Array-1]);
    Serial.println("Stride time: " + String(Recordtime.Delta_HS/ 1000.0f));
    } else {
      Serial.println("Heel strike time: " + String(Recordtime.HS[Recordtime.Array]));
    }
    Recordtime.Array++;
    if (Recordtime.Array >= 100) Recordtime.Array = 0;  // Optional: wrap around if array is full
}
void HeelStrike(void)
{
  Recordtime.vib_time=Recordtime.watching_you-Recordtime.ACK_time;
  if (currentData.pitch>(TO.Threshold_pitch-2) && Mangnetude.accl > Mangnetude.Thresold_accl && Green_zone==1)
{
  slave_flag.HS=1;
  ReadTime();
  digitalWrite(BuzzerPin, HIGH);
  delay(200);
  digitalWrite(BuzzerPin, LOW);
  Green_zone = 0;
}
// Serial.println(Recordtime.watching_you);
if (Recordtime.vib_time>Recordtime.HS_delay&& count.vib<=(count.totalvib-1))
  {
    count.vib+=1;
    // Serial.println(("watch time:")+String(Recordtime.watching_you-Recordtime.ACK_time));
   SOS();
  }
}
void SOS(void)
{
   digitalWrite(Motor1, HIGH);
    delay(vib_time);
    digitalWrite(Motor1, LOW);
    delay(vib_gap_time);
}
void data_manipulation()
{
manipulate.roll=currentData.roll*100;
manipulate.pitch=currentData.pitch*100;
manipulate.yaw=currentData.yaw*100;
manipulate.Accl=Mangnetude.accl*100;
}