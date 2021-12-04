FROM ubuntu:20.04

RUN apt-get update && \
    apt-get -y install gcc mono-mcs librdkafka-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src/

COPY ./kafka/producer/producer.c /src/
COPY ./kafka/producer/random.h /src/

RUN gcc -o producer producer.c -lrdkafka -fopenmp -lm

CMD ["./producer"]
