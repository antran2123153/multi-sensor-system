# Multi sensor system

## Deploy on a single computer

- With the current multi-core architecture of most computers, we can use this resource to build a data generation system on a single computer.
- Deployment we build 3 services on the same machine representing producer, consumer, server. In there:
  - The server is deployed based on the support of images provided through docker, then we can deploy one or a cluster of brokers for the server quickly.
  The program built for the producer is run on multiple threads so that it can be seen as if there are multiple producers running at the same time.
  - Then, with the system or program that needs to receive data for testing, it will run the consumer's program and connect to the server.
  - Services communicate with each other through the ports of the computer. The multithreading division for services must be tailored to the computer's capabilities.

## Multiple computers in the same LAN

- With the above single-machine model, only the ability to provide small data can be met. In fact, the amount of data typically for a real-time system is enormous. Therefore, we must increase our power by combining many computers together.\par
- In this model, we also need to build the same 3 programs for consumer, producer, server but with a larger scale as follows:
  - Server: use a dedicated computer as a server or can use cloud services.
  - Producer: deployed on a cluster. In which, there is a machine that holds the control (manager), through this machine we can quickly manipulate and transmit commands to many other computers in the network (worker). The machines in the cluster execute the program that generates data on multiple threads.
  - Consumer: deployed on a separate machine to receive data.
  - The computers communicate, transmit - receive data through the pre-connected LAN.

## Many computers scattered far apart

- The deployment on LAN has some disadvantages in terms of requiring machines to be close to each other and the scalability is not high. To overcome that drawback, we deploy the model on a wide area network (WAN). At that time, we can flexibly coordinate many machines in remote places, and at the same time, the scalability is also enhanced when more machines are needed in the system.\par
- With the scale of this model, we further expand by deploying the server with a system of machines to increase handling capacity to avoid congestion and overload. The remaining implementations for producers and consumers are similar to the above model, the difference is that the communication will be through the WAN.
## Attached link:

- Source code github: https://github.com/antran2123153/Mult...
- Report: https://www.overleaf.com/read/dptqdqv...
- Slide: https://docs.google.com/presentation/...
- Video: https://youtu.be/aAymK9hcAZU

## Infomation:

- Zookeeper: https://zookeeper.apache.org
- Apache Kafka: https://kafka.apache.org/documentation
- Docker: https://docs.docker.com
- Docker network: https://docs.docker.com/network
- Docker compose: https://docs.docker.com/compose
- Docker Swarm:https://docs.docker.com/engine/swarm
- Kafka libary in C programming: https://github.com/edenhill/librdkafka
- OpenMP: https://www.openmp.org/wp-content/upl...
