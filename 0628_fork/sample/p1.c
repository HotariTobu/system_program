#include <stdio.h>	/* 2024.06.28 kk@numinfo.ise.chuo-u.ac.jp */
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void * callback(void*arg){ int i; /* コールバック関数 */
    for(i=0;i<10;i++) {
        printf("#(callback func)\n");
        sleep(1);
    }
    return NULL;
}
int main(){ int i,p1=-1;
    pthread_t t;
    pthread_attr_t t_attr;
    pthread_attr_init(&t_attr);

    if((p1=pthread_create(&t,&t_attr,callback,NULL))!=0){/*スレッド作成*/
       printf("p1=%d\n", p1); exit(1); }
    for(i=0;i<10;i++) {
        printf("@(main)\n");
        sleep(1); }
    return 0;
}
