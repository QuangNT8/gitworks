
#include <ArduinoJson.h>
#include <SocketIOClient.h>
#include <SoftwareSerial.h>
//#include <SerialCommand.h>  // Thêm vào sketch thư viện Serial Command 

SocketIOClient client;
int led = 16;
const char* ssid = "quangp610";          //Tên mạng Wifi mà Socket server của bạn đang kết nối
const char* password = "quang102";  //Pass mạng wifi ahihi, anh em rãnh thì share pass cho mình với.
 
 char host[] = "192.168.1.4";  //Địa chỉ IP dịch vụ, hãy thay đổi nó theo địa chỉ IP Socket server của bạn.
 int port = 3484;                  //Cổng dịch vụ socket server do chúng ta tạo!
/* https://beautiful-badlands-80233.herokuapp.com/#!/ */

//char host[] = "beautiful-badlands-80233.herokuapp.com";  
//int port = 80;                  

char namespace_esp8266[] = "esp8266";   
 
//từ khóa extern: dùng để #include các biến toàn cục ở một số thư viện khác. Trong thư viện SocketIOClient có hai biến toàn cục
// mà chúng ta cần quan tâm đó là
// RID: Tên hàm (tên sự kiện
// Rfull: Danh sách biến (được đóng gói lại là chuối JSON)
extern String RID;
extern String Rfull;
 
 
//Một số biến dùng cho việc tạo một task
unsigned long previousMillis = 0;
long interval = 2000;
 
void setup()
{
    //Bật baudrate ở mức 115200 để giao tiếp với máy tính qua Serial
    Serial.begin(115200);
    delay(10);
     pinMode(led,OUTPUT);
    //Việc đầu tiên cần làm là kết nối vào mạng Wifi
    Serial.print("Ket noi vao mang ");
    Serial.println(ssid);
 
    //Kết nối vào mạng Wifi
    WiFi.begin(ssid, password);
 
    //Chờ đến khi đã được kết nối
    while (WiFi.status() != WL_CONNECTED) { //Thoát ra khỏi vòng 
        delay(500);
        Serial.print('.');
    }
 
    Serial.println();
    Serial.println(F("Da ket noi WiFi"));
    Serial.println(F("Di chi IP cua ESP8266 (Socket Client ESP8266): "));
    Serial.println(WiFi.localIP());
 
    if (!client.connect(host, port)) {
        Serial.println(F("Ket noi den socket server that bai!"));
        return;
    }
 
    //Khi đã kết nối thành công
    if (client.connected()) {
        //Thì gửi sự kiện ("connection") đến Socket server ahihi.
        Serial.println(F("connected to webapp"));
        client.send("connection", "message", "Connected !!!!");
    }
}
 
void loop()
{

    //Khi bắt được bất kỳ sự kiện nào thì chúng ta có hai tham số:
    //  +RID: Tên sự kiện
    //  +RFull: Danh sách tham số được nén thành chuỗi JSON!
    if (client.monitor()) 
    {
    
//        Serial.println(RID);
//        Serial.println(Rfull);
        led_out();
        
    }
 
    //Kết nối lại!
    if (!client.connected()) {
      client.reconnect(host, port,namespace_esp8266);
    }
}
void led_out() 
{
    DynamicJsonBuffer jsonBuffer;
     String input =
    "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    JsonObject& root = jsonBuffer.parseObject(Rfull);
     long time = root["led"][0];
    //kiểm thử giá trị
    Serial.println(time);
    Serial.println("ID:");
    Serial.println(RID);
    Serial.println("FULL:");
    Serial.println(Rfull); 
    digitalWrite(led, time);
    if(RID=="led")client.send("LED_STATUS", Rfull);//gửi dữ liệu về cho Socket Server
}
/*
void defaultCommand(String command) {
  char *json = sCmd.next();
  
 
  //In ra serial monitor để debug
  Serial.print(command);
  Serial.print(' ');
  Serial.println(json);
}*/
