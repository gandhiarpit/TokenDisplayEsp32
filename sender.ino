#include <esp_now.h>
#include <WiFi.h>
#include <Keypad.h>
#include <TM1637Display.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};  // Rows
byte colPins[COLS] = {26, 25, 33, 32};  // Columns
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define CLK 18
#define DIO 19
TM1637Display display(CLK, DIO);

// REPLACE WITH RECEIVER MAC: e.g. {0x24,0x6F,0x28,0x12,0x34,0x56}
uint8_t receiverMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_token {
  uint16_t number;
  char digits[5];  // "1234\0"
} struct_token;

struct_token myToken;
String currentNum = "";

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Peer add failed");
    return;
  }
  
  display.setBrightness(0x0f);
  display.showNumberDec(0, false);
  Serial.println("Sender ready. MAC: " + WiFi.macAddress());
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key: ");
    Serial.println(key);
    
    if (key >= '0' && key <= '9' && currentNum.length() < 4) {
      currentNum += key;
      display.showNumberDec(currentNum.toInt(), false);
    }
    else if (key == '#') {  // Send
      if (currentNum.length() == 4) {
        myToken.number = currentNum.toInt();
        strcpy(myToken.digits, currentNum.c_str());
        esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &myToken, sizeof(myToken));
        if (result == ESP_OK) {
          Serial.println("Token sent: " + currentNum);
        } else {
          Serial.println("Send error");
        }
        currentNum = "";  // Reset for next
        display.clear();
      }
    }
    else if (key == '*') {  // Clear
      currentNum = "";
      display.clear();
    }
  }
  delay(50);
}
