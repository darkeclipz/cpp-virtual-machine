class Tokenizer:

    def __init__(self, script):
        self.script = script.strip()
        self.index = 0
        self.tokens = []
        self.tokenize()

    def tokenize(self):
        stream = self.char_stream()
        for c in stream:
            c = self.parse_whitespace(c, stream)
            c = self.parse_comment(c, stream)
            c, word = self.parse_word(c, stream)
            if len(word) > 0:
                self.tokens.append(word)

    def parse_whitespace(self, c, stream):
        while c in [' ', '\r', '\n', '\t']:
            c = next(stream)
        return c

    def parse_comment(self, c, stream):
        if c == ';':
            while c != '\n':
                c = next(stream)
        return c

    def parse_word(self, c, stream):
        word = ''
        while c not in [' ', ',', '\r', '\n', '\t'] and self.index < len(self.script):
            word += c
            c = next(stream)
        word += c.strip()
        return c, word.lower()

    def char_stream(self):
        while self.index < len(self.script):
            char = self.script[self.index]
            self.index += 1
            yield char
