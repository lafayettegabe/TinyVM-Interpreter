#include <iostream>
#include "VirtualMachine.hpp"

int main() {
    VirtualMachine vm;
    VirtualMachine::Debugger debugger;

    debugger.run(vm);
    
    return 0;
}