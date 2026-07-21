#include "Library/Library.h"
#include "Program/Program.h"

int main(int argc, char **argv) {

    Program program_(argc, argv);
    program_.processRequests();

    return 0;
}