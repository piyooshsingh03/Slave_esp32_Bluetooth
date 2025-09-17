#include<memory.h>
#include<imu.h>
void save_data(void)
{  
   erase_memory();
  EEPROM.put(HS_Threshold_pitch_Address, HS.Threshold_pitch);  
  EEPROM.put(TO_Threshold_pitch_Address, TO.Threshold_pitch);
  EEPROM.put(Threshold_accl_Address, Mangnetude.Thresold_accl);
  EEPROM.put(ST_Threshold_pitch_Address, Stand.Threshold_pitch);
  EEPROM.commit();   // save to flash
  Serial.println(HS.Threshold_pitch);
  Serial.println(TO.Threshold_pitch);
  Serial.println(Mangnetude.Thresold_accl);
  Serial.println(Stand.Threshold_pitch);
}

void Write_config_data(void)
{
  if (vib_time>500){
  EEPROM.put(vib_time_Address, vib_time);
  EEPROM.put(vib_timegap_Address,vib_gap_time);
  EEPROM.commit();   // save to flash
  }
    EEPROM.put(vib_time_Address, vib_time);
    EEPROM.put(vib_timegap_Address,vib_gap_time);
    if (count.totalvib>10){count.totalvib=3;}
    EEPROM.put(Total_vib_Address, count.totalvib);
    EEPROM.put(Freez_time_Address, Recordtime.HS_delay);
    EEPROM.commit();   // save to flash
}

void Read_config_data(void)
{
  EEPROM.get(vib_time_Address, vib_time);        // not prited
  if(vib_time<500){vib_time=500;}
  EEPROM.get(vib_timegap_Address, vib_gap_time); // not prited
  if(vib_gap_time<500){vib_gap_time=500;}
  EEPROM.get(Total_vib_Address, count.totalvib);
  if(count.totalvib<1 || count.totalvib>10){count.totalvib=3;}
  EEPROM.get(Freez_time_Address, Recordtime.HS_delay);
  if(Recordtime.HS_delay<2000){Recordtime.HS_delay=2000;}
  Serial.println("vib Time: " + String(vib_time));
  Serial.println("vib Time gap: " + String(vib_gap_time));
  Serial.println("Number of vib: " + String(count.totalvib));
  Serial.println("Freezing time: " + String(Recordtime.HS_delay));
}

void Read_data(void)
{

// Read from EEPROM
EEPROM.get(HS_Threshold_pitch_Address, HS.Threshold_pitch);
EEPROM.get(TO_Threshold_pitch_Address, TO.Threshold_pitch);
EEPROM.get(Threshold_accl_Address, Mangnetude.Thresold_accl);
EEPROM.get(ST_Threshold_pitch_Address, Stand.Threshold_pitch);
// Serial.println(HS.Threshold_pitch);
// Serial.println(TO.Threshold_pitch);
// Serial.println(Mangnetude.Thresold_accl);
// Serial.println(Stand.Threshold_pitch);
if(Mangnetude.accl<5){Mangnetude.Thresold_accl;}
if(currentData.pitch>20 || currentData.pitch<-10 ){TO.Threshold_pitch=5;}
snprintf(sentdata, sizeof(sentdata), "%.2f,%.2f,%.2f,%.2f", HS.Threshold_pitch, TO.Threshold_pitch, Mangnetude.Thresold_accl, Stand.Threshold_pitch);

}
void erase_memory(void)
{
    for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}