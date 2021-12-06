FROM ubuntu:20.04

RUN apt-get update && \
    apt-get -y install gcc mono-mcs librdkafka-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src/

COPY ./producer.c /src/
COPY ./random.h /src/

RUN gcc -o producer producer.c -lrdkafka -fopenmp -lm

CMD ["./producer"]