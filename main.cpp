#include <iostream>
#include <unistd.h>
using  namespace std;

//variables
int maxBags = 400;
int silo = 0;
int bodega = 0;
pthread_mutex_t sumMutex;
pthread_cond_t condSilo;


void* tostadora(void* args){
    while(maxBags > 0){
        sleep(1);
        pthread_mutex_lock(&sumMutex);

        cout << "Libras en el silo: "<<silo << endl;
        silo++;
        maxBags--;

        pthread_mutex_unlock(&sumMutex);
        pthread_cond_signal(&condSilo);
    }
}

void* empacadora(void* args){
    pthread_mutex_lock(&sumMutex);
    while (silo < 5){
        cout<< "Libras en el silo son  menores a 5.  Esperando... " << endl;
        pthread_cond_wait(&condSilo,&sumMutex);
    }
    pthread_mutex_unlock(&sumMutex);

    while(silo > 1){
        pthread_mutex_lock(&sumMutex);

        bodega += 1;
        silo -= 1;
        cout << "Bolsas en bodega: " << bodega << endl;

        pthread_mutex_unlock(&sumMutex);
        sleep(1);
    }


}

int main() {
    //threads
    pthread_mutex_init(&sumMutex, NULL);
    pthread_cond_init(&condSilo, NULL);

    pthread_t thread_id[3];

    pthread_create(&thread_id[0], nullptr, tostadora, nullptr);
    pthread_create(&thread_id[1], nullptr, tostadora, nullptr);
    pthread_create(&thread_id[2], nullptr, empacadora, nullptr);

    for(auto thread : thread_id){
        pthread_join(thread, nullptr);
    }

    return 0;
}
