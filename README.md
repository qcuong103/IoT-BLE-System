# IoT BLE System

## Giới thiệu
Hệ thống IoT giám sát nhiệt độ và độ ẩm sử dụng Bluetooth Low Energy (BLE) để truyền dữ liệu từ vi điều khiển STM32 đến ESP32. ESP32 hiển thị dữ liệu trên màn hình TFT và đồng bộ hóa với Firebase, từ đó cung cấp thông tin theo thời gian thực trên ứng dụng di động.

## Cấu trúc hệ thống
- **Cảm biến DHT21**: Đo nhiệt độ và độ ẩm.
- **STM32F411CEU6**: Xử lý dữ liệu cảm biến, gửi qua BLE.
- **Module BLE HM-10**: Giao tiếp BLE giữa STM32 và ESP32.
- **ESP32**: Nhận dữ liệu BLE, hiển thị trên màn hình TFT, gửi lên Firebase.
- **Firebase**: Lưu trữ và đồng bộ hóa dữ liệu.
- **Ứng dụng di động (React Native)**: Hiển thị dữ liệu và biểu đồ theo thời gian thực.

## Cài đặt và triển khai
### 1. Cài đặt phần mềm
- **STM32**: Lập trình bằng STM32CubeIDE.
- **ESP32**: Sử dụng PlatformIO hoặc Arduino IDE.
- **Ứng dụng di động**: React Native + Expo.

### 2. Kết nối phần cứng
1. Kết nối cảm biến DHT21 với STM32.
2. Kết nối module HM-10 với STM32 qua UART.
3. Kết nối ESP32 với màn hình TFT ST7735.
4. Kết nối ESP32 với Firebase qua Wi-Fi.

## Hướng dẫn sử dụng
1. Nạp chương trình cho STM32 để gửi dữ liệu BLE.
2. Nạp chương trình cho ESP32 để nhận dữ liệu và gửi lên Firebase.
3. Cài đặt ứng dụng di động và theo dõi dữ liệu theo thời gian thực.

## Kiểm chứng và đánh giá
- **Truyền dữ liệu BLE**: Đánh giá độ trễ và độ ổn định.
- **Firebase**: Kiểm tra khả năng đồng bộ hóa dữ liệu theo thời gian thực.
- **Ứng dụng di động**: Đánh giá khả năng hiển thị dữ liệu chính xác.

## Kết luận
Dự án cung cấp giải pháp giám sát nhiệt độ và độ ẩm thông minh, tích hợp BLE và Firebase để theo dõi từ xa. Trong tương lai, có thể mở rộng với nhiều loại cảm biến và tối ưu hóa thuật toán truyền dữ liệu BLE.

## Tác giả
- **Dương Quốc Cường** - [GitHub](https://github.com/qcuong103/IoT-BLE-System)
