#include <iostream>
#include "VirtualMachine.hpp"

int main() {
    // Run the program
    VirtualMachine vm;
    
    vm.loadProgram(vm);
    vm.runProgram(vm);
    // ------------------------------------------------
    // Debug the results
    VirtualMachine::Debugger debugger;
    
    debugger.run(vm);
    // ------------------------------------------------   
    return 0;
}