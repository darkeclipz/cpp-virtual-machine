class Token():
    def __self__(self, value):
        self.value = value


class LabelDefinitionToken(Token):
    def __self__(self, value):
        super().__init__(self, value)


class LabelReferenceToken(Token):
    def __self__(self, value):
        super().__init__(self, value)


class OperatorToken(Token):
    def __self__(self, value):
        super().__init__(self, value)


class OperandToken(Token):
    def __self__(self, value):
        super().__init__(self, value)


class MemoryOperandToken(OperandToken):
    def __self__(self, value):
        super().__init__(self, value)


class RegisterOperandToken(OperandToken):
    def __self__(self, value):
        super().__init__(self, value)


class ConstantOperandToken(OperandToken):
    def __self__(self, value):
        super().__init__(self, value)


class RegisterPointerOperandToken(OperandToken):
    def __self__(self, value):
        super().__init__(self, value)