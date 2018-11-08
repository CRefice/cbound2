class Tabulator:
    def __init__(self):
        self.indent = 0

    def push_indent(self):
        self.indent = self.indent+1

    def pop_indent(self):
        self.indent = self.indent-1

    def line(self, text):
        return '\t' * self.indent + text + '\n'
