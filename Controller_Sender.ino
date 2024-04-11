#include <ESP8266WiFi.h>
#include <espnow.h>

#define AnalogPin A0
#define pinX D0
#define pinY D1
#define swPin D2

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x08, 0xF9, 0xE0, 0x73, 0xA4, 0x50};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float x;
  float y;
  int sw;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long timerDelay = 30;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);  
  pinMode(swPin, INPUT_PULLUP);
}
 
void loop() {
  delay(timerDelay);
  digitalWrite(pinY, LOW);
  digitalWrite(pinX, HIGH);
  myData.x = analogRead(AnalogPin);
  Serial.println(myData.x);

  digitalWrite(pinX, LOW);
  digitalWrite(pinY, HIGH);
  myData.y = analogRead(AnalogPin);
  Serial.println(myData.y);

  myData.sw = digitalRead(swPin);
  Serial.println(myData.sw);

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

}