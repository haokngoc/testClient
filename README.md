## Bài Test 1: baitest1.cpp
### Mô Tả
- Client gửi đến server một bản tin 5 byte, trong đó có cmd là DET_STATE_WORK.
- Client nhận bản tin 21Mb từ server.
- Client gửi liên tiếp 4 lần bản tin 5 byte, trong đó cmd không liên quan đến State của server
- Cuối cùng, client gửi bản tin 5 byte với cmd là DET_STATE_WORK và bản tin 21Mb từ server.
## Bài Test 2: baitest2.cpp
### Mô Tả
- Client gửi liên tiếp 5 lần bản tin 5 byte, trong đó cmd là DET_STATE_WORK.
- Đồng thời client nhận 5 lần bản tin 21Mb từ server.
## Bài Test 3: baitest3.cpp
### Mô Tả
- Client gửi đến server một bản tin 5 byte, trong đó có cmd là DET_STATE_WORK.
- Client nhận bản tin 21Mb từ server.
- Trong lúc nhận bản tin 21Mb, client chỉ nhận 15Mb và thoát khỏi chương trình.
## Bài Test 4: baitest4.cpp
### Mô Tả
- Client gửi đến server 4 bản tin 5 byte, trong đó có 3 bản tin có header không hợp lệ và 1 bản tin hợp lệ
## Bài Test 5:
### Mô Tả
- Khởi chạy server, server đang đợi kết nối từ client
- Sau đó ta chạy nhiều client một lúc
## Bài Test 6:
### Mô Tả
- Client gửi đến server một bản tin 5 byte, trong đó có cmd là DET_STATE_WORK.
- Ở client tạo một vòng lặp while true, để gửi yêu cầu và nhận liên tục từ server
- Test chương trình trong khoảng thời gian dài (24 giờ).



