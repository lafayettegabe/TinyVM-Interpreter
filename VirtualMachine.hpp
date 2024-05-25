#ifndef VM
#define VM

#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <fstream>

class VirtualMachine {
private:
    std::vector<int> memory, registers;
    int programSize, programCounter, accumulator;

    enum Instructions {
        LA   = 0,         // Load accumulator
        SA   = 1,         // Store accumulator
        AA   = 2,         // Add accumulator
        MUL  = 3,         // Multiply accumulator
        DIV  = 4,         // Divide accumulator
        SUB  = 5,         // Subtract accumulator
        JMP  = 6,         // Jump
        JEQ  = 7,         // Jump if equal
        JGT  = 8,         // Jump if greater
        JLT  = 9,         // Jump if less
        PW   = 10,        // Print word
        RW   = 11,        // Read word
        STOP = 12         // Stop machine
    };

public:
    VirtualMachine() : 
        memory(128, 0),
        registers(2, 0), 
        programSize(0),
        programCounter(0), 
        accumulator(0) 
    {
        std::cout << "VirtualMachine initialized" << std::endl;
    }

    void loadProgram(VirtualMachine &vm) {
        std::ifstream file("prog.txt");
        if (file.is_open()) {
            int address = 0;
            while (!file.eof()) {
                int opcode, operand;
                file >> opcode >> operand;
                vm.memory[address++]= opcode;
                vm.memory[address++]= operand;
                vm.programSize += 2;
                if (address >= vm.memory.size()) {
                    break; // Stop reading if memory is full
                }
            }
            file.close();
            std::cout << "Program loaded successfully with " << vm.programSize << " instructions" << std::endl;
        } else {
            std::cerr << "Unable to open file prog.txt" << std::endl;
        }
    }

    void runProgram(VirtualMachine &vm) {
        const int MemoryStart = vm.programSize;
        const int MemoryEnd = vm.memory.size();

        std::cout << "Memory Start: " << MemoryStart << std::endl;

        int opcode, operand;
        while (opcode != STOP) {
            opcode = vm.memory[vm.programCounter];
            operand = vm.memory[vm.programCounter + 1];
            switch (opcode) {
                case LA:
                    std::cout << "Load accumulator with memory[" << operand << "]" << std::endl;
                    vm.accumulator = vm.memory[MemoryStart + operand];
                    break;
                case SA:
                    std::cout << "Store accumulator in memory[" << operand << "]" << std::endl;
                    vm.memory[MemoryStart + operand] = vm.accumulator;
                    break;
                case AA:
                    std::cout << "Add memory[" << operand << "] to accumulator" << std::endl;
                    vm.accumulator += vm.memory[MemoryStart + operand];
                    break;
                case MUL:
                    std::cout << "Multiply accumulator by memory[" << operand << "]" << std::endl;
                    vm.accumulator *= vm.memory[MemoryStart + operand];
                    break;
                case DIV:
                    std::cout << "Divide accumulator by memory[" << operand << "]" << std::endl;
                    vm.accumulator /= vm.memory[MemoryStart + operand];
                    break;
                case SUB:
                    std::cout << "Subtract memory[" << operand << "] from accumulator" << std::endl;
                    vm.accumulator -= vm.memory[MemoryStart + operand];
                    break;
                case JMP:
                    std::cout << "Jump to memory[" << operand << "]" << std::endl;
                    vm.programCounter = operand;
                    break;
                case JEQ:
                    std::cout << "Jump to memory[" << operand << "] if accumulator is zero" << std::endl;
                    if (vm.accumulator == 0) {
                        std::cout << "Accumulator is zero" << std::endl;
                        vm.programCounter = operand;
                    }
                    break;
                case JGT:
                    std::cout << "Jump to memory[" << operand << "] if accumulator is greater than zero" << std::endl;
                    if (vm.accumulator > 0) {
                        std::cout << "Accumulator is greater than zero" << std::endl;
                        vm.programCounter = operand;
                    }
                    break;
                case JLT:
                    std::cout << "Jump to memory[" << operand << "] if accumulator is less than zero" << std::endl;
                    if (vm.accumulator < 0) {
                        std::cout << "Accumulator is less than zero" << std::endl;
                        vm.programCounter = operand;
                    }
                    break;
                case PW:
                    std::cout << "Print word: " << vm.memory[MemoryStart + operand] << std::endl;
                    break;
                case RW:
                    std::cout << "Read word: ";
                    std::cin >> vm.memory[MemoryStart + operand];
                    break;
                case STOP:
                    std::cout << "Machine stopped" << std::endl;
                    break;
                default:
                    std::cerr << "Invalid opcode: " << opcode << std::endl;
                    break;
            }
            vm.programCounter += 2;
            if (vm.programCounter >= MemoryEnd) {
                std::cerr << "Program counter out of bounds" << std::endl;
                break;
            }
        }
    }

    class Debugger {
    private:
        VirtualMachine &vm;
        const std::string BLUE = "\033[34m";
        const std::string RED = "\033[31m";
        const std::string RESET = "\033[0m";
        
    public:
        Debugger(VirtualMachine &vm) : vm(vm) {
            std::cout << std::endl;
            std::cout << "Debugger initialized" << std::endl;
            std::cout << std::endl;
        }

        void log() {
            std::cout << std::endl;
            displayRegisters();
            displayProgramCounter();
            displayAccumulator();
            displayMemory();
            std::cout << std::endl;
        }

        void dash_separator() {
            std::cout << "+" << std::setw(86) << std::setfill('-') << "+" << std::endl;
        }

        void empty_line() {
            std::cout << "|" << std::setw(86) << std::setfill(' ') << "|" << std::endl;
        }

        void displayMemory() {
            // Private functions ----------------------------------|
            auto title = [this]() {
                std::cout << "|" << std::setw(35) << std::setfill(' ') << ' ' << "Memory Dump" << std::setw(39) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto row_legend = [this](int by_per_group) {
                empty_line();
                std::cout << "| " << std::setw(23) << std::setfill(' ') << ' ';
                for (int j = 0; j < by_per_group; j++) {
                    std::cout << std::setw(4) << std::setfill('0') << j;
                    if (j < by_per_group - 1) {
                        std::cout << std::setfill(' ') << ' ';
                    }
                }
                std::cout << std::setw(11) << std::setfill(' ') << ' ' << " |" << std::endl;
                empty_line();
            };

            auto memory_group = [this](const VirtualMachine &vm, int start, int end) {
                std::cout << "|  Memory[" << std::setw(3) << std::setfill('0') << start << " - " << std::setw(3) << std::setfill('0') << (end > vm.memory.size() ? vm.memory.size() : end) << "]  |  ";

                for (int j = start; j <= end; j++) {
                    if (!(j > vm.memory.size())) {
                        if (j < vm.programSize) {
                            std::cout << RED;
                        } else {
                            std::cout << BLUE;
                        }
                        if(j % 2 == 0) {
                            std::cout << std::setw(4) << std::setfill('0') << static_cast<int>(vm.memory[j]);
                        } else {
                            std::cout << std::setw(4) << std::setfill('0') << static_cast<int>(vm.memory[j]);
                        }
                        std::cout << RESET;
                        if (j < end) {
                            std::cout << std::setfill(' ') << ' ';
                        } else {
                            std::cout << std::setw(13) << std::setfill (' ') << " |";
                        }

                    } else {
                        std::cout << std::setw(17) << std::setfill (' ') << " |";
                        break;
                    }
                    
                }
                std::cout << std::endl;
            };

            // -----------------------------------------------------|
            dash_separator();
            // Execution -------------------------------------------|

            constexpr int BYTES_PER_GROUP = 10;

            title();
            row_legend(BYTES_PER_GROUP);
            
            for (int i = 0; i < vm.memory.size(); i += BYTES_PER_GROUP) {
                memory_group(vm, i, i + BYTES_PER_GROUP - 1);
            }

            // End of execution ------------------------------------|
            dash_separator();
            // -----------------------------------------------------|
        }

        void displayRegisters() {
            // Private functions ----------------------------------|
            auto title = [this]() {
                std::cout << "|" << std::setw(35) << std::setfill(' ') << ' ' << "Registers" << std::setw(41) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto display_register = [this](const VirtualMachine &vm, int i) {
                std::cout << "| R" << i << " = " << std::setw(2) << std::setfill('0') << std::uppercase << vm.registers[i] << std::setw(77) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            // -----------------------------------------------------|
            dash_separator();
            // Execution -------------------------------------------|
            
            title();
            for (int i = 0; i < vm.registers.size(); i++) {
                display_register(vm, i);
            }
            
            // End of execution ------------------------------------|
            // -----------------------------------------------------|
        }

        void displayProgramCounter() {
            auto title = [this]() {
                std::cout << "|" << std::setw(35) << std::setfill(' ') << ' ' << "Program Counter" << std::setw(35) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto display_program_counter = [this](const VirtualMachine &vm) {
                std::cout << "| PC = " << std::setw(2) << std::setfill('0') << std::uppercase << vm.programCounter << std::setw(77) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            // -----------------------------------------------------|
            dash_separator();
            // Execution -------------------------------------------|
            
            title();
            display_program_counter(vm);

            // End of execution ------------------------------------|
            // -----------------------------------------------------|
        }

        void displayAccumulator() {
            auto title = [this]() {
                std::cout << "|" << std::setw(35) << std::setfill(' ') << ' ' << "Accumulator" << std::setw(39) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto display_accumulator = [this](const VirtualMachine &vm) {
                std::cout << "| AC = " << std::setw(2) << std::setfill('0') << vm.accumulator << std::setw(77) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            // -----------------------------------------------------|
            dash_separator();
            // Execution -------------------------------------------|

            title();
            display_accumulator(vm);

            // End of execution ------------------------------------|
            // -----------------------------------------------------|
        }

        ~Debugger() {
            std::cout << "Debugger destructor called" << std::endl;
        }
    };

    ~VirtualMachine() {
        std::cout << "VirtualMachine destructor called" << std::endl;
    }
};
#endif