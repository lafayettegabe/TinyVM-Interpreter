#ifndef VM
#define VM

#include <iostream>
#include <vector>
#include <iomanip>
#include <random>

class VirtualMachine {
private:
    std::vector<int> memory, registers;
    int programCounter, accumulator;

public:
    VirtualMachine() : 
        memory(128, 0), 
        registers(2, 0), 
        programCounter(0), 
        accumulator(0) 
    {
        std::cout << "VirtualMachine constructor called" << std::endl;
    }

    class Debugger {
    public:
        Debugger() {
            std::cout << "Debugger constructor called" << std::endl;
            dash_separator();
        }

        void run(VirtualMachine &vm) {

            // just to test the debugger
            fillMemoryRandom(vm);
            
            displayRegisters(vm);
            displayProgramCounter(vm);
            displayAccumulator(vm);
            displayMemoryHex(vm);
        }

        void dash_separator() {
            std::cout << "|" << std::setw(65) << std::setfill('-') << "|" << std::endl;
        }

        void empty_line() {
            std::cout << "|" << std::setw(65) << std::setfill(' ') << "|" << std::endl;
        }

        void fillMemoryRandom(VirtualMachine &vm) {
            // Use a random device to seed the random number generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 255); // Generate random integers between 0 and 255

            // Iterate over the memory vector and fill it with random values
            for (int i = 0; i < vm.memory.size(); ++i) {
                vm.memory[i] = dis(gen); // Generate a random value and assign it to memory[i]
            }
        }

        void displayMemoryHex(const VirtualMachine &vm) {
            // Private functions ----------------------------------|
            auto title = [this]() {
                std::cout << "|" << std::setw(24) << std::setfill(' ') << ' ' << "Memory Dump (Hex)" << std::setw(23) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto row_legend = [this](int by_per_group) {
                empty_line();
                std::cout << "| " << std::setw(15) << std::setfill(' ') << ' ';
                for (int j = 0; j < by_per_group; j++) {
                    std::cout << std::setw(2) << std::setfill('x') << std::hex << std::uppercase << j;
                    if (j < by_per_group - 1) {
                        std::cout << std::setfill(' ') << ' ';
                    }
                }
                std::cout << " |" << std::endl;
                empty_line();
            };

            auto memory_group = [this](const VirtualMachine &vm, int start, int end) {
                std::cout << "| Memory[" << std::setw(2) << std::setfill('0') << start << "-" 
                        << std::setw(2) << std::setfill('0') << end << "]: ";

                for (int j = start; j <= end; j++) {
                    std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << vm.memory[j];
                    if (j < end) {
                        std::cout << std::setfill(' ') << ' ';
                    } else {
                        std::cout << " |";
                    }
                }
                std::cout << std::endl;
            };

            // -----------------------------------------------------|
            dash_separator();
            // Execution -------------------------------------------|

            constexpr int BYTES_PER_GROUP = 16;
            constexpr int MEMORY_SIZE = 128;

            title();
            row_legend(BYTES_PER_GROUP);
            
            for (int i = 0; i < MEMORY_SIZE; i += BYTES_PER_GROUP) {
                memory_group(vm, i, i + BYTES_PER_GROUP - 1);
            }

            // End of execution ------------------------------------|
            dash_separator();
            // -----------------------------------------------------|
        }

        void displayRegisters(const VirtualMachine &vm) {
            // Private functions ----------------------------------|
            auto title = [this]() {
                std::cout << "|" << std::setw(24) << std::setfill(' ') << ' ' << "Registers" << std::setw(31) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto display_register = [this](const VirtualMachine &vm, int i) {
                std::cout << "| R" << i << " = " << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << vm.registers[i] << std::setw(56) << std::setfill(' ') << ' ' << "|" << std::endl;
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
                std::cout << "|" << std::setw(24) << std::setfill(' ') << ' ' << "Program Counter" << std::setw(25) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto display_program_counter = [this](const VirtualMachine &vm) {
                std::cout << "| PC = " << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << vm.programCounter << std::setw(56) << std::setfill(' ') << ' ' << "|" << std::endl;
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
                std::cout << "|" << std::setw(24) << std::setfill(' ') << ' ' << "Accumulator" << std::setw(29) << std::setfill(' ') << ' ' << "|" << std::endl;
            };

            auto display_accumulator = [this](const VirtualMachine &vm) {
                std::cout << "| AC = " << std::setw(2) << std::setfill('0') << vm.accumulator << std::setw(56) << std::setfill(' ') << ' ' << "|" << std::endl;
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
            std::cout << "Debugger destructor called" << std::endl;
        }
    };

    // Getter functions for private members
    const std::vector<int>& getMemory() const { return memory; }
    const std::vector<int>& getRegisters() const { return registers; }
    int getProgramCounter() const { return programCounter; }
    int getAccumulator() const { return accumulator; }

    ~VirtualMachine() {
        std::cout << "VirtualMachine destructor called" << std::endl;
    }
};
#endif