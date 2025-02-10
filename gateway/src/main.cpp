#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>  // Thư viện kết nối WiFi
#include <FirebaseESP32.h>
#include "config.h"

#include <BLEDevice.h>

// Cấu hình chân kết nối
#define TFT_CS    5   // Chip Select
#define TFT_RST   17  // Reset
#define TFT_DC    16  // Data/Command

// Khởi tạo màn hình
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Cấu hình mạng WiFi
const char* ssid = "Redmi";  // Thay bằng tên WiFi của bạn
const char* password = "12345678";  // Thay bằng mật khẩu WiFi của bạn

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

// BLE
BLEClient* pClient;
BLERemoteCharacteristic* pRemoteCharacteristic;
bool connected = false;
std::string targetAddress = "d4:36:39:8a:d7:cd";  // Thay bằng MAC của HM-10

int lastReceivedInt = 10000000;
int receivedInt = 10000000;

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
        Serial.println("Connected to HM-10!");
        connected = true;
    }
    void onDisconnect(BLEClient* pclient) {
        Serial.println("Disconnected.");
        connected = false;
    }
};

float calculateHeatIndex(float temperature, float humidity) {
    if (temperature < 26.0) {
    return temperature;
  }
    float c1 = -8.78469475556;
    float c2 = 1.61139411;
    float c3 = 2.33854883889;
    float c4 = -0.14611605;
    float c5 = -0.012308094;
    float c6 = -0.0164248277778;
    float c7 = 0.002211732;
    float c8 = 0.00072546;
    float c9 = -0.000003582;

    float heatIndex =    c1 + (c2 * temperature) + (c3 * humidity) + 
                        (c4 * temperature * humidity) + 
                        (c5 * temperature * temperature) + 
                        (c6 * humidity * humidity) + 
                        (c7 * temperature * temperature * humidity) + 
                        (c8 * temperature * humidity * humidity) + 
                        (c9 * temperature * temperature * humidity * humidity);

    return heatIndex;
}

void displayData(float humidity, float temperature) {
    float heatIndex = calculateHeatIndex(temperature, humidity);
    tft.setCursor(9, 54);
    tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
    if (temperature < 0)
        tft.printf("-%.2f", temperature);
    else
        tft.printf(" %.2f", temperature);

    tft.setCursor(21, 100);
    tft.setTextColor(ST7735_MAGENTA, ST7735_BLACK);
    tft.printf("%.2f %%", humidity);

    tft.setCursor(28, 146);
    tft.setTextColor(0xFD00, ST7735_BLACK);
    tft.printf("%.1f",calculateHeatIndex(temperature, humidity));  // Display heat index with one decimal place
    // tft.setCursor(97, 146);         // Corrected cursor position for the degree symbol
    // tft.print("C");
}

void updateFirebase(float temperature, float humidity) {
    FirebaseJson data;
    data.set("humidity", humidity);
    data.set("temperature", temperature);
    if (Firebase.setJSON(firebaseData, "/sensorData", data)) {
        Serial.println("Data sent successfully.\n");
    } else {
        Serial.println("Failed to send data: " + firebaseData.errorReason());
    }
}

void drawCenteredText(int y, const char* text, uint16_t color, uint16_t bgColor, uint8_t textSize) {
    int16_t x1, y1;
    uint16_t w, h;
    
    tft.setTextSize(textSize);
    tft.setTextColor(color, bgColor);
    
    // Tính kích thước chữ
    tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
    
    // Tính vị trí X để căn giữa
    int x = (tft.width() - w) / 2;
    
    tft.setCursor(x, y);
    tft.print(text);
}

// void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
//     Serial.println("Notification received!");
//     String receivedData = String((char*)pData).substring(0, length);
//     String value = String((char*)pData); // Chuyển đổi dữ liệu nhận được thành chuỗi
//     Serial.println("Received data: " + value);
//     float temp, humid;
//     if (sscanf(receivedData.c_str(), "%f %f", &temp, &humid) == 2) {
//         Serial.printf("Received: Temp = %.2f, Humid = %.2f\n", temp, humid);
//         displayData(temp, humid);
//     }
// }

void setup() {
    Serial.begin(115200);

    // Khởi tạo màn hình
    tft.initR(INITR_BLACKTAB);  // Sử dụng cấu hình ST7735 Black Tab
    tft.setRotation(0);        // Xoay màn hình nếu cần
    tft.fillScreen(ST77XX_BLACK);
    // WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, IPAddress(8, 8, 8, 8), IPAddress(8, 8, 4, 4));

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
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(8, 8, 8, 8), IPAddress(8, 8, 4, 4));  // Dùng DNS Google
      // Hiển thị địa chỉ IP trong 2 giây

    
     // Cấu hình Firebase
    firebaseConfig.api_key = API_KEY;
    firebaseConfig.database_url = DATABASE_URL;
    firebaseAuth.user.email = USER_EMAIL;
    firebaseAuth.user.password = USER_PASSWORD;

    // Firebase.reconnectWiFi(true);
    // firebaseData.setResponseSize(4096);

    // // Assign the maximum retry of token generation
    // firebaseConfig.max_token_generation_retry = 5;

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
    tft.print("User UID: ");
    tft.println(uid);
    // Update database path
    databasePath = "/UsersData/" + uid + "/readings";

    BLEDevice::init("ESP32_BLE_Client");

    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());

    Serial.println("Connecting to HM-10...");
    pClient->connect(BLEAddress(targetAddress.c_str()));
    tft.println("Connected to HM-10!");
    Serial.println("Connected and ready to receive data.");
    delay(5000);
    BLERemoteService* pRemoteService = pClient->getService(BLEUUID("0000ffe0-0000-1000-8000-00805f9b34fb"));
    pRemoteCharacteristic = pRemoteService->getCharacteristic(BLEUUID("0000ffe1-0000-1000-8000-00805f9b34fb"));

    pRemoteCharacteristic->registerForNotify([](BLERemoteCharacteristic* pChar, uint8_t* data, size_t len, bool isNotify) {
        Serial.println("Notification received!");
        String receivedStr = String((char*)data);
        receivedStr.trim();
        receivedInt = receivedStr.toInt();
        Serial.println("Received Int: " + receivedStr);
        // uint32_t receivedData = static_cast<uint32_t>(receivedInt);
        // float temp = (receivedData / 10000) / 100.0;
        // float humid = (receivedData % 10000) / 100.0;

        // Serial.printf("Temp: %.2f°C, Humid: %.2f%%\n", temp, humid);

        // updateFirebase(temp, humid);
        // printf("updateFirebase");

        // if (receivedData != lastData) {  // Chỉ gửi Firebase khi dữ liệu thay đổi
        //     // displayData(temp, humid);
        //     // printf("displayData");
        //     if (lastData != receivedData) {
        //         updateFirebase(temp, humid);
        //         printf("updateFirebase");
        //     }
        //     lastData = receivedData;
        // }
    });

    tft.fillScreen(ST77XX_BLACK);  // Xóa màn hình
    tft.drawFastHLine(0, 30, tft.width(), ST7735_WHITE);
    tft.drawFastHLine(0, 76, tft.width(), ST7735_WHITE);
    tft.drawFastHLine(0, 122, tft.width(), ST7735_WHITE);

    drawCenteredText(8, "IOT BLE", ST7735_WHITE, ST7735_BLACK, 2);
    drawCenteredText(39, "TEMPERATURE", ST7735_RED, ST7735_BLACK, 1);
    drawCenteredText(85, "HUMIDITY", ST7735_CYAN, ST7735_BLACK, 1);
    drawCenteredText(131, "FEELS LIKE", ST7735_GREEN, ST7735_BLACK, 1);

    tft.setTextSize(2);
    tft.drawCircle(87, 56, 2, ST7735_YELLOW);
    tft.setCursor(95, 54);  
    tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
    tft.print("C");

    tft.setTextColor(0xFD00, ST7735_BLACK);
    tft.drawCircle(81, 148, 2, 0xFD00);
    tft.setCursor(89, 146);  
    tft.print("C");
}


void loop() {
    // timeClient.update();
    // String currentTime = timeClient.getFormattedTime();

    // Tạo dữ liệu ngẫu nhiên
    // float humidity = random(3000, 8000) / 100.0;    // Độ ẩm (30.00% đến 80.00%)
    // float temperature = random(2000, 4000) / 100.0; // Nhiệt độ (20.00°C đến 40.00°C)

    // Gửi dữ liệu lên Firebase
    // FirebaseJson data;
    // data.set("humidity", humidity);
    // data.set("temperature", temperature);
    // data.set("time", currentTime); // Gửi thời gian thực

    // if (Firebase.setJSON(firebaseData, "/sensorData", data)) {
    //     Serial.println("Data sent successfully.");
    // } else {
    //     Serial.print("Failed to send data: ");
    //     Serial.println(firebaseData.errorReason());
    // }

    // Hiển thị dữ liệu
    // displayData(humidity, temperature);

    // Cập nhật sau mỗi 2 giây
    // delay(5000);

    if (receivedInt != lastReceivedInt) {
        uint32_t receivedData = static_cast<uint32_t>(receivedInt);
        float temp = (receivedData / 10000) / 100.0;
        float humid = (receivedData % 10000) / 100.0;
        Serial.printf("Temp: %.2f°C, Humid: %.2f%%\n", temp, humid);
        updateFirebase(temp, humid);
        Serial.println("updateFirebase");
        displayData(humid, temp);
        lastReceivedInt = receivedInt;
    }

    // if (connected && pRemote haracteristic->canRead()) {
    //     std::string value = pRemoteCharacteristic->readValue();
    //     Serial.println("Received: " + String(value.c_str()));
    //     value.erase(value.find_last_not_of(" \t\n\r") + 1);  // Xóa khoảng trắng cuối chuỗi
    //     int receivedInt = atoi(value.c_str());
    //     uint32_t receivedData = static_cast<uint32_t>(receivedInt);
    //     float temp = (receivedData / 10000) / 100.0;
    //     float humid = (receivedData % 10000) / 100.0;
    //     Serial.printf("Temp: %.2f°C, Humid: %.2f%%\n", temp, humid);
    //     updateFirebase(temp, humid);
    //     printf("updateFirebase");
    // }
    delay(2000);
    // yield();
}
