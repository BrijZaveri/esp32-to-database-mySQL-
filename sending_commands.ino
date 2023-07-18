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
  delay(250);
}

void send_hardware_serial_number_query()                                       //for getting hardware serial number
{
  CAN.beginExtendedPacket(0xA79090D);                // SOC of Total Voltage
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
  delay(10);
}
