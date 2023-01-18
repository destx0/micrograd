#include <pthread.h>
#include <semaphore.h>

#include <iostream>

using namespace std;

sem_t mutex, wrt;
int readcount = 0;

void *reader(void *arg) {
    int id = *((int *)arg);
    sem_wait(&mutex);
    readcount++;
    if (readcount == 1) {
        sem_wait(&wrt);
    }
    sem_post(&mutex);

    cout << "Reader " << id << " is reading" << endl;

    sem_wait(&mutex);
    readcount--;
    if (readcount == 0) {
        sem_post(&wrt);
    }
    sem_post(&mutex);
}

void *writer(void *arg) {
    int id = *((int *)arg);
    sem_wait(&wrt);

    cout << "Writer " << id << " is writing" << endl;

    sem_post(&wrt);
}

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_t r1, r2, r3, w1, w2;
    int r1id = 1, r2id = 2, r3id = 3, w1id = 1, w2id = 2;

    pthread_create(&r1, NULL, reader, &r1id);
    pthread_create(&r2, NULL, reader, &r2id);
    pthread_create(&r3, NULL, reader, &r3id);
    pthread_create(&w1, NULL, writer, &w1id);
    pthread_create(&w2, NULL, writer, &w2id);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
