//
// Created by os on 8/22/23.
//

#include "../h/consoleBuffer.hpp"

char consoleBuffer::getIn() {
    return inputBuffer->take();
}

char consoleBuffer::getOut() {
    return outputBuffer->take();
}

void consoleBuffer::putOut(char c){
    outputBuffer->append(c);
}

void consoleBuffer::putIn(char c){
    inputBuffer->append(c);
}

consoleBuffer& consoleBuffer::getInstance() {
    static consoleBuffer instance;
    return instance;
}