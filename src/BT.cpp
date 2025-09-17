// #include<BT.h>
// #include<imu.h>
// BluetoothSerial SerialBT;     // Declare, do not define
// void Handle_Bluetooth(void)
// {
//     if (SerialBT.available()) {
//     String received = SerialBT.readStringUntil('\n');

//     int sepIndex = received.indexOf(':');
//     if (sepIndex != -1) {
//       String flag = received.substring(0, sepIndex);
//       master_flag.vibration = received.substring(sepIndex + 1).toInt();

//       if (flag == "Master_HS_flag") {
//        Recordtime.ACK_time=millis();
//       Serial.println("Master HS_flag : " + String(master_flag.vibration));
//       }
//     }
//   }

// //   if (SerialBT.hasClient()) {
// //     if (flag.HS==1){SerialBT.println("HS_flag:"+String(flag.HS));flag.HS=0;}  
// //   }
// //   else{
// //     Serial.println("Disconnected. Trying to reconnect...");
// //   }

//   if (SerialBT.connected()) {
//     if (slave_flag.HS==1){slave_flag.vibration=1;
//       SerialBT.println("Slabve_HS_flag:"+String(slave_flag.HS));slave_flag.HS=0;
//       SerialBT.println("Vib_flag:"+String(slave_flag.vibration));slave_flag.vibration=0;}  
//   } else {
//     Serial.println("Disconnected. Trying to reconnect...");
//   }
// }