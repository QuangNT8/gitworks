#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
//https://smarthome-4d58d.firebaseio.com/
#define Firebase_HOST "webapp1-testing.firebaseio.com"
#define Firebase_AUTH ""

//#define WIFI_SSID "QuangNV7"
//#define WIFI_PASSWORD "12345678@"

#define WIFI_SSID "IDD-T1"
#define WIFI_PASSWORD "1qazxsw@"

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  // Két nối wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //Firebase.begin(Firebase_HOST, Firebase_AUTH);
  Firebase.begin(Firebase_HOST);
}
void loop() {
  //String giatri = Firebase.getString("/led");
  String giatri = Firebase.getString("/messages/.../name");
  Serial.println(giatri);
  Serial.println(giatri);
  if (giatri == "on")
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (giatri == "off")
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }

//  Firebase.setInt("/Millis", millis());
//  Firebase.setFloat("/Millis:3", millis() / 3.0);
  Firebase.setInt("/Millis1", millis());
  Firebase.setFloat("/Millis1:3", millis() / 3.0);
  Serial.println(millis());
  delay(500);
}
