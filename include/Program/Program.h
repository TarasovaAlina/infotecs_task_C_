#ifndef PROGRAM_H
#define PROGRAM_H

#include "Library/Library.h"
#include "ThreadClasses/ThreadClasses.h"

class Program {
public:
    Program(int argc, char **argv) noexcept;

    void processRequests() noexcept;

private:

    std::unique_ptr<BaseLogger> LogLibrary;
    ThreadQueue m_q_;

};

#endif