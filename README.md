# IoT BLE System

## Giới thiệu
Hệ thống IoT giám sát nhiệt độ và độ ẩm sử dụng Bluetooth Low Energy (BLE) để truyền dữ liệu từ vi điều khiển STM32 đến ESP32. ESP32 hiển thị dữ liệu trên màn hình TFT và đồng bộ hóa với Firebase, từ đó cung cấp thông tin theo thời gian thực trên ứng dụng di động. Hệ thống giúp người dùng có thể theo dõi và phân tích dữ liệu môi trường theo thời gian thực, hỗ trợ các ứng dụng trong nông nghiệp, nhà thông minh và y tế.

## Cấu trúc hệ thống
Hệ thống bao gồm các thành phần chính sau:
- **Cảm biến DHT21**: Đo nhiệt độ và độ ẩm.
- **STM32F411CEU6**: Vi điều khiển thu thập dữ liệu cảm biến, xử lý và gửi qua BLE.
- **Module BLE HM-10**: Truyền tải dữ liệu không dây giữa STM32 và ESP32.
- **ESP32**: Nhận dữ liệu BLE, hiển thị trên màn hình TFT và gửi lên Firebase.
- **Firebase**: Lưu trữ và đồng bộ hóa dữ liệu theo thời gian thực.
- **Ứng dụng di động (React Native)**: Hiển thị dữ liệu và biểu đồ theo thời gian thực, giúp người dùng dễ dàng giám sát và phân tích dữ liệu.

## Cài đặt và triển khai
### 1. Cài đặt phần mềm
- **STM32**: Lập trình bằng STM32CubeIDE.
- **ESP32**: Sử dụng PlatformIO hoặc Arduino IDE.
- **Ứng dụng di động**: React Native + Expo.
- **Firebase**: Thiết lập cơ sở dữ liệu thời gian thực để lưu trữ và truy xuất dữ liệu cảm biến.

### 2. Kết nối phần cứng
1. Kết nối cảm biến DHT21 với STM32 qua giao tiếp digital.
2. Kết nối module HM-10 với STM32 qua UART.
3. Kết nối ESP32 với màn hình TFT ST7735 qua giao tiếp SPI.
4. Kết nối ESP32 với Firebase qua Wi-Fi để đồng bộ hóa dữ liệu.

## Hướng dẫn sử dụng
1. Nạp chương trình cho STM32 để thu thập dữ liệu và gửi qua BLE.
2. Nạp chương trình cho ESP32 để nhận dữ liệu, hiển thị trên màn hình và gửi lên Firebase.
3. Cài đặt ứng dụng di động React Native và đăng nhập Firebase để theo dõi dữ liệu theo thời gian thực.
4. Kiểm tra kết nối BLE và Wi-Fi để đảm bảo dữ liệu được truyền tải chính xác.

## Kiểm chứng và đánh giá
- **Truyền dữ liệu BLE**: Đánh giá độ trễ và độ ổn định trong nhiều điều kiện môi trường khác nhau.
- **Firebase**: Kiểm tra tốc độ đồng bộ hóa dữ liệu và khả năng mở rộng.
- **Ứng dụng di động**: Đánh giá giao diện người dùng, tính trực quan và khả năng hiển thị dữ liệu chính xác.
- **Tính tiêu thụ năng lượng**: Đo mức tiêu hao điện năng của từng thành phần để tối ưu hiệu suất hoạt động.

## Kết luận
Dự án này cung cấp một giải pháp IoT hiệu quả cho việc giám sát nhiệt độ và độ ẩm. Hệ thống hoạt động ổn định, cung cấp dữ liệu theo thời gian thực và dễ dàng tích hợp vào các ứng dụng thực tế như nông nghiệp thông minh, giám sát môi trường và hệ thống nhà thông minh. Trong tương lai, hệ thống có thể được mở rộng với các loại cảm biến khác như CO2, ánh sáng, áp suất và tích hợp thêm AI để phân tích dữ liệu thông minh hơn.

## Tác giả
- **Dương Quốc Cường** - [GitHub](https://github.com/qcuong103/IoT-BLE-System)
