# Вызывается автоматически из commit.bat для инкремента версии сборки перед коммитом в репозиторий

import os
from datetime import datetime

def CurrentTime():
    t = datetime.now()
    year = str(t.year)
    return \
        str(t.year) + "-" + \
        '{:-02}'.format(t.month) + "-" + \
        '{:-02}'.format(t.day) + " " + \
        '{:-02}'.format(t.hour) + ":" + \
        '{:-02}'.format(t.minute) + ":" + \
        '{:-02}'.format(t.second)

def ProcessLine(line):
    if line.startswith("#define VERSION_BUILD"):
        strings = line.split(" ")
        line = strings[0] + " " + strings[1] + " " + str(int(strings[2]) + 1) + "\n"
    if line.startswith("#define DATE_BUILD"):
        strings = line.split(" ")
        line = strings[0] + " " + strings[1] + " " + "\"" + CurrentTime() + "\"\n"
    return line

def ProcessFile(name_file):
    lines = []
    with open(name_file, "r", encoding="cp1251") as file:
        for line in file:
            lines.append(line)
        file.close()
        with open(name_file, "w", encoding="cp1251") as file:
            for line in lines:
                line = ProcessLine(line)
                file.write(line)
            file.close()

ProcessFile("sources/MultiSensors/src/defines.cpp")
