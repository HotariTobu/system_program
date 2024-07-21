#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    setegid(20);
    seteuid(1000);
    printf("uid=%d euid=%d gid=%d egid=%d\n",
        getuid(),geteuid(),getgid(),getegid());
    creat("A1.dat",0544);
    seteuid(0); setegid(10);
    creat("A2.dat",04544);
}
