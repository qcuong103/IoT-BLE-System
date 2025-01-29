#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>  // Thư viện kết nối WiFi
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"
// Cấu hình chân kết nối
#define TFT_CS    5   // Chip Select
#define TFT_RST   17  // Reset
#define TFT_DC    16  // Data/Command

// Khởi tạo màn hình
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Cấu hình mạng WiFi
const char* ssid = "Redmi";  // Thay bằng tên WiFi của bạn
const char* password = "12345678";  // Thay bằng mật khẩu WiFi của bạn

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 3600000);  // Cấu hình NTP, lấy thời gian từ "pool.ntp.org"

#define USER_EMAIL "dqcuong103@gmail.com"
#define USER_PASSWORD "Q10032000"
#define DATABASE_URL "https://iot-ble-system-default-rtdb.asia-southeast1.firebasedatabase.app" // Thay bằng URL của Fireba

FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 180000;

void setup() {
    Serial.begin(115200);

    // Khởi tạo màn hình
    tft.initR(INITR_BLACKTAB);  // Sử dụng cấu hình ST7735 Black Tab
    tft.setRotation(1);        // Xoay màn hình nếu cần
    tft.fillScreen(ST77XX_BLACK);

    // Kết nối với WiFi
    WiFi.begin(ssid, password);
    tft.setCursor(0, 60);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.println("Connecting to WiFi...");
    
    // Chờ kết nối WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        tft.print(".");
    }

    // Hiển thị địa chỉ IP sau khi kết nối
    tft.fillScreen(ST77XX_BLACK);  // Clear screen
    tft.setCursor(0, 60);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(1);
    tft.println("WiFi Connected!");
    tft.print("IP: ");
    tft.println(WiFi.localIP());
    delay(2000);  // Hiển thị địa chỉ IP trong 2 giây

     // Cấu hình Firebase
    firebaseConfig.api_key = API_KEY;
    firebaseConfig.database_url = DATABASE_URL;
    firebaseAuth.user.email = USER_EMAIL;
    firebaseAuth.user.password = USER_PASSWORD;

    Firebase.reconnectWiFi(true);
    firebaseData.setResponseSize(4096);

    // Assign the maximum retry of token generation
    firebaseConfig.max_token_generation_retry = 5;

    Firebase.begin(&firebaseConfig, &firebaseAuth);

    // Getting the user UID might take a few seconds
    Serial.println("Getting User UID");
    while ((firebaseAuth.token.uid) == "") {
        Serial.print('.');
        delay(1000);
    }
    // Print user UID
    uid = firebaseAuth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.println(uid);

    // Update database path
    databasePath = "/UsersData/" + uid + "/readings";

}

void displayData(float humidity, float temperature) {
    tft.fillScreen(ST77XX_BLACK);  // Xóa màn hình
    
    // Hiển thị icon nhiệt độ
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.printf("%.2f C", temperature);

    // Hiển thị icon độ ẩm
    tft.setCursor(10, 40);
    tft.setTextColor(ST77XX_CYAN);
    tft.setTextSize(2);
    tft.printf("%.2f %%", humidity);
}

void loop() {
    timeClient.update();
    String currentTime = timeClient.getFormattedTime();

    // Tạo dữ liệu ngẫu nhiên
    float humidity = random(3000, 8000) / 100.0;    // Độ ẩm (30.00% đến 80.00%)
    float temperature = random(2000, 4000) / 100.0; // Nhiệt độ (20.00°C đến 40.00°C)

    // Gửi dữ liệu lên Firebase
    FirebaseJson data;
    data.set("humidity", humidity);
    data.set("temperature", temperature);
    data.set("time", currentTime); // Gửi thời gian thực

    if (Firebase.setJSON(firebaseData, "/sensorData", data)) {
        Serial.println("Data sent successfully.");
    } else {
        Serial.print("Failed to send data: ");
        Serial.println(firebaseData.errorReason());
    }

    // Hiển thị dữ liệu
    displayData(humidity, temperature);

    // Cập nhật sau mỗi 2 giây
    delay(5000);
}
