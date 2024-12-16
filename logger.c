#include <windows.h>
#include <stdio.h>

HHOOK hHook = NULL;

// Funzione callback per catturare i tasti
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT *)lParam;
        if (kbd->vkCode == VK_ESCAPE) {
            PostQuitMessage(0); // Esci se viene premuto ESC
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
    printf("Premi ESC per uscire...\n");

    // Installa l'hook globale
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

    // Rimuovi l'hook
    UnhookWindowsHookEx(hHook);
    return 0;
}
