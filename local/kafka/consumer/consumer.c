#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <librdkafka/rdkafka.h>

// Configure infomations
#define pressure_topic "pressure"
#define temperature_topic "temperature"
#define bootstrap_server "localhost:9093"
#define sasl_mechanism "SCRAM-SHA-256"
#define sasl_username "user_test1"
#define sasl_password "12345678"
#define ssl_keystore_location "D:\\kafka\\kafka.keystore.jks"
#define ssl_keystore_password "FzOGrwIE"
#define ssl_key_location "D:\\kafka\\kafka.truststore.jks"
#define ssl_key_password "FzOGrwIE"
#define group_id "id1"

static volatile sig_atomic_t run = 1;
rd_kafka_conf_t *config;
char errstr[512];

static void stop(int sig)
{
    run = 0;
}

static int is_printable(const char *buf, size_t size)
{
    size_t i;

    for (i = 0; i < size; i++)
        if (!isprint((int)buf[i]))
            return 0;

    return 1;
}

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
    rd_kafka_conf_set(config, "group.id", group_id, errstr, sizeof(errstr));
    rd_kafka_conf_set(config, "auto.offset.reset", "earliest", errstr, sizeof(errstr));
}

int main(int argc, char **argv)
{
    rd_kafka_t *consumer;
    rd_kafka_topic_partition_list_t *subscription;
    rd_kafka_resp_err_t err;
    int i;

    setConfig();

    consumer = rd_kafka_new(RD_KAFKA_CONSUMER, config, errstr, sizeof(errstr));

    if (!consumer)
    {
        fprintf(stderr, "%% Failed to create new consumer: %s\n", errstr);
        return 1;
    }

    config = NULL;

    rd_kafka_poll_set_consumer(consumer);

    subscription = rd_kafka_topic_partition_list_new(2);
    rd_kafka_topic_partition_list_add(subscription, pressure_topic, RD_KAFKA_PARTITION_UA);
    rd_kafka_topic_partition_list_add(subscription, temperature_topic, RD_KAFKA_PARTITION_UA);

    err = rd_kafka_subscribe(consumer, subscription);
    if (err)
    {
        fprintf(stderr, "%% Failed to subscribe to %d topics: %s\n", subscription->cnt, rd_kafka_err2str(err));
        rd_kafka_topic_partition_list_destroy(subscription);
        rd_kafka_destroy(consumer);
        return 1;
    }

    fprintf(stderr, "%% Subscribed to %d topic(s), waiting for rebalance and messages...\n", subscription->cnt);

    rd_kafka_topic_partition_list_destroy(subscription);

    signal(SIGINT, stop);

    while (run)
    {
        rd_kafka_message_t *rkm;

        rkm = rd_kafka_consumer_poll(consumer, 100);
        if (!rkm)
            continue;
        if (rkm->err)
        {
            fprintf(stderr, "%% Consumer error: %s\n", rd_kafka_message_errstr(rkm));
            rd_kafka_message_destroy(rkm);
            continue;
        }

        printf("Message on %s [%" PRId32 "] at offset %" PRId64 ":\n", rd_kafka_topic_name(rkm->rkt), rkm->partition, rkm->offset);

        if (rkm->key && is_printable(rkm->key, rkm->key_len))
            printf(" Key: %.*s\n", (int)rkm->key_len, (const char *)rkm->key);
        else if (rkm->key)
            printf(" Key: (%d bytes)\n", (int)rkm->key_len);

        if (rkm->payload && is_printable(rkm->payload, rkm->len))
            printf(" Value: %.*s\n", (int)rkm->len, (const char *)rkm->payload);
        else if (rkm->payload)
            printf(" Value: (%d bytes)\n", (int)rkm->len);

        rd_kafka_message_destroy(rkm);
    }

    fprintf(stderr, "%% Closing consumer\n");
    rd_kafka_consumer_close(consumer);

    rd_kafka_destroy(consumer);

    return 0;
}