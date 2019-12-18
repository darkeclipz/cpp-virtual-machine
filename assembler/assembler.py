from tokenizer import Tokenizer

SCRIPT = """

    MOV ax, 10
    MOV ax, bx
    MOV ax, [10]
    
    MOV [10], ax
    MOV [10], [10]
    
    MOV [10], [ax]
    
    JMP $+3

    ADD ax, 10

    SHL ax, 8

    HLT
    
"""

tokenizer = Tokenizer(SCRIPT)
tokens = tokenizer.tokens

print(tokens)
