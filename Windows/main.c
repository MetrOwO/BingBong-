#include "tools.h"

int main(int argc, char** argv){
    printf("%s",argv[1]);
    loggerBody();
    printf("Finito Logger");
    initialiseSocketClient(argv[1]);
    return 0;
}