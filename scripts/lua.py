from collections import Mapping


def indent(text, tabs):
    return "\t" * tabs + text


def format(elem, indentation=0):
    if type(elem) is str:
        return _format_str(elem)
    elif type(elem) is list:
        return _format_list(elem, indentation)
    elif isinstance(elem, Mapping):
        return _format_table(elem, indentation)
    else:
        return str(elem)


def _format_list(arr, tabs):
    line = ", ".join([format(v, tabs + 1) for v in arr])
    return "{\n" + indent(line, tabs + 1) + "\n" + indent("}", tabs)


def _format_table(table, tabs):
    line = ",\n".join(
        [
            indent("{} = {}".format(k, format(v, tabs + 1)), tabs + 1)
            for k, v in table.items()
        ]
    )
    return "{\n" + line + "\n" + indent("}", tabs)


def _format_str(s):
    return '"{}"'.format(s)
