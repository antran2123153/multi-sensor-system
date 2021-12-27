<<<<<<< HEAD

## Triển khai trên một máy tính đơn

- Với kiến trúc đa lõi hiện nay của hầu hết các máy tính, ta có thể tập dụng nguồn tài nguyên này để có thể xây dựng hệ thống sinh dữ liệu trên một máy tính duy nhất.
- Việc triển khai ta xây dựng 3 service trên cùng máy đại diện cho producer, consumer, server. Trong đó:
  - Server được triển khai dựa trên sự hỗ trợ của các image được cung cấp sẵn thông qua docker, khi đó ta có thể triển khai một hoặc một cụm các broker cho server nhanh chóng.
  - Chương trình xây dựng cho producer được chạy trên nhiều luồng để có thể xem như có nhiều producer đang chạy cùng lúc.
  - Khi đó với hệ thống hoặc chương trình cần nhận dữ liệu để kiểm thử, sẽ chạy chương trình của consumer và kết nối đến server.
  - Các dịch vụ giao tiếp với nhau thông qua các cổng cảu máy tính. Việc đa luồng phân chia cho các dịch vụ phải được điều chỉnh phù hợp với khả năng của máy tính.

## Nhiều máy tính trong cùng một mạng LAN

- Với mô hình một máy trên chỉ có thể đáp ứng được khả năng cung cấp dữ liệu nhỏ. Thực tế lượng dữ liệu thường cho một hệ thống thời gian thực là cực lớn. Vì thế, ta phải tăng sức mạnh bằng phiệc phối hợp nhiều máy tính lại với nhau.\par
- Trong mô hình này ta cũng cần xây dựng tương tự 3 chương trình cho consumer, producer, server nhưng với quy mô lớn hơn như sau:
  - Server: sử dụng một máy tính chuyên biệt để làm server hoặc có thể sử dụng dịch vụ cloud.
  - Producer: được triển khải trên một cụm máy(cluster). Trong đó, có 1 máy nắm quyền điều khiển(manager), thông qua máy này ta có thể nhanh chóng thao tác truyền lệnh đến nhiều máy tính còn lại khác trong mạng(worker). Các máy trong cụm thực hiện chạy chương trình sinh dữ liệu trên nhiều thread.
  - Consumer: triển khai trên một máy riêng để nhận dữ liệu.
  - Các máy tính giao tiếp, truyền - nhận dữ liệu thông qua mạng LAN đã được kết nối sẵn.

## Nhiều máy tính phân tán xa nhau

- Việc triển khai trên mạng LAN gặp một số bất lợi về yêu cầu các máy phải ở gần nhau và khả năng mở rộng chưa cao. Khắc phục nhược điểm đó ta triển khai mô hình trên mạng diện rộng WAN. Khi đó ta có thể linh hoạt phối hợp nhiều máy ở các nơi xa nhau, đồng thời khả năng mở rộng cũng được tăng cường khi cần thêm máy vào trong hệ thống.\par
- Với quy mô của mô hình này, ta mở rộng thêm bằng việc triển khai cho server bằng một hệ các máy để tăng khả năng xử lý tránh tắc nghẽn, quá tải. Còn lại việc triển khai cho producer và consumer cũng tương tự như mô hình trên, khác biệt ở phần giao tiếp sẽ thông qua mạng WAN.

## Liên kết đính kèm:

- Source code github: https://github.com/antran2123153/Mult...
- Báo cáo trình bày chi tiết: https://www.overleaf.com/read/dptqdqv...
- Slide thuyết trình: https://docs.google.com/presentation/...
- Video hướng dẫn chạy: https://youtu.be/aAymK9hcAZU

## Thông tin tham khảo:

- Zookeeper: https://zookeeper.apache.org
- Apache Kafka: https://kafka.apache.org/documentation
- Docker: https://docs.docker.com
- Docker network: https://docs.docker.com/network
- Docker compose: https://docs.docker.com/compose
- Docker Swarm:https://docs.docker.com/engine/swarm
- Kafka libary in C programming: https://github.com/edenhill/librdkafka
- OpenMP: https://www.openmp.org/wp-content/upl...
