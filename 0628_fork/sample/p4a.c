#include <stdio.h>	/* 2024.06.28 kk@numinfo.ise.chuo-u.ac.jp */
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
pthread_mutex_t mutex0=PTHREAD_MUTEX_INITIALIZER;

int total=0;

void * callback(void*arg){ int i;
    for(i=0;i<1000;i++) { int tmp;
        pthread_mutex_lock(&mutex0);
        tmp=total;
            printf("#(callback func)\n");
        tmp=tmp+5000;
        total=tmp;
        pthread_mutex_unlock(&mutex0);
            printf("# total=%d\n", total);
    }
    return NULL;
}
int main(){ int i,p1=-1,p2=-1;
    pthread_t   t1,t2;
    pthread_attr_t      t1_attr,t2_attr;
    pthread_attr_init(&t1_attr);
    pthread_attr_init(&t2_attr);

    if( (p1=pthread_create(&t1, &t1_attr,callback,NULL))!=0 ) {
       printf("p1=%d\n", p1); exit(1); }

    if( (p2=pthread_create(&t2, &t2_attr,callback,NULL))!=0 ) {
       printf("p2=%d\n", p1); exit(1); }

    for(i=0;i<1000;i++) { printf("@(main)\n"); }

    pthread_join(t1,NULL); pthread_join(t2,NULL);

    printf("total=%d\n", total);
    return 0;
}
