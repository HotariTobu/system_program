#include <stdio.h>	/* 2024.06.28 kk@numinfo.ise.chuo-u.ac.jp */
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void * callback(void*arg){ int i;
    for(i=0;i<10;i++) {
        printf("#(callback func)\n");
        sleep(1);
    }
    return NULL;
}
void * callback2(void*arg){ int i;
    for(i=0;i<10;i++) {
        printf("%%(callback2 func)\n");
        sleep(1);
    }
    return NULL;
}
int main(){ int i,p1=-1,p2=-1;
    pthread_t t,t2;
    pthread_attr_t t_attr,t2_attr;
    pthread_attr_init(&t_attr);
    pthread_attr_init(&t2_attr);

    if( (p1=pthread_create(&t, &t_attr,callback,NULL))!=0 ) {
       printf("p1=%d\n", p1); exit(1); }
    if( (p2=pthread_create(&t2, &t2_attr,callback2,NULL))!=0 ) {
       printf("p2=%d\n", p1); exit(1); }
    for(i=0;i<10;i++) {
        printf("@(main)\n");
        sleep(1);
    }
    return 0;
}
