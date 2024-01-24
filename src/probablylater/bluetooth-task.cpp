
// #include "BluetoothSerial.h"
// #include "./ui-task.h"

// BluetoothSerial SerialBT;
// byte BTData;

// void bluetooth_task(void *arg)
// {
//   #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//   #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//   #endif
//   SerialBT.begin("ESP32test");
//   Serial.println("The device started, now you can pair it with bluetooth!");
//   while(1){
//     if (SerialBT.available())
//     {
//       BTData = SerialBT.read();
//       Serial.write(BTData);
//     }

//     if (BTData == '1')
//     {
//       turnOn(NULL);
//     }

//     if (BTData == '0')
//     {
//       turnOff(NULL);
//     }
//   }
// }
