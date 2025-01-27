#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <chrono>
#include <thread>

const int max_orders = 100;
const int max_order_length = 200;

typedef struct {
    int order_number;
    char product_name[50];
    int quantity;
    float sum;
    char producer_timestamp[20];
    char consumer_id[5];
    char consumer_timestamp[20];
} Order;

Order orders[max_orders];
int order_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void getTimestamp(char *timestamp) {
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", local_time);
}

void sleepMilliseconds(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void *producer(void *arg) {
    FILE *file = fopen("orders.csv", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[max_order_length];
    while (fgets(line, sizeof(line), file)) {
        pthread_mutex_lock(&mutex);
        if (order_count < max_orders) {
            if (sscanf(line, "%d,%49[^,],%d,%f\n",
                &orders[order_count].order_number,
                orders[order_count].product_name,
                &orders[order_count].quantity,
                &orders[order_count].sum) == 4) {
                getTimestamp(orders[order_count].producer_timestamp);
                order_count++;
            } else {
                fprintf(stderr, "Error parsing line: %s\n", line);
            }
        } else {
            fprintf(stderr, "Error: Maximum orders reached.\n");
            break;
        }
        pthread_mutex_unlock(&mutex);
    }

    fclose(file);
    return NULL;
}

FILE *processed_file;

void *consumer(void *arg) {
    int consumer_id = *(int *)arg;
    char consumer_id_str[5];
    sprintf(consumer_id_str, "%d", consumer_id);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (order_count > 0) {
            strcpy(orders[order_count - 1].consumer_timestamp, "");
            getTimestamp(orders[order_count - 1].consumer_timestamp);
            strcpy(orders[order_count - 1].consumer_id, consumer_id_str);
            fprintf(processed_file, "%d,%s,%d,%.2f,%s,%s,%s\n",
                orders[order_count - 1].order_number,
                orders[order_count - 1].product_name,
                orders[order_count - 1].quantity,
                orders[order_count - 1].sum,
                orders[order_count - 1].producer_timestamp,
                orders[order_count - 1].consumer_id,
                orders[order_count - 1].consumer_timestamp);
            printf("Consumer %d processing order: %d\n", consumer_id, orders[order_count - 1].order_number);
            order_count--;
        } else {
            pthread_mutex_unlock(&mutex); // Unlock mutex before breaking
            break;
        }
        pthread_mutex_unlock(&mutex); // Unlock mutex after processing
        sleepMilliseconds(100);
    }

    return NULL;
}

int main() {
    pthread_t prod_tid, cons_tid[2];

    processed_file = fopen("processed.csv", "a");

    if (!processed_file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    pthread_create(&prod_tid, NULL, producer, NULL);

    int i;
    int cons_ids[2];
    for (i = 0; i < 2; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&cons_tid[i], NULL, consumer, &cons_ids[i]);
    }

    pthread_join(prod_tid, NULL);
    for (i = 0; i < 2; i++) {
        pthread_join(cons_tid[i], NULL);
    }

    fclose(processed_file);
    pthread_mutex_destroy(&mutex);

    return 0;
}
