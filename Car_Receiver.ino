#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

#define pinIN1 D1
#define pinIN2 D2
#define pinServo D0

typedef struct struct_message
{
    float x;
    float y;
    int sw;
} struct_message;

struct_message myData;

Servo servo;

float Lerp(float a, float b, float t)
{
  return (1 - t) * a + t * b;
}

void MotorSpeed(float speed)
{
  //Avant 
  if(speed > 0)
  {
    analogWrite(pinIN1, uint8_t(speed));
    analogWrite(pinIN2, 0);
    Serial.println("Avant");
  }
    
  // Arriere
  if(speed < 0)
  {
    analogWrite(pinIN1, 0);
    analogWrite(pinIN2, uint8_t(abs(speed)));
    Serial.println("Arriere");
  }
  Serial.print("Speed : ");
  Serial.println(int(speed));
  
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&myData, incomingData, sizeof(myData));

  // Rotation
  float value = Lerp(1, 180, myData.x / 1024);
  servo.write(value);

  // Move
  value = myData.y / 1024;
  value = Lerp(255, -255, value);
  MotorSpeed(value);

  Serial.println(myData.sw);
}

void setup()
{
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  servo.attach(pinServo);
}

void loop()
{
  
}