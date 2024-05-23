#include <iostream>
#include "VirtualMachine.hpp"

int main() {
    VirtualMachine vm;
    VirtualMachine::Debugger debugger(vm);
    
    vm.loadProgram(vm);
    vm.runProgram(vm);
    
    debugger.log();

    return 0;
}