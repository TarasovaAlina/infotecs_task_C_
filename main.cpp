#include "include/Library.h"

int main() {
    // FileLogger lib("new_file.txt", MESSAGE_IMPORTANCE::LOW);

    SocketLogger lib("127.0.0.1", "3490", MESSAGE_IMPORTANCE::LOW);

    lib.addMessageToLog(MESSAGE_IMPORTANCE::MEDIUM, "Brbrbr");

    return 0;
}