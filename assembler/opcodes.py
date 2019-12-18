class Opcode():
    def __init__(self, name, symbol, size):
        self.name = name
        self.symbol = symbol;
        self.size = size


OPCODES = {
    "mov_reg_const": Opcode("MOV reg, const", 0x09, 3),
    "mov_reg_reg": Opcode("MOV reg, reg", 0x10, 3),
    "mov_reg_mem": Opcode("MOV reg, mem", 0x11, 3),
    "mov_mem_reg": Opcode("MOV mem, reg", 0x12, 3),
    "mov_mem_mem": Opcode("MOV mem, mem", 0x13, 3),
    "shl_reg_const": Opcode("SHL reg, const", 0x8c, 3),
    "hlt": Opcode("HLT", 0x05, 1)
}