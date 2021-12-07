## Các thư mục tương ứng với các mô hình triển khai
- SH: Single host
- MHL: Multi host in LAN network
- MCSH: Multi container in single host

## Chạy mã
### Single host:
```
$ docker-compose up # chạy tất cả service cùng lúc

$ docker-compose up <SERVICE-NAME> # chạy từng service một
```
### Multi host in LAN network:
- Mở cổng giao tiếp cho các máy kết nối trong swarm: 22/tcp, 2377/tcp, 7946/tcp, 7946/udp, 4789/udp
- Chạy máy làm server kafka:
```
$ MLH>kafka-server> docker-compose up
```
- Chạy máy làm consumer:
```
$ MLH>consumer> docker build .
$ MLH>consumer> docker run .
```
- Thao tác trên swarm cho producer:
```
$ MLH>producer> docker swarm init --advertise-addr <MANAGER-IP>
$ MLH>producer> docker swarm join --token <TOKEN> <MANAGER-IP>:<PORT>
```
- Xem các node đang có trong swarm:
```
$ MLH>producer> docker node ls
```
- Deploy producer service lên swarm:
```
$ docker service create --replicas <NUM-TASK> --name <SERVICE-NAME> <IMAGE>
```
- Dừng và xóa service:
```
$ docker service rm <SERVICE-NAME>
```
