
// const char* wifiSsid = "OvenControllerNetwork";
// const char* wifiPassword = "password";

// void wifi_task(void *arg)
// {
//   nvs_flash_init();
//   // Set web server port number to 80
//   WiFiServer server(80);

//   // Variable to store the HTTP request
//   String header;

//   // Current time
//   unsigned long currentTime = millis();
//   // Previous time
//   unsigned long previousTime = 0;
//   // Define timeout time in milliseconds (example: 2000ms = 2s)
//   const long timeoutTime = 2000;

//   printf("[WiFi_Task] Beginning \n");
//   WiFi.softAP(wifiSsid);
//   IPAddress IP = WiFi.softAPIP();
//   printf("[WiFi_Task]: AP IP address");
//   Serial.println(IP);
//   // while (WiFi.status() != WL_CONNECTED) {
//   //   vTaskDelay(500/portTICK_RATE_MS);
//   //   printf(".");
//   // }
//   server.begin();
//   printf("[WiFi_Task] Initialized!\n");

//   while(1){
//     WiFiClient client = server.available();   // Listen for incoming clients

//     if (client) {                             // If a new client connects,
//       currentTime = millis();
//       previousTime = currentTime;
//       printf("[Wifi_Task] Client connected\n");    // print a message out in the serial port
//       String currentLine = "";                // make a String to hold incoming data from the client
//       while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
//         currentTime = millis();
//         if (client.available()) {             // if there's bytes to read from the client,
//           char c = client.read();             // read a byte, then
//           Serial.write(c);                    // print it out the serial monitor
//           header += c;
//           if (c == '\n') {                    // if the byte is a newline character
//             // if the current line is blank, you got two newline characters in a row.
//             // that's the end of the client HTTP request, so send a response:
//             if (currentLine.length() == 0) {
//               // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//               // and a content-type so the client knows what's coming, then a blank line:
//               client.println("HTTP/1.1 200 OK");
//               client.println("Content-type:text/html");
//               client.println("Connection: close");
//               client.println();
//               client.println("<html><body><h1>ESP32 Web Server</h1></body></html>");
//               client.println();
//               // Clear the header variable
//               // Close the connection
//               client.stop();
//               printf("[Wifi_Task] Client disconnected. \n\n");

//               // turns the GPIOs on and off
//               if (header.indexOf("GET /on") >= 0) {
//                 printf("[Wifi_Task] Turning ON LED\n");
//                 turnOn(NULL);
//               } else if (header.indexOf("GET /off") >= 0) {
//                 printf("[Wifi_Task] Turning OFF LED\n");
//                 turnOff(NULL);
//               }

//               header = "";

//               break;
//             } else { // if you got a newline, then clear currentLine
//               currentLine = "";
//             }
//           } else if (c != '\r') {  // if you got anything else but a carriage return character,
//             currentLine += c;      // add it to the end of the currentLine
//           }
//         }
//       }


//     }
//   }
// }
