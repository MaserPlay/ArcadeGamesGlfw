import shutil
import sys
import os

#sys.argv[1] - from dir    sys.argv[2] - to dir
if (len(sys.argv) >= 3 and os.path.isdir(sys.argv[1]) and os.path.isdir(sys.argv[2])):
    shutil.make_archive(sys.argv[2] + "/" + sys.argv[1].split("/")[-1], 'zip', sys.argv[1])
else:
    print("sys.argv[1] is not None and sys.argv[2] is not None and os.path.isdir(sys.argv[1]) and os.path.isdir(sys.argv[2]) failed")