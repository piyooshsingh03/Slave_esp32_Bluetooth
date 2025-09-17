#ifndef MEMORY_H
#define MEMORY_H
#include<EEPROM.h>
#define EEPROM_SIZE 64

#define HS_Threshold_pitch_Address    0
#define TO_Threshold_pitch_Address   (HS_Threshold_pitch_Address + sizeof(float))
#define Threshold_accl_Address       (TO_Threshold_pitch_Address + sizeof(float))
#define ST_Threshold_pitch_Address   (Threshold_accl_Address + sizeof(float))
#define vib_time_Address             (ST_Threshold_pitch_Address + sizeof(int))
#define vib_timegap_Address          (vib_time_Address + sizeof(int))
#define Total_vib_Address            (vib_timegap_Address + sizeof(int))
#define Freez_time_Address            (Total_vib_Address + sizeof(int))

void save_data(void);
void Read_data(void);
void erase_memory(void);
void Write_config_data(void);
void Read_config_data(void);
#endif