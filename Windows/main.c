#include "tools.h"

int main(int argc, char** argv){
    printf("%s",argv[1]);
    loggerBody();
    printf("Finito Logger");
    initialiseSocketClient(argv[1]);
//    initialiseSocketClient("192.168.178.175");
    return 0;
}