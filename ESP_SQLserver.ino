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
byte a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40; //for 16 cell voltages

int CellVoltage1;
int CellVoltage2;
int CellVoltage3;
int CellVoltage4;
int CellVoltage5;
int CellVoltage6;
int CellVoltage7;
int CellVoltage8;
int CellVoltage9;
int CellVoltage10;
int CellVoltage11;
int CellVoltage12;
int CellVoltage13;
int CellVoltage14;
int CellVoltage15;
int CellVoltage16;
int CellVoltage17;
int CellVoltage18;
int CellVoltage19;
int CellVoltage20;
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
//const char* ssid = "Z";  //
//const char* password = "123321123";
//WiFiClient client;
char server[] = "192.168.1.11";  //eg: 192.168.0.222
String combinedString = ""; // Variable to store the combined substring
char firstByte;

WiFiClient client;

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
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
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  


  // Start the server
  //  server.begin();
  Serial.println("Server started");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
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
    CANSender_all();                //sender
        send_hardware_serial_number_query();   //sender
    Sending_To_phpmyadmindatabase();
    //    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void Task2code(void* pvParameters) {

  for (;;) {
    all_battery_params();               //receiver
//        Hardware_Serial_number();
//        getBatterySerialNumber();           //receiver
  }
}
