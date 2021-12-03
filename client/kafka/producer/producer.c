#include <stdio.h>
#include <omp.h>
#include <librdkafka/rdkafka.h>
#include "../../sensors/random.h"
#include <string.h>
// #include <time.h>

#define OMP_NUM_THREADS 4

// Configure infomations
const char *pressure_topic = "pressure";
const char *temperature_topic = "temperature";
const char *bootstrap_server = "localhost:9092";
const char *sasl_mechanism = "SCRAM-SHA-256";
const char *sasl_username = "user_test1";
const char *sasl_password = "12345678";
const char *ssl_keystore_location = "D:\\kafka\\kafka.keystore.jks";
const char *ssl_keystore_password = "FzOGrwIE";
const char *ssl_key_location = "D:\\kafka\\kafka.truststore.jks";
const char *ssl_key_password = "FzOGrwIE";

rd_kafka_conf_t *config;
char errstr[512];

void setConfig()
{
    config = rd_kafka_conf_new();
    rd_kafka_conf_set(config, "bootstrap.servers", bootstrap_server, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "sasl.mechanism", sasl_mechanism, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "sasl.username", sasl_username, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "sasl.password", sasl_password, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.keystore.location", ssl_keystore_location, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.keystore.password", ssl_keystore_password, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.key.location", ssl_key_location, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "ssl.key.password", ssl_key_password, errstr, sizeof(errstr));
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
        omp_set_num_threads(OMP_NUM_THREADS);
        while (1)
        {
            // send temperature
            char buf1[512];
            randomTemperature(buf1);
            len = strlen(buf1);

        reSendTemperature:
            err = rd_kafka_producev(
                producer,
                RD_KAFKA_V_TOPIC(temperature_topic),
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
                RD_KAFKA_V_TOPIC(pressure_topic),
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