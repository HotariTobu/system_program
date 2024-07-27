#include <stdio.h>
#include <time.h>

int main() {
    time_t t = time(NULL);
    char *s = asctime(localtime(&t));
    printf("%s", s);
}
