TaskHandle_t Task1;
TaskHandle_t Task2;
#include <CAN.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

String substring = "";

byte Counter = 0;
byte Stored;
float Voltage;
float Current;
int Speed = 0;
int RPM;
byte c0, c1, c2, c3, c4, c5;  //for voltage, current, remaining Ah
byte t0, t1, t2, t3, t4, t5;  //for temperature
byte s1, s2;                  //for SOC
byte b0, b1, b2, b3, b4, b5;
unsigned int h1, h2, h3, h4, h5, h6;
unsigned long h7;
String asciiString = ""; // String to store the ASCII characters
//  int asciiLength = asciiString.length(); // Determine the length of the string
byte noCAN = 0;
int Seconds;
int SOC;
long decimalValue;
// long SerialNumber;
int remainingAh;

const char* ssid = "WARDJOY";  //
const char* password = "WaRd@2023#";
//WiFiClient client;
char server[] = "192.168.1.11";  //eg: 192.168.0.222

WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(10);
  if (!CAN.begin(250E3)) {
    Serial.println("Starting CAN failed!");
    while (1)
      ;
  }
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  //  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");


  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */
  delay(500);

  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */
  delay(500);
}

void loop() {
}

void Task1code(void* pvParameters) {

  for (;;) {
    CANSender_all();
//    send_hardware_serial_number_query();
    Sending_To_phpmyadmindatabase();
    //    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void Task2code(void* pvParameters) {

  for (;;) {
    all_battery_params();
//    getBatterySerialNumber();
  }
}

void CANSender_all() {
  CAN.beginExtendedPacket(0x0E64090D);  // SOC of Total Voltage
  CAN.write(00);
  CAN.write(00);
  CAN.write(00);
  CAN.write(00);
  CAN.write(00);
  CAN.write(00);
  CAN.write(00);
  CAN.write(00);
  CAN.endPacket();
  //  Serial.println("done");
  delay(1000);
}

//void send_hardware_serial_number_query()                                       //for getting hardware serial number
//{
//  CAN.beginExtendedPacket(0xA77090D);                // SOC of Total Voltage
//  CAN.write(00);
//  CAN.write(00);
//  CAN.write(00);
//  CAN.write(00);
//  CAN.write(00);
//  CAN.write(00);
//  CAN.write(00);
//  CAN.write(00);
//  CAN.endPacket();
//  //  Serial.println("done");
//  //  delay(1);
//}

void all_battery_params() {

  Counter = 0;
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    if (CAN.packetExtended()) {
      if (CAN.packetId() == 174918921)  // Green Tiger Battery Voltage & Current
      {
        while (CAN.available()) {
          Stored = CAN.read();
          Counter++;
          if (Counter == 1) {
            c0 = Stored;
          }

          if (Counter == 2) {
            c1 = Stored;

            Voltage = (c0 << 8) + c1;
            // Serial.print("Voltage = ");
            // Serial.print(Voltage * 0.1);
            // Serial.print("%");
            Serial.println();
          }

          if (Counter == 3) {
            c2 = Stored;
          }

          if (Counter == 4) {
            c3 = Stored;
            Current = (c2 << 8) + c3;
            //           if ( Current >= 65535 )
            //           {
            //           Serial.print("Current = ");
            //           Serial.print((65535 - Current)*0.1);
            //           Serial.print("%");
            //           Serial.println();
            //           }
            //           else
            //           {
            //           Serial.print("Current = ");
            //           Serial.print( Current * 0.1 );
            //           Serial.print("%");
            //           Serial.println();

            //           Serial.print("Power = ");
            //           Serial.print((Current * 0.1 ) * ( Voltage * 0.1 ));
            //           Serial.println();
          }
          if (Counter == 5) {
            c4 = Stored;
          }
          if (Counter == 6) {
            c5 = Stored;
            remainingAh = (((c4 << 8) + c5) / 10);
          }
        }
      }

      if (CAN.packetId() == 241962249)  // Green Tiger Battery Temp1,2,3,4,5
      {
        while (CAN.available()) {
          Stored = CAN.read();
          Counter++;

          if (Counter == 1) {
            t1 = Stored;
            // Serial.print("Temp 1 = ");
            // Serial.print(t1);
            // Serial.println();
          }

          if (Counter == 2) {
            t2 = Stored;
            // Serial.print("Temp 2 = ");
            // Serial.print(t2);
            // Serial.println();
          }

          if (Counter == 3) {
            t3 = Stored;
            // Serial.print("Temp 3 = ");
            // Serial.print(t3);
            // Serial.println();
          }

          if (Counter == 4) {
            t4 = Stored;
            // Serial.print("Temp 4 = ");
            // Serial.print(t4);
            // Serial.println();
          }

          if (Counter == 5) {
            t5 = Stored;
            // Serial.print("Temp 5 = ");
            // Serial.print(t5);
            // Serial.println();
          }
        }
      }


      if (CAN.packetId() == 174984457)  // Green Tiger SOC
      {
        while (CAN.available()) {
          Stored = CAN.read();
          Counter++;

          if (Counter == 1) {
            s1 = Stored;
          }

          if (Counter == 2) {
            s2 = Stored;
            SOC = (s1 << 8) + s2;
            Serial.println();
          }
        }
      }
      //      if (CAN.packetId() == 175443209)  // Green Tiger Serialnumber
      //      {
      //        while (CAN.available()) {
      //          byte Stored = CAN.read();
      //          Counter++;
      //
      //          if (Counter == 1) {
      //            h1 = Stored;
      //            //            Serial.print("h1=");
      //            //            Serial.print(h1);
      //          }
      //          if (Counter == 2) {
      //            h2 = Stored;
      //            //            Serial.print("h2=");
      //            //            Serial.print(h2);
      //          }
      //          if (Counter == 3) {
      //            h3 = Stored;
      //            //            Serial.print("h3=");
      //            //            Serial.print(h3);
      //          }
      //          if (Counter == 4) {
      //            h4 = Stored;
      //            //            Serial.print("h4=");
      //            //            Serial.print(h4);
      //          }
      //          if (Counter == 5) {
      //            h5 = Stored;
      //            //            Serial.print("h5=");
      //            //            Serial.print(h5);
      //
      //            int decimalValues[] = {h1, h2, h3, h4, h5 }; // Decimal values to convert
      //            //            String asciiString = ""; // String to store the ASCII characters
      //
      //            int asciiLength = asciiString.length(); // Determine the length of the string
      //            String substring = asciiString.substring(0, 5); // Extract the first 5 characters
      //
      //            // Convert decimal values to ASCII characters and concatenate them into the string
      //            for (int i = 0; i < sizeof(decimalValues) / sizeof(decimalValues[0]); i++) {
      //              char asciiChar = (char)decimalValues[i]; // Convert decimal value to ASCII character
      //              asciiString += asciiChar; // Concatenate the character to the string
      //            }
      //
      //            // Print the resulting string
      //            Serial.print("BATTERY SERIAL NUMBER =");
      //            Serial.println(substring);
      //            Serial.print("asciiLength = ");
      //            Serial.println(asciiLength);
      //          }
      //        }
      //      }
    }
    // delay(1000);
  }
}

//void Hardware_Serial_number()
//{
//  byte hardwarenumber[8];
//  int Counter = 0;
//  int packetSize = CAN.parsePacket();
//
//  if (packetSize)
//  {
//    if (CAN.packetExtended())
//    {
//      if (CAN.packetId() == 175574281)  // Green Tiger Serialnumber
//      {
//        while (CAN.available()) {
//          byte Stored = CAN.read();
//          hardwarenumber[Counter] = Stored;
//          Counter++;
//
//          if (Counter == 8) {
//            String asciiString = "";
//
//            for (int i = 0; i < sizeof(hardwarenumber) / sizeof(hardwarenumber[0]); i++) {
//              char asciiChar = (char)hardwarenumber[i];
//              asciiString += asciiChar;
//            }
//
//            String substring = asciiString.substring(0, 7);
//            Serial.print("BATTERY SERIAL NUMBER = ");
//            Serial.println(substring);
//          }
//        }
//        delay(1);
//      }
//    }
//  }
//}

//void getBatterySerialNumber() {
//  byte hardwarenumber[8];
//  int Counter = 0;
//  int packetSize = CAN.parsePacket();
//
//  if (packetSize)
//  {
//    if (CAN.packetExtended())
//    {
//      if (CAN.packetId() == 175574281)  // Green Tiger Serialnumber
//      {
//        while (CAN.available()) {
//          byte Stored = CAN.read();
//          hardwarenumber[Counter] = Stored;
//          Counter++;
//
//          if (Counter == 8) {
//            String asciiString = "";
//
//            for (int i = 0; i < sizeof(hardwarenumber) / sizeof(hardwarenumber[0]); i++) {
//              char asciiChar = (char)hardwarenumber[i];
//              asciiString += asciiChar;
//            }
//
//            substring = asciiString.substring(0, 7);
//            Serial.print("BATTERY SERIAL NUMBER = ");
//            Serial.println(substring);
//          }
//        }
//        delay(1);
//      }
//    }
//  }
//}

void Sending_To_phpmyadmindatabase() {

  substring = asciiString.substring(0, 7);

  if (client.connect(server, 8070)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /Battery_data_logger.php?id=");
    client.print("GET /Battery_data_logger.php?id=");  //YOUR URL
    Serial.println("DL_01");
    client.print("DL_01");
    Serial.print("&voltage=");
    client.print("&voltage=");  //YOUR URL
    Serial.println(Voltage * 0.1);
    client.print(Voltage * 0.1);
    client.print("&current=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&current=");
    Serial.println(Current * 0.1);
    client.print(Current * 0.1);
    client.print("&soc=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&soc=");
    Serial.println(SOC * 0.1);
    client.print(SOC * 0.1);
    client.print("&remainingAh=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&remainingAh=");
    Serial.println(remainingAh);
    client.print(remainingAh);
    client.print("&temp1=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&temp1=");
    Serial.println(t1);
    client.print(t1);
    client.print("&temp2=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&temp2=");
    Serial.println(t2);
    client.print(t2);
    client.print("&temp3=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&temp3=");
    Serial.println(t3);
    client.print(t3);
    client.print("&temp4=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&temp4=");
    Serial.println(t4);
    client.print(t4);
    client.print("&temp5=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&temp5=");
    Serial.println(t5);
    client.print(t5);
    client.print("&temp5=");  //ADD THIS LINE TO SEPARATE THE TWO VALUES
    Serial.print("&temp5=");
    Serial.println(t5);
    client.print(t5);
//    Serial.print("&serialnumber=");
//    client.print("&serialnumber=");
//    Serial.println(substring);
//    client.print(substring);
    client.print(" HTTP/1.1");  //MOVE THIS LINE HERE TO CLOSE THE REQUEST PROPERLY
    client.println();
    client.println("Host: 192.168.1.11");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  // delay(1000);
}
