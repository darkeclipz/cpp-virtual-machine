from constants import *
from opcodes import *
from tokens import *


SCRIPT = """
loop:
    MOV eax, 2
    MOV ebx, 8
    MUL eax, ebx
    SHL eax, 1         ; do some stuff
    HLT
    
; shouldn't come here
    JMP loop
"""


