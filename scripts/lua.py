from collections import Mapping

from tabulator import Tabulator


class LuaFormatter:
    def __init__(self):
        self.tab = Tabulator()

    def format(self, elem):
        if type(elem) is list:
            return self.format_list(elem)
        elif type(elem) is str:
            return self.format_str(elem)
        elif isinstance(elem, Mapping):
            return self.format_table(elem)
        else:
            return str(elem)

    def format_list(self, arr):
        ret = "{\n"
        self.tab.indent()
        line = ",".join([self.format(v) for v in arr])
        ret += self.tab.line(line)
        self.tab.unindent()
        return ret + self.tab.string("}")

    def format_table(self, table):
        ret = "{\n"
        self.tab.indent()
        for k, v in table.items():
            line = "{} = {},".format(k, self.format(v))
            ret += self.tab.line(line)
        self.tab.unindent()
        return ret + self.tab.string("}")

    def format_str(self, s):
        return '"{}"'.format(s)
