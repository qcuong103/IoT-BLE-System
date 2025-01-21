# IoT-BLE-System
# Thiết Kế Hệ Thống IoT Sử Dụng BLE

## Tổng Quan Dự Án
Dự án này tập trung vào việc thiết kế và triển khai một hệ thống IoT để giám sát các thông số môi trường từ xa bằng công nghệ IoT. Hệ thống sử dụng vi điều khiển ARM để thu thập và truyền dữ liệu. BLE (Bluetooth Low Energy) được sử dụng để giao tiếp giữa các nút cảm biến và gateway, dữ liệu sau đó được gửi lên cloud để người dùng truy cập qua ứng dụng di động hoặc website.

## Mục Lục

1. [Giới Thiệu](#giới-thiệu)
2. [Yêu Cầu Thiết Kế](#yêu-cầu-thiết-kế)
   - [Đặc Tả Kỹ Thuật](#đặc-tả-kỹ-thuật)
3. [Triển Khai Hệ Thống](#triển-khai-hệ-thống)
   - [Kiến Trúc Phần Cứng](#kiến-trúc-phần-cứng)
     - [Khối Xử Lý Trung Tâm](#khối-xử-lý-trung-tâm)
     - [Cảm Biến Đầu Vào](#cảm-biến-đầu-vào)
     - [LED](#led)
     - [LCD](#lcd)
   - [Lập Trình Hệ Thống Nhúng](#lập-trình-hệ-thống-nhúng)
     - [Thiết Lập Clock Cho Hệ Thống](#thiết-lập-clock-cho-hệ-thống)
     - [Khởi Tạo Cảm Biến](#khởi-tạo-cảm-biến)
     - [Khởi Tạo Mô-đun BLE](#khởi-tạo-mô-đun-ble)
     - [Khởi Tạo Systick Timer](#khởi-tạo-systick-timer)
     - [Thiết Lập Mức Ưu Tiên Ngắt](#thiết-lập-mức-ưu-tiên-ngắt)
     - [Chương Trình Chính](#chương-trình-chính)
   - [Phát Triển Ứng Dụng/Website](#phát-triển-ứng-dụng-website)
4. [Kiểm Chứng](#kiểm-chứng)
5. [Kết Luận](#kết-luận)

## Giới Thiệu
Mục tiêu của dự án là vận dụng kiến thức và kỹ năng đã học để thiết kế và triển khai một hệ thống IoT có khả năng giám sát các thông số môi trường từ xa, ví dụ như nhiệt độ, độ ẩm hoặc ánh sáng. Dự án yêu cầu sử dụng vi điều khiển ARM để thực hiện.

## Yêu Cầu Thiết Kế

### Đặc Tả Kỹ Thuật
- **Nút Cảm Biến:**
  - Sử dụng vi điều khiển ARM (ví dụ: STM32).
  - Được trang bị cảm biến để đo ít nhất một thông số môi trường.
  - Giao tiếp với gateway qua BLE.

- **Gateway:**
  - Có thể thực hiện bằng smartphone, ESP32, v.v.
  - Chuyển tiếp dữ liệu từ nút cảm biến lên cloud.
  - Giao tiếp với nút cảm biến qua BLE và với cloud qua LTE/Wi-Fi.

- **Cloud:**
  - Lưu trữ dữ liệu cảm biến và cho phép người dùng truy cập từ xa.

- **Ứng Dụng Di Động/Website:**
  - Lấy dữ liệu từ cloud và hiển thị cho người dùng.

## Triển Khai Hệ Thống

### Kiến Trúc Phần Cứng

#### Khối Xử Lý Trung Tâm
Hệ thống sử dụng vi điều khiển ARM để triển khai thuật toán điều khiển.

#### Cảm Biến Đầu Vào
Cảm biến được sử dụng để đo các thông số môi trường. Ví dụ:
- **DHT21:** Đo nhiệt độ và độ ẩm.

#### LED
LED được sử dụng để hiển thị trạng thái. Ví dụ:
- LED đỏ: Pin PTE29
- LED xanh lá: Pin PTD5

#### LCD
Hiển thị dữ liệu cảm biến theo thời gian thực.

### Lập Trình Hệ Thống Nhúng

#### Thiết Lập Clock Cho Hệ Thống
Cấu hình clock hệ thống để đảm bảo hoạt động ổn định.

#### Khởi Tạo Cảm Biến
Các quy trình khởi tạo cảm biến môi trường.

#### Khởi Tạo Mô-đun BLE
- Kích hoạt clock.
- Thiết lập ngắt.

#### Khởi Tạo Systick Timer
Cấu hình bộ đếm thời gian để kiểm tra hệ thống định kỳ.

#### Thiết Lập Mức Ưu Tiên Ngắt
Thiết lập mức ưu tiên cho các ngắt để đảm bảo hệ thống hoạt động trơn tru.

#### Chương Trình Chính
Thực hiện logic chính và luồng điều khiển.

### Phát Triển Ứng Dụng/Website
Phát triển giao diện người dùng để lấy và hiển thị dữ liệu từ cloud.

## Kiểm Chứng
Kiểm chứng bao gồm:
- Trình diễn hệ thống hoạt động.
- Cung cấp video minh họa thiết lập và kết quả.

## Kết Luận
Dự án đã triển khai thành công hệ thống IoT sử dụng vi điều khiển ARM và BLE. Hệ thống đáp ứng được việc giám sát từ xa và cung cấp dữ liệu cho người dùng thông qua ứng dụng di động hoặc website.

## Tài Liệu Tham Khảo
- Vui lòng bổ sung các tài liệu tham khảo tại đây.

---