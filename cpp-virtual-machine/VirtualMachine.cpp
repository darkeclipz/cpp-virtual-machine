#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(int memory_size) {

	for (int i = 0; i < 256; i++) {
		lookup.push_back({ "NOP", &VirtualMachine::NOP, 1, "NOP" });
	}

	lookup[0x05] = { "HLT", &VirtualMachine::HLT, 1, "HLT" };
	lookup[0x09] = { "MRC", &VirtualMachine::MRC, 3, "MOV" };
	lookup[0x10] = { "MRR", &VirtualMachine::MRR, 3, "MOV" };
	lookup[0x11] = { "MRM", &VirtualMachine::MRM, 3, "MOV" };
	lookup[0x12] = { "MMR", &VirtualMachine::MMR, 3, "MOV" };
	lookup[0x13] = { "MMM", &VirtualMachine::MMM, 3, "MOV" };
	lookup[0x14] = { "MPP", &VirtualMachine::MPP, 3, "MOV" };
	lookup[0x21] = { "JMP", &VirtualMachine::JMP, 2, "JMP" };
	lookup[100] =  { "ADD", &VirtualMachine::ARC, 3, "ADD" };
	lookup[140] =  { "SHL", &VirtualMachine::SHL, 3, "SHL" };

	m_memory_size = memory_size;
	m_memory = new uint8_t[m_memory_size]{ 0x0 };

	reset();
}

VirtualMachine::~VirtualMachine() {
	delete[] m_memory;
	lookup.clear();
}

void VirtualMachine::step() {
	clock();
}

uint16_t VirtualMachine::read_reg(uint8_t reg) {
	switch (reg) {
	case VirtualMachine::REGISTERS::AX:	return ax;
	case VirtualMachine::REGISTERS::BX:	return bx;
	case VirtualMachine::REGISTERS::CX:	return cx;
	case VirtualMachine::REGISTERS::DX:	return dx;
	case VirtualMachine::REGISTERS::SP:	return sp;
	case VirtualMachine::REGISTERS::SB:	return sb;
	case VirtualMachine::REGISTERS::BP: return bp;
	case VirtualMachine::REGISTERS::IP:	return ip;
	default:
		throw std::invalid_argument("invalid register");
	}
}

void VirtualMachine::write_reg(uint8_t reg, uint16_t value) {
	switch (reg) {
	case VirtualMachine::REGISTERS::AX:	ax = value; break;
	case VirtualMachine::REGISTERS::BX:	bx = value; break;
	case VirtualMachine::REGISTERS::CX:	cx = value; break;
	case VirtualMachine::REGISTERS::DX:	dx = value; break;
	case VirtualMachine::REGISTERS::SP:	sp = value; break;
	case VirtualMachine::REGISTERS::BP:	bp = value; break;
	case VirtualMachine::REGISTERS::IP:	ip = value; break;
	default:
		throw std::invalid_argument("invalid register");
	}
}

uint16_t VirtualMachine::GetAX() {
	return ax;
}

uint16_t VirtualMachine::GetBX() {
	return bx;
}

uint16_t VirtualMachine::GetCX() {
	return cx;
}

uint16_t VirtualMachine::GetDX() {
	return dx;
}

uint16_t VirtualMachine::GetSP() {
	return sp;
}

uint16_t VirtualMachine::GetSB() {
	return sb;
}

uint16_t VirtualMachine::GetBP() {
	return bp;
}

uint16_t VirtualMachine::GetIP() {
	return ip;
}

uint8_t VirtualMachine::NOP() {
	return 0;
}

uint8_t VirtualMachine::MRR() {  // MOV reg, reg
	uint8_t r1 = read_mem(ip + 1);
	uint8_t r2 = read_mem(ip + 2);
	uint16_t value = read_reg(r2);
	write_reg(r1, value);
	return 0;
}

uint8_t VirtualMachine::MRM() {  // MOV reg, mem
	uint8_t reg = read_mem(ip + 1);
	uint8_t addr = read_mem(ip + 2);
	uint8_t value = read_mem(addr);
	write_reg(reg, value);
	return 0;
}

uint8_t VirtualMachine::MMR() {  // MOV mem, reg
	uint8_t addr = read_mem(ip + 1);
	uint8_t reg = read_mem(ip + 2);
	uint16_t value = read_reg(reg);
	write_mem(addr, value & 0x00ff);
	return 0;
}

uint8_t VirtualMachine::MMM() {  // MOV mem, mem
	uint8_t addr1 = read_mem(ip + 1);
	uint8_t addr2 = read_mem(ip + 2);
	uint8_t value = read_mem(addr2);
	write_mem(addr1, value);
	return 0;
}

uint8_t VirtualMachine::HLT() {
	SetFlag(B, true);
	return 0;
}

uint8_t VirtualMachine::MRC() {  // MOV reg, const
	uint8_t reg = read_mem(ip + 1);
	uint8_t val = read_mem(ip + 2);
	write_reg(reg, val);
	return 0;
}

uint8_t VirtualMachine::JMP() {
	uint8_t addr = read_mem(ip + 1);
	write_reg(REGISTERS::IP, addr - 2);
	return 0;
}

uint8_t VirtualMachine::ARC() {  // ADD reg, const
	uint8_t reg = read_mem(ip + 1);
	uint8_t val = read_mem(ip + 2);
	uint16_t current_value = read_reg(reg);
	write_reg(reg, current_value + val);
	return 0;
}

uint8_t VirtualMachine::MPP() {  // MOV *reg, *reg
	uint8_t reg1 = read_mem(ip + 1);
	uint8_t reg2 = read_mem(ip + 2);
	uint16_t addr = read_reg(reg1);
	uint8_t val = read_reg(reg2);
	write_mem(addr, val);
	return 0;
}

uint8_t VirtualMachine::SHL() { // SHL reg
	uint8_t reg = read_mem(ip + 1);
	int shift = read_mem(ip + 2);
	uint16_t value = read_reg(reg);
	value <<= shift;
	write_reg(reg, value);
	return 0;
}

uint8_t VirtualMachine::read_mem(uint16_t address) {
	if (address < 0 || address > m_memory_size) {
		throw std::invalid_argument("Reading outside of memory range.");
	}
	return m_memory[address];
}

void VirtualMachine::write_mem(uint16_t address, uint8_t value) {
	if (address < 0 || address > m_memory_size) {
		throw std::invalid_argument("Reading outside of memory range.");
	}
	m_memory[address] = value;
}

void VirtualMachine::clock() {
	opcode = m_memory[ip];
	(this->*lookup[opcode].operate)();
	ip += lookup[opcode].instruction_size;
	if (ip > m_memory_size) {
		ip = 0x00;
	}
}

void VirtualMachine::reset() {
	ax = 0; bx = 0; cx = 0; dx = 0; bp = 0;
	sb = m_memory_size - 1; sp = sb;  ip = 0; 
	m_flags = FLAGS::B;
}

uint8_t VirtualMachine::GetFlag(FLAGS flag) {
	return (m_flags & flag) > 0 ? 1 : 0;
}

void VirtualMachine::SetFlag(FLAGS flag, bool value) {
	if (value) {
		m_flags |= flag;
	}
	else {
		m_flags &= ~flag;
	}
}

void VirtualMachine::load_rom(uint8_t* rom, int size, uint8_t address) {
	for (int i = 0; i < size; i++) {
		write_mem(address + i, rom[i]);
	}
}