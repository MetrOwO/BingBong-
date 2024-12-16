#include <windows.h>
#include <stdio.h>

void sendKeystroke(WORD key) {
    // Simula pressione del tasto
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key; // Virtual-Key code (es. 'A' è VK_A)
    SendInput(1, &input, sizeof(INPUT));

    // Simula rilascio del tasto
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

int main() {
    printf("Invio del tasto 'A'...\n");
    Sleep(1000); // Aspetta un secondo
    sendKeystroke(0x30); // VK_A per 'A'
    printf("Fatto!\n");
    return 0;
}
