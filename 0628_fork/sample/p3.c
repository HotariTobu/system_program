#include <stdio.h> 			/* 2024.06.28  kk@numinfo.ise.chuo-u.ac.jp */
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int total=0;  /* グローバル変数 スレッドで共有できる. */
void * callback(void*arg){ int i; /* スレッドで共有 */
    for(i=0;i<10;i++) { int tmp; char c=(*(int*)arg)==1?'#':'%';
        tmp=total;
        printf("%c(callback func)\n",c);
        tmp=tmp+5000;
        sleep(1);  /* 普通はこのような sleep(1) 呼び出しはしない */ 
        total=tmp;
        printf("%c total=%d\n",c, total);
    }
    return NULL;
}
int main(){
    int i,p1=-1,p2=-1,a1=1,a2=2;
    pthread_t t1,t2;
    pthread_attr_t t1_attr,t2_attr;
    pthread_attr_init(&t1_attr); pthread_attr_init(&t2_attr);

    if( (p1=pthread_create(&t1, &t1_attr,callback,&a1))!=0 ) { printf("p1=%d\n", p1); exit(1); }
    if( (p2=pthread_create(&t2, &t2_attr,callback,&a2))!=0 ) { printf("p2=%d\n", p2); exit(1); }
    for(i=0;i<10;i++) { printf("@(main)\n"); sleep(1); }
    pthread_join(t1,NULL);  /* join: スレッド t の終了を待つ */
    pthread_join(t2,NULL); /* join: スレッド t2 の終了を待つ */
    printf("total=%d\n", total);
    return 0;
}
