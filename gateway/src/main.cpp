#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>  // Thư viện kết nối WiFi
#include <FirebaseESP32.h>
#include <.env>
// Cấu hình chân kết nối
#define TFT_CS    5   // Chip Select
#define TFT_RST   17  // Reset
#define TFT_DC    16  // Data/Command

// Khởi tạo màn hình
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Bitmap icon (16x16 pixel)
const uint16_t tempIcon[] PROGMEM = {
    0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
    0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x07E0, 0xF800, 0xF800, 0xF800, 0x07E0, 0x0000,
    0x0000, 0x0000, 0x0000, 0x07E0, 0xF800, 0xF800, 0x07E0, 0x0000
};

const uint16_t humidIcon[] PROGMEM = {
    0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,
    0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x07FF, 0xF81F, 0xF81F, 0xF81F, 0x07FF, 0x0000,
    0x0000, 0x0000, 0x0000, 0x07FF, 0xF81F, 0xF81F, 0x07FF, 0x0000
};

// Cấu hình mạng WiFi
const char* ssid = "Redmi";  // Thay bằng tên WiFi của bạn
const char* password = "12345678";  // Thay bằng mật khẩu WiFi của bạn

#define API_KEY ".env/API_KEY"
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
    tft.drawRGBBitmap(10, 10, tempIcon, 16, 16);
    tft.setCursor(30, 10);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.printf("%.2f C", temperature);

    // Hiển thị icon độ ẩm
    tft.drawRGBBitmap(10, 40, humidIcon, 16, 16);
    tft.setCursor(30, 40);
    tft.setTextColor(ST77XX_CYAN);
    tft.setTextSize(2);
    tft.printf("%.2f %%", humidity);
}

void loop() {
    // Tạo dữ liệu ngẫu nhiên
    float humidity = random(3000, 8000) / 100.0;    // Độ ẩm (30.00% đến 80.00%)
    float temperature = random(2000, 4000) / 100.0; // Nhiệt độ (20.00°C đến 40.00°C)

    // Gửi dữ liệu lên Firebase
    if (Firebase.setFloat(firebaseData, "/sensorData/temperature", temperature)) {
        Serial.println("Temperature data sent successfully.");
    }
    if (Firebase.setFloat(firebaseData, "/sensorData/humidity", humidity)) {
        Serial.println("Humidity data sent successfully.");
    }

    // Hiển thị dữ liệu
    displayData(humidity, temperature);

    // Cập nhật sau mỗi 2 giây
    delay(5000);
}
