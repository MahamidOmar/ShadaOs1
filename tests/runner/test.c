#include <signal.h>
#include <stdio.h>

void PRINT() {
    printf("RECIEVED!\n");
}

int main()
{
    int i;

    signal(SIGINT, PRINT);

    while (1) {}
    return 0;
}
