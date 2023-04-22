TaskHandle_t Task1;
TaskHandle_t Task2;
#include <CAN.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

byte Counter = 0;               
byte Stored;                    
float Voltage; 
float Current; 
int Speed = 0;  
int RPM;                  
byte c0,c1,c2,c3,c4,c5;          //for voltage, current, remaining Ah
byte t0,t1,t2,t3,t4,t5;   //for temperature
byte s1,s2;                //for SOC
byte b0,b1,b2,b3,b4,b5;
byte noCAN = 0;
int Seconds;  
int SOC;
int remainingAh;    

const char* ssid = "HO-WIFI";// 
const char* password = "Admin@123";
//WiFiClient client;
char server[] = "192.168.1.9";   //eg: 192.168.0.222


WiFiClient client;    


void setup()
{
 Serial.begin(9600);
  delay(10);
  if (!CAN.begin(250E3))
  {
    Serial.println("Starting CAN failed!");
    while (1);
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
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500);
 }

void loop()
{ 

 }

 void Task1code( void * pvParameters )
{
//  Serial.print("Task1000 running on core ");
//  Serial.println(xPortGetCoreID());

  for(;;)
  {
    CANSender();
    Sending_To_phpmyadmindatabase(); 
    
//    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void Task2code( void * pvParameters ){
//  Serial.print("Task2 running on core ");
//  Serial.println(xPortGetCoreID());

  for(;;)
  {
    
    CANReceiver();
      

//    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability

  }
}

void CANSender()
{
  CAN.beginExtendedPacket(0x0E64090D);                // SOC of Total Voltage
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

void CANReceiver()
{
  
  Counter = 0;
  int packetSize = CAN.parsePacket();
  
  if (packetSize)
  {
    if (CAN.packetExtended()) 
    {
      if(CAN.packetId() == 174918921)                   // Green Tiger Battery Voltage & Current
      {
        while (CAN.available()) 
        {
          Stored =  CAN.read();
          Counter++;
          if(Counter ==1)
          {
           c0 = Stored;                                     
          }
  
          if(Counter ==2)
          {
           c1 = Stored;
          
          Voltage = (c0<<8) + c1;
          Serial.print("Voltage = ");
          Serial.print(Voltage * 0.1);                          
          Serial.print("%");
          Serial.println();
          }

          if(Counter ==3)
          {
           c2 = Stored;                                    
          }

          if(Counter ==4)
          {
           c3 = Stored;
           Current = (c2<<8) + c3;
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
           if(Counter ==5)
          {
           c4 = Stored;
          }
          if(Counter ==6)
          {
           c5 = Stored;
           remainingAh = (c4<<8) + c5;
        }
        }
      }
      
      if(CAN.packetId() == 241962249)                       // Green Tiger Battery Temp1,2,3,4,5
    {
      while (CAN.available()) 
      {
        Stored =  CAN.read();
        Counter++;
        
           if(Counter ==1)
             {
              t1 = Stored;
             Serial.print("Temp 1 = ");
             Serial.print(t1);
             Serial.println();
             }
    
           if(Counter ==2)
             {
              t2 = Stored;
             Serial.print("Temp 2 = ");
             Serial.print(t2);
             Serial.println();
             }
      
           if(Counter ==3)
             {
              t3 = Stored;
             Serial.print("Temp 3 = ");
             Serial.print(t3);
             Serial.println();
             }
    
           if(Counter ==4)
             {
              t4 = Stored;
             Serial.print("Temp 4 = ");
             Serial.print(t4);
             Serial.println();
             }
    
           if(Counter ==5)
             {
              t5 = Stored;
             Serial.print("Temp 5 = ");
             Serial.print(t5);
             Serial.println();
             }
           }
         }        
       }
     }
  }

 void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /testcode/battery2.php?voltage=");
    client.print("GET /testcode/battery2.php?voltage=");     //YOUR URL
    Serial.println(Voltage*0.1);
    client.print(Voltage*0.1);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.1.9");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }