import os
import sys
from datetime import datetime

if len(sys.argv) < 2:
    quit()

command = ""

for str in sys.argv:
    if str != sys.argv[0]:
        command += str
        command += " "

startTime = datetime.now();

os.system(command)

print("\n Execution time command \" ", command, "\" : ", datetime.now() - startTime, "\n")
