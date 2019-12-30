#include <iostream>
#include "../cpp-virtual-machine/VirtualMachine.cpp"
#include "SourceFile.h"

int main() {
	VirtualMachine vm(0);
	vm.lookup;

	std::cout << "Hello assembler!" << std::endl;

	SourceFile source("C:\\Users\\Isomorphism\\Documents\\Repos\\cpp-virtual-machine\\x64\\Debug\\print_hearts.vasm");

	char ch = 0;
	while (!source.end()) {
		ch = source.next_char();
		std::cout << ch << std::endl;
	}
	source.put_back(ch);

	system("pause");
}