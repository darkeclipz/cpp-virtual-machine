#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(int memory_size) {

	for (int i = 0; i < 256; i++) {
		lookup.push_back({ "NOP", &VirtualMachine::NOP, 1, "NOP" });
	}

	lookup[0x05] = { "HLT", &VirtualMachine::HLT, 1, "HLT", OPCODE_ARGUMENTS::NONE };
	lookup[0x10] = { "MOVRC", &VirtualMachine::MOVRC, 4, "MOV", OPCODE_ARGUMENTS::RC };
	lookup[0x11] = { "MOVRR", &VirtualMachine::MOVRR, 3, "MOV", OPCODE_ARGUMENTS::RR };
	lookup[0x19] = { "MOVPR", &VirtualMachine::MOVPR, 3, "MOV", OPCODE_ARGUMENTS::PR };
	lookup[0x20] = { "JMPC", &VirtualMachine::JMPC, 3, "JMP", OPCODE_ARGUMENTS::C };
	lookup[0x40] = { "ADDRC", &VirtualMachine::ADDRC, 4, "ADD", OPCODE_ARGUMENTS::RC };

	//lookup[0x12] = { "MOVRM", &VirtualMachine::MOVRM, 3, "MOV",  };
	//lookup[0x15] = { "MOVMR", &VirtualMachine::MOVMR, 4, "MOV" };
	//lookup[0x16] = { "MOVMM", &VirtualMachine::MOVMM, 3, "MOV" };
	//lookup[0x50] = { "SHLRC", &VirtualMachine::SHLRC, 4, "SHL" };

	m_memory_size = memory_size;
	m_memory = new uint8_t[m_memory_size]{ 0x0 };

	reset();
}

VirtualMachine::~VirtualMachine() {
	delete[] m_memory;
	lookup.clear();
}

uint16_t VirtualMachine::read_reg(uint8_t reg) {
	switch (reg) {
	case REGISTERS::AX:	return ax;
	case REGISTERS::BX:	return bx;
	case REGISTERS::CX:	return cx;
	case REGISTERS::DX:	return dx;
	case REGISTERS::SP:	return sp;
	case REGISTERS::SB:	return sb;
	case REGISTERS::BP: return bp;
	case REGISTERS::IP:	return ip;
	default:
		throw std::invalid_argument("invalid register");
	}
}

void VirtualMachine::write_reg(uint8_t reg, uint16_t value) {
	switch (reg) {
	case REGISTERS::AX:	ax = value; break;
	case REGISTERS::BX:	bx = value; break;
	case REGISTERS::CX:	cx = value; break;
	case REGISTERS::DX:	dx = value; break;
	case REGISTERS::SP:	sp = value; break;
	case REGISTERS::BP:	bp = value; break;
	case REGISTERS::IP:	ip = value; break;
	default:
		throw std::invalid_argument("invalid register");
	}
}

uint8_t VirtualMachine::read_flag(uint8_t flag) {
	return (m_flags & flag) > 0 ? 1 : 0;
}

void VirtualMachine::write_flag(uint8_t flag, bool value) {
	if (value) {
		m_flags |= flag;
	}
	else {
		m_flags &= ~flag;
	}
}

uint8_t VirtualMachine::read_mem(uint16_t address) {
	if (address < 0 || address > m_memory_size) {
		return 0x00;
	}
	return m_memory[address];
}

void VirtualMachine::write_mem(uint16_t address, uint8_t value) {
	if (address < 0 || address > m_memory_size) {
		return;
	}
	m_memory[address] = value;
}

inline uint8_t VirtualMachine::read_ptr(uint8_t reg) {
	uint16_t addr = read_reg(reg);
	return read_mem(addr);
}

inline void VirtualMachine::write_ptr(uint8_t reg, uint8_t val) {
	uint16_t addr = read_reg(reg);
	write_mem(addr, val);
}

inline uint16_t VirtualMachine::read_int(uint8_t addr) {
	return read_mem(addr) << 8 | read_mem(addr + 1);
}

inline void VirtualMachine::write_int(uint8_t addr, uint16_t val) {
	write_mem(addr, val & 0xFF00);
	write_mem(addr + 1, val & 0x00FF);
}

void VirtualMachine::clock() {
	opcode = m_memory[ip];
	(this->*lookup[opcode].operate)();
	ip += lookup[opcode].instruction_size;
	if (ip > m_memory_size) {
		ip = 0x00;
		write_flag(FLAGS::B, true);
	}
}

void VirtualMachine::step() {
	clock();
}

void VirtualMachine::reset() {
	ax = 0; bx = 0; cx = 0; dx = 0; bp = 0;
	sb = m_memory_size - 1; sp = sb;  ip = 0; 
	m_flags = FLAGS::B;
}

void VirtualMachine::load_rom(uint8_t* rom, int size, uint8_t address) {
	for (int i = 0; i < size; i++) {
		write_mem(address + i, rom[i]);
	}
}

// ======== OPCODES ==================================================================

void VirtualMachine::HLT() {
	write_flag(B, true);
}

void VirtualMachine::NOP() {
	// no operation
}

void VirtualMachine::MOVRR() {  // MOV reg, reg
	uint8_t r1 = read_mem(ip + 1);
	uint8_t r2 = read_mem(ip + 2);
	uint16_t value = read_reg(r2);
	write_reg(r1, value);
}

void VirtualMachine::MOVRM() {  // MOV reg, mem
	uint8_t reg = read_mem(ip + 1);
	uint8_t addr = read_mem(ip + 2);
	uint8_t value = read_mem(addr);
	write_reg(reg, value);
}

void VirtualMachine::MOVMR() {  // MOV mem, reg
	uint8_t addr = read_mem(ip + 1);
	uint8_t reg = read_mem(ip + 3);
	uint16_t value = read_reg(reg);
	write_mem(addr, value & 0x00ff);
}

void VirtualMachine::MOVMM() {  // MOV mem, mem
	uint8_t addr1 = read_mem(ip + 1);
	uint8_t addr2 = read_mem(ip + 2);
	uint8_t value = read_mem(addr2);
	write_mem(addr1, value);
}

void VirtualMachine::MOVRC() {  // MOV reg, const
	uint8_t reg = read_mem(ip + 1);
	uint16_t val = read_int(ip + 2);// read_mem(ip + 2) << 8 | read_mem(ip + 3);
	write_reg(reg, val);
}

void VirtualMachine::JMPC() {
	uint16_t addr = read_mem(ip + 1) << 8 | read_mem(ip + 2);
	write_reg(REGISTERS::IP, addr - 3 + read_reg(REGISTERS::BP));
}

void VirtualMachine::ADDRC() {  // ADD reg, const
	uint8_t reg = read_mem(ip + 1);
	uint16_t val = read_int(ip + 2); // read_mem(ip + 2) << 8 | read_mem(ip + 3);
	uint16_t current_value = read_reg(reg);
	write_reg(reg, current_value + val);
}

void VirtualMachine::MOVPR() {
	uint8_t ptr = read_mem(ip + 1);
	uint8_t reg = read_mem(ip + 2);
	uint16_t addr = read_reg(ptr);
	uint16_t value = read_reg(reg);
	write_mem(addr, value & 0x00ff);
}

void VirtualMachine::SHLRC() { // SHL reg
	uint8_t reg = read_mem(ip + 1);
	int shift = read_mem(ip + 2);
	uint16_t value = read_reg(reg);
	value <<= shift;
	write_reg(reg, value);
}