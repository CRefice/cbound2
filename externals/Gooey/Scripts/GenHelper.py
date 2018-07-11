# Python script for creating a helper include file
# that contains all other include files.
# File is located at Include\Gooey.hpp

import os

dirpath = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..', 'Include'))

with open(os.path.join(dirpath, 'Gooey.hpp'), 'w', encoding='utf-8') as write_file:
    write_file.write(("//This is a helper include file that includes most (all) controls for you.\n\n"))

    for root, subdirs, files in os.walk(dirpath):
        for file in files:
            if file.endswith('.hpp') and file != 'Gooey.hpp':
                filename = os.path.join(root, file)
                path = os.path.relpath(filename, dirpath)
                write_file.write('#include "{}"\n'.format(path))
