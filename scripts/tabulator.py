class Tabulator:
    def __init__(self):
        self.tabs = 0

    def indent(self):
        self.tabs += 1

    def unindent(self):
        self.tabs -= 1

    def string(self, text):
        return "\t" * self.tabs + text

    def line(self, text):
        return self.string(text) + "\n"
