#include <stdio.h>
#include <string.h>

int main() {
    const char *text = "Hello_World!";
    printf("text: %s\n", text);

    char *r1 = strstr(text, "and");
    printf("and: %s\n", r1);

    char *r2 = strstr(text, "or");
    printf("or: %s\n", r2);
}
