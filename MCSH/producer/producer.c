#include "./random.h"
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <librdkafka/rdkafka.h>

rd_kafka_conf_t *config;
char errstr[512];

void setConfig()
{
    config = rd_kafka_conf_new();
    rd_kafka_conf_set(config, "bootstrap.servers", getenv("BOOTSTRAP_SERVER"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "sasl.mechanism", getenv("SASL_MECHANISM"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "sasl.username", getenv("SASL_USERNAME"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "sasl.password", getenv("SASL_PASSWORD"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.keystore.location", getenv("SSL_KEYSTORE_LOCATION"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.keystore.password", getenv("SSL_KEYSTORE_PASSWORD"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.key.location", getenv("SSL_KEY_LOCATION"), errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.key.password", getenv("SSL_KEY_PASSWORD"), errstr, sizeof(errstr));
}

int main(int argc, char **argv)
{
    rd_kafka_t *producer;
    rd_kafka_resp_err_t err;
    size_t len;

    setConfig();

    producer = rd_kafka_new(RD_KAFKA_PRODUCER, config, errstr, sizeof(errstr));
    if (!producer)
    {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        return 1;
    }

    // srand((unsigned int)time(NULL));

#pragma omp parallel
    {
        omp_set_num_threads(getenv("OMP_NUM_THREADS"));
        while (1)
        {
            // send temperature
            char buf1[512];
            randomTemperature(buf1);
            len = strlen(buf1);

        reSendTemperature:
            err = rd_kafka_producev(
                producer,
                RD_KAFKA_V_TOPIC(getenv("TEMPERATURE_TOPIC")),
                RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                RD_KAFKA_V_VALUE(buf1, len),
                RD_KAFKA_V_OPAQUE(NULL),
                RD_KAFKA_V_END);

            if (err && err == RD_KAFKA_RESP_ERR__QUEUE_FULL)
            {
                rd_kafka_poll(producer, 1000);
                goto reSendTemperature;
            }

            // send pressure
            char buf2[512];
            randomPressure(buf2);
            len = strlen(buf2);

        reSendPressure:
            err = rd_kafka_producev(
                producer,
                RD_KAFKA_V_TOPIC(getenv("PRESSURE_TOPIC")),
                RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                RD_KAFKA_V_VALUE(buf2, len),
                RD_KAFKA_V_OPAQUE(NULL),
                RD_KAFKA_V_END);

            if (err && err == RD_KAFKA_RESP_ERR__QUEUE_FULL)
            {
                rd_kafka_poll(producer, 1000);
                goto reSendPressure;
            }
        }
    }

    rd_kafka_poll(producer, 0);
    rd_kafka_flush(producer, 10 * 1000);
    rd_kafka_destroy(producer);

    return 0;
}