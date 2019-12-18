from tokenizer import Tokenizer

SCRIPT = """

    MOV ax, 2
    MOV bx, 8
    MUL ax, bx
    
    
    MOV [ax],[bx]
    SHL ax, 1         ; do some stuff
    HLT
    
"""

tokenizer = Tokenizer(SCRIPT)
tokens = tokenizer.tokens

print(tokens)
