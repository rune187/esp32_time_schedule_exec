// wifiに接続して1秒ごとに年月日曜日、時間を取得し定時に処理を実行
#include <WiFi.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>

#define JST 3600 * 9
const int led = 2;
const char* ssid = "ssid";
const char* password = "pass";

Adafruit_NeoPixel pixels(1, led, NEO_GRB + NEO_KHZ800);
WiFiClientSecure secureClient;

void setup() {
  Serial.begin(115200);
  pixels.begin();
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

void any_func() {
  Serial.println("exec");
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
      any_func();
    }
  }

  delay(1000);
}
