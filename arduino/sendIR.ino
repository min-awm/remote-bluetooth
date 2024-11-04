#include <IRremote.h>

const int IR_SEND_PIN = 18;     // Chân GPIO phát tín hiệu IR (LED IR)


void setup() {
  Serial.begin(115200);
  // pinMode(IR_SEND_PIN, OUTPUT);  // Thiết lập chân phát IR (LED IR) là đầu ra
  IrSender.begin(IR_SEND_PIN ,ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);   // Khởi động IR LED để phát tín hiệu

}


void loop() {
  uint64_t tRawData1[]={0x20402000E0B8C3, 0xE005000000};
  IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4550, 550, 1700, 550, 550, &tRawData1[0], 104, PROTOCOL_IS_LSB_FIRST, 50, 3);
  Serial.println(F("Da gui 1"));
  delay(5000);
  uint64_t tRawData[]={0x20402000E0B8C3, 0x5002000};
  IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4550, 550, 1700, 550, 550, &tRawData[0], 104, PROTOCOL_IS_LSB_FIRST, 50, 3);
  Serial.println(F("Da gui 2"));
  delay(5000);
}