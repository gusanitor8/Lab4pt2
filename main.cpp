#include <iostream>
#include <unistd.h>
using  namespace std;

//variables
int silo = 0;
int bodega = 0;
pthread_mutex_t sumMutex;
pthread_cond_t condSilo;


void* tostadora(void* args){
    sleep(1);
    cout << "Holii" << endl;
    pthread_mutex_lock(&sumMutex);
    silo++;
    pthread_mutex_unlock(&sumMutex);
}

void* empacadora(void* args){
    while (silo < 5){
        cout<< "Libras en el silo son  menores a 5.  Esperando... " << endl;
        pthread_cond_wait(&condSilo,&sumMutex);
    }
    bodega+=5;
    pthread_mutex_unlock(&sumMutex);
}

int main() {
    //threads
    pthread_mutex_init(&sumMutex, NULL);
    pthread_cond_init(&condSilo, NULL);

    pthread_t thread_id;
    pthread_attr_t attr;

    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    pthread_create(&thread_id, &attr, tostadora, nullptr);
    pthread_join(thread_id, nullptr);

    pthread_create(&thread_id, &attr, tostadora, nullptr);
    pthread_join(thread_id, nullptr);

    pthread_create(&thread_id, &attr, empacadora, nullptr);
    pthread_join(thread_id, nullptr);

    return 0;
}
