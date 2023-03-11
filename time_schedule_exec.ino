// wifiに接続して1秒ごとに年月日曜日、時間を取得し定時に処理を実行
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>

#define JST 3600 * 9
const int led = 2;
const char* ssid = "187Hideout-2G";
const char* password = "1prrm6ened0xyt";

Adafruit_NeoPixel pixels(1, led, NEO_GRB + NEO_KHZ800);
WiFiClientSecure secureClient;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  secureClient.setInsecure();
  delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    pixels.setPixelColor(0, pixels.Color(30, 0, 0));
    pixels.show();
    delay(100);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    delay(100);
  }
  Serial.printf("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  pixels.setPixelColor(0, pixels.Color(0, 30, 30));
  pixels.show();

  configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  delay(5000);
}

int lineSend(String message, int SI = 0, int PSI = 0) {
  const char* host = "notify-api.line.me";
  const char* token = "7V7Px0bfjGo4M6PpcbJfiZrvrUIkO8n9OFbY6YqoAC5";  //line notify token

  Serial.println("Try");
  if (!secureClient.connect(host, 443)) {
    Serial.println("Connection failed");
    return (0);
  }
  Serial.println("Connected");
  String query = "message=" + message;
  if (SI > 0) {
    query = query + "&stickerId=" + SI + "&stickerPackageId=" + PSI;
  }

  Serial.println(query);
  String request =
    String("") + "POST /api/notify HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Authorization: Bearer " + token + "\r\n" + "Content-Length: " + String(query.length()) + "\r\n" + "Content-Type: application/x-www-form-urlencoded\r\n\r\n" + query + "\r\n";
  secureClient.print(request);
  String res = secureClient.readString();
  Serial.println(res);
  secureClient.stop();
  return (1);
}

void loop() {
  time_t t;
  struct tm* tm;

  t = time(NULL);
  tm = localtime(&t);
  Serial.println(tm->tm_sec);
  // if (tm->tm_hour == 0) { // set target hour
  //   if (tm->tm_min == 0) { // set target minute
  //     if (tm->tm_sec == 0) { // set target second
  //       lineSend("exec");
  //     }
  //   }
  // }

  if (tm->tm_min == 0) {    // set target minute
    if (tm->tm_sec == 0) {  // set target second
      lineSend("exec");
    }
  }

  delay(1000);
}