#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_SLAVE");  // Slave mode
  Serial.println("Slave ready. Waiting for master...");
}

void loop() {
  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');

    int sepIndex = received.indexOf(':');
    if (sepIndex != -1) {
      String flag = received.substring(0, sepIndex);
      int slaveack = received.substring(sepIndex + 1).toInt();

      if (flag == "CMD1") {
        Serial.println("Motor speed set to " + String(slaveack));
      }
    }
  }

  if (SerialBT.hasClient()) {
    SerialBT.println("CMD1:123"); // Only send if client is connected
    delay(500);
  }
}
