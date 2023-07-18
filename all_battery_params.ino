void all_battery_params() {

  Counter = 0;
  int packetSize = CAN.parsePacket();
  
//************************************************************VOLTAGE & CURRENT**********************************************
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

//**********************************************************************TEMPERATURE****************************************

      if (CAN.packetId() == 241962249)  // Green Tiger Battery Temp1,2,3,4,5
      {
        while (CAN.available()) {
          Stored = CAN.read();
          Counter++;

          if (Counter == 1) {
            t1 = Stored;
          }

          if (Counter == 2) {
            t2 = Stored;
          }

          if (Counter == 3) {
            t3 = Stored;
          }

          if (Counter == 4) {
            t4 = Stored;
          }

          if (Counter == 5) {
            t5 = Stored;
          }
        }
      }
      
//****************************************************************SOC***********************************************************

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
//****************************************************************HARDWARE NUMBER***************************************************
  byte hardwarenumber[8];
//String combinedString = ""; // Variable to store the combined substring

if (CAN.packetId() == 175705353)  // Green Tiger Serialnumber
{
  while (CAN.available()) {
    byte Stored = CAN.read();
    hardwarenumber[Counter] = Stored;
    Counter++;

    if (Counter == 8) {
      for (int i = 0; i < sizeof(hardwarenumber) / sizeof(hardwarenumber[0]); i++) {
        char asciiChar = (char)hardwarenumber[i];
        combinedString += asciiChar;
      }
      Counter=0;
//      Serial.print("BATTERY SERIAL NUMBER = ");
//      Serial.println(combinedString);
//      Serial.println(combinedString.substring(0, 8));
    }
  }
}


//*********************************************************CELL VOLTAGE 1 to 4*********************************************
      if (CAN.packetId() == 241437961)                                        // Green Tiger Battery 4 Cell voltages
      {
        while (CAN.available())
        {
          Stored =  CAN.read();
          Counter++;
          if (Counter == 1)
          {
            a0 = Stored;
          }

          if (Counter == 2)
          {
            a1 = Stored;
            CellVoltage1 = (a0 << 8) + a1;
          }

          if (Counter == 3)
          {
            a2 = Stored;
          }

          if (Counter == 4)
          {
            a3 = Stored;
            CellVoltage2 = (a2 << 8) + a3;
          }
          if (Counter == 5)
          {
            a4 = Stored;
          }
          if (Counter == 6)
          {
            a5 = Stored;
            CellVoltage3 = (a4 << 8) + a5;
          }
          if (Counter == 7)
          {
            a6 = Stored;
          }
          if (Counter == 8)
          {
            a7 = Stored;
            CellVoltage4 = (a6 << 8) + a7;
          }
        }
      }

//**********************************************************CELL VOLTAGE 5 to 8*********************************************

      if (CAN.packetId() == 241503497)                  // Green Tiger Battery 4 Cell voltages
      {
        while (CAN.available())
        {
          Stored =  CAN.read();
          Counter++;
          if (Counter == 1)
          {
            a8 = Stored;
          }

          if (Counter == 2)
          {
            a9 = Stored;
            CellVoltage5 = (a8 << 8) + a9;
          }

          if (Counter == 3)
          {
            a10 = Stored;
          }

          if (Counter == 4)
          {
            a11 = Stored;
            CellVoltage6 = (a10 << 8) + a11;
          }
          if (Counter == 5)
          {
            a12 = Stored;
          }
          if (Counter == 6)
          {
            a13 = Stored;
            CellVoltage7 = (a12 << 8) + a13;
          }
          if (Counter == 7)
          {
            a14 = Stored;
          }
          if (Counter == 8)
          {
            a15 = Stored;
            CellVoltage8 = (a14 << 8) + a15;
          }
        }
      }

//*******************************************************CELL VOLTAGE 9 to 12*********************************************

      if (CAN.packetId() == 241569033)                  // Green Tiger Battery 4 Cell voltages
      {
        while (CAN.available())
        {
          Stored =  CAN.read();
          Counter++;
          if (Counter == 1)
          {
            a16 = Stored;
          }

          if (Counter == 2)
          {
            a17 = Stored;
            CellVoltage9 = (a16 << 8) + a17;
          }

          if (Counter == 3)
          {
            a18 = Stored;
          }

          if (Counter == 4)
          {
            a19 = Stored;
            CellVoltage10 = (a18 << 8) + a19;
          }
          if (Counter == 5)
          {
            a20 = Stored;
          }
          if (Counter == 6)
          {
            a21 = Stored;
            CellVoltage11 = (a20 << 8) + a21;
          }
          if (Counter == 7)
          {
            a22 = Stored;
          }
          if (Counter == 8)
          {
            a23 = Stored;
            CellVoltage12 = (a22 << 8) + a23;
          }
        }
      }

//*******************************************************CELL VOLTAGE 13 to 16*********************************************

      if (CAN.packetId() == 241634569)                  // Green Tiger Battery 4 Cell voltages
      {
        while (CAN.available())
        {
          Stored =  CAN.read();
          Counter++;
          if (Counter == 1)
          {
            a24 = Stored;
          }

          if (Counter == 2)
          {
            a25 = Stored;
            CellVoltage13 = (a24 << 8) + a25;
          }

          if (Counter == 3)
          {
            a27 = Stored;
          }

          if (Counter == 4)
          {
            a28 = Stored;
            CellVoltage14 = (a27 << 8) + a28;
          }
          if (Counter == 5)
          {
            a29 = Stored;
          }
          if (Counter == 6)
          {
            a30 = Stored;
            CellVoltage15 = (a29 << 8) + a30;
          }
          if (Counter == 7)
          {
            a31 = Stored;
          }
          if (Counter == 8)
          {
            a32 = Stored;
            CellVoltage16 = (a31 << 8) + a32;
          }
        }
      }

//**********************************************************CELL VOLTAGE 17 to 20*********************************************

      if (CAN.packetId() == 241700105)                  // Green Tiger Battery Cell voltages
      {
        while (CAN.available())
        {
          Stored =  CAN.read();
          Counter++;
          if (Counter == 1)
          {
            a33 = Stored;
          }

          if (Counter == 2)
          {
            a34 = Stored;
            CellVoltage17 = (a33 << 8) + a34;
          }

          if (Counter == 3)
          {
            a35 = Stored;
          }

          if (Counter == 4)
          {
            a36 = Stored;
            CellVoltage18 = (a35 << 8) + a36;
          }
          if (Counter == 5)
          {
            a37 = Stored;
          }
          if (Counter == 6)
          {
            a38 = Stored;
            CellVoltage19 = (a37 << 8) + a38;
          }
          if (Counter == 7)
          {
            a39 = Stored;
          }
          if (Counter == 8)
          {
            a40 = Stored;
            CellVoltage20 = (a39 << 8) + a40;
          }
        }
      }
     
    }
    // delay(1000);
  }
}
