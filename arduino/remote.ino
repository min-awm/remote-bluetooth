#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <IRremote.h>

// UUID của dịch vụ và đặc tính
#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "abcd1234-1234-1234-1234-123456789abc"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

const int IR_SEND_PIN = 18;     // Chân GPIO phát tín hiệu IR (LED IR)
const int IR_RECEIVE_PIN = 4;  // Chân GPIO nhận tín hiệu IR (cảm biến IR 1838)
bool listeningIR = false;


// Callback để theo dõi kết nối
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;

    // Bắt đầu quảng bá lại khi ngắt kết nối
    pServer->getAdvertising()->start();
    // Serial.println("Start advertising again after disconnect.");
  }
};

// Callback để nhận dữ liệu từ client
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    String value = pCharacteristic->getValue().c_str();

    if (value.length() > 0 && deviceConnected) {
      if (value == "listenIR") {
        listeningIR = true;
      } else {
        // uint32_t irCommand = (uint32_t) strtol(value.c_str(), NULL, 16);
        // IrSender.sendNEC(irCommand, 32);  // Phát tín hiệu NEC
        // Serial.println(irCommand, HEX);
      }

      // Serial.print("Received Value: ");
      // Serial.println(value);
    }
  }
};

void setup() {
  // Serial.begin(115200);

  // pinMode(IR_SEND_PIN, OUTPUT);  // Thiết lập chân phát IR (LED IR) là đầu ra
  // IrSender.begin(IR_SEND_PIN);   // Khởi động IR LED để phát tín hiệu
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  // Khởi động cảm biến IR để nhận tín hiệu

  // Khởi tạo BLE
  BLEDevice::init("MIN_REMOTE_IR");

  // Tạo server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Tạo dịch vụ BLE
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Tạo đặc tính BLE (gửi/nhận dữ liệu)
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  // Đặt callback để xử lý khi có dữ liệu được ghi từ client
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Bắt đầu dịch vụ
  pService->start();

  // Bắt đầu quảng bá BLE
  pServer->getAdvertising()->start();
  // Serial.println("Waiting for a client connection to notify...");
}



void loop() {
  if (IrReceiver.decode() && listeningIR && deviceConnected) {
    listeningIR = false;
    pCharacteristic->setValue(String(IrReceiver.decodedIRData.decodedRawData, HEX));
    pCharacteristic->notify();

    // In mã nhận được dưới dạng HEX
    // Serial.print("Cảm biến IR nhận mã: ");
    // Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    // Tiếp tục nhận tín hiệu tiếp theo
    IrReceiver.resume();
  }
}
