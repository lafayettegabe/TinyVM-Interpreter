#ifndef VM
#define VM

#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <fstream>

typedef struct memory_t {
    int opcode;
    int operand;
} memory_t;

class VirtualMachine {
private:
    std::vector<memory_t> memory;
    std::vector<int> registers;
    int programCounter, accumulator;

public:
    VirtualMachine() : 
        memory(128, {0, 0}),
        registers(2, 0), 
        programCounter(0), 
        accumulator(0) 
    {
        std::cout << "VirtualMachine constructor called" << std::endl;
    }

    void loadProgram(VirtualMachine &vm) {
        std::ifstream file("prog.txt");
        if (file.is_open()) {
            int address = 0;
            while (!file.eof()) {
                int opcode, operand;
                file >> opcode >> operand;
                vm.memory[address].opcode = opcode;
                vm.memory[address].operand = operand;
                vm.programCounter++;
                address++;
                if (address >= vm.memory.size()) {
                    break; // Stop reading if memory is full
                }
            }
            file.close();
            std::cout << "Program loaded successfully with " << vm.programCounter << " instructions" << std::endl;
        } else {
            std::cerr << "Unable to open file prog.txt" << std::endl;
        }
    }

    void runProgram(VirtualMachine &vm) {
        const int MemoryStart = vm.programCounter;

        // just log the memory start
        std::cout << "Memory Start: " << MemoryStart << std::endl;
    }

    class Debugger {
    public:
        Debugger() {
            std::cout << std::endl;
            std::cout << "Debugger constructor called" << std::endl;
            std::cout << std::endl;
            dash_separator();
        }

        void run(VirtualMachine &vm) {

            // just to test the debugger
            // fillMemoryRandom(vm);
            
            displayRegisters(vm);
            displayProgramCounter(vm);
            displayAccumulator(vm);
            displayMemory(vm);
        }

        void dash_separator() {
            std::cout << "|" << std::setw(86) << std::setfill('-') << "|" << std::endl;
        }

        void empty_line() {
            std::cout << "|" << std::setw(86) << std::setfill(' ') << "|" << std::endl;
        }

        void fillMemoryRandom(VirtualMachine &vm) {
            // Use a random device to seed the random number generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 255); // Generate random integers between 0 and 255

            // Iterate over the memory vector and fill it with random values
            for (int i = 0; i < vm.memory.size(); ++i) {
                vm.memory[i].opcode = static_cast<uint8_t>(dis(gen));
                vm.memory[i].operand = static_cast<uint8_t>(dis(gen));
            }
        }

        void displayMemory(const VirtualMachine &vm) {
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
                        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(vm.memory[j].opcode);
                        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(vm.memory[j].operand);
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

        void displayRegisters(const VirtualMachine &vm) {
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

        void displayProgramCounter(const VirtualMachine &vm) {
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

        void displayAccumulator(const VirtualMachine &vm) {
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
            dash_separator();
            std::cout << std::endl;
            std::cout << "Debugger destructor called" << std::endl;
        }
    };

    ~VirtualMachine() {
        std::cout << "VirtualMachine destructor called" << std::endl;
    }
};
#endif