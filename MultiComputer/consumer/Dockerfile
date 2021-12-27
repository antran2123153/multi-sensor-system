FROM ubuntu:20.04

RUN apt-get update && \
    apt-get -y install gcc mono-mcs librdkafka-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src/

COPY . /src/

RUN gcc consumer.c -o consumer -lrdkafka

CMD ["./consumer"]