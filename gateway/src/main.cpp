#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

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

void setup() {
    Serial.begin(115200);

    // Khởi tạo màn hình
    tft.initR(INITR_BLACKTAB);  // Sử dụng cấu hình ST7735 Black Tab
    tft.setRotation(1);        // Xoay màn hình nếu cần
    tft.fillScreen(ST77XX_BLACK);
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

    // Hiển thị dữ liệu
    displayData(humidity, temperature);

    // Cập nhật sau mỗi 2 giây
    delay(2000);
}
