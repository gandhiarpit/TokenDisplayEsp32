#include <esp_now.h>
#include <WiFi.h>
#include <TM1637Display.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

#define CLK 18
#define DIO 19
TM1637Display display(CLK, DIO);

HardwareSerial DF_Serial(1);
DFRobotDFPlayerMini myDFPlayer;

// REPLACE WITH SENDER MAC
uint8_t senderMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_token {
  uint16_t number;
  char digits[5];
} struct_token;

struct_token incomingToken;

void OnDataRecv(const esp_now_recv_info_t *mac, const uint8_t *data, int len) {
  memcpy(&incomingToken, data, sizeof(incomingToken));
  display.showNumberDec(incomingToken.number, false);
  
  // Announce: e.g. token 1234 plays file 1234 (/01/01234.mp3 not needed, pads to 4 digits)
  int fileId = incomingToken.number;
  myDFPlayer.play(fileId);
  
  Serial.printf("Token %d (%s) announced\n", incomingToken.number, incomingToken.digits);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  
  display.setBrightness(0x0f);
  
  // DFPlayer UART1: RX=16, TX=17
  DF_Serial.begin(9600, SERIAL_8N1, 16, 17);
  if (!myDFPlayer.begin(DF_Serial)) {
    Serial.println("DFPlayer init failed! Check wiring/SD.");
    while (1) delay(100);
  }
  myDFPlayer.volume(25);  // 0-30
  Serial.println("DFPlayer OK");
  
  Serial.println("Receiver ready. MAC: " + WiFi.macAddress());
}

void loop() {
  // Idle, handles callbacks
}
