
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NewPingESP8266.h>
#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPingESP8266 sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP8266_ASYNC;

float distanceToObstacle;
WebSocketsServer webSocket = WebSocketsServer(81);
int arduinoWakePin = 13;
const char* ssid = "PUPU";
const char* password = "pakistan9";
int AnyRequest = 0;
int iterations = 5;

void setup() {
  //  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  // Serial.println("WIFI connected");
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  distanceToObstacle = getDistance();
}

void loop() {
  distanceToObstacle = getDistance();
  webSocket.loop();
  if (AnyRequest == 1) {
    String stringifiedDistanceToObstacle = String(distanceToObstacle);
    webSocket.sendTXT(0, stringifiedDistanceToObstacle);
  }
  delay(500);
}



void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  String request;
  if (type == WStype_TEXT) {
    AnyRequest = 1;
  }
  else if (type == WStype_DISCONNECTED) {
    AnyRequest = 0;
  }
}

float getDistance() {
  float duration = sonar.ping_median(iterations);
  float distance = (duration / 2) * 0.0343;
  return distance;
}
