//
// Created by os on 8/22/23.
//

#ifndef PROJECT_BASE_V1_1_CONSOLEBUFFER_HPP
#define PROJECT_BASE_V1_1_CONSOLEBUFFER_HPP

#include "boundedBuffer.hpp"

class consoleBuffer {
public:
    void putIn(char c);
    void putOut(char c);

    char getIn();
    char getOut();

    static consoleBuffer& getInstance();

private:
    consoleBuffer() {
        inputBuffer = new boundedBuffer();
        outputBuffer = new boundedBuffer();
    }

    boundedBuffer *inputBuffer, *outputBuffer;

    consoleBuffer(const consoleBuffer&) = delete;
   // consoleBuffer& operator=(const consoleBuffer&) = delete;

};


#endif //PROJECT_BASE_V1_1_CONSOLEBUFFER_HPP
