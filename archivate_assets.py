import shutil
import sys
import os

slash = "/"
#sys.argv[1] - from dir    sys.argv[2] - to dir
if (len(sys.argv) >= 3):
    for i in os.listdir(sys.argv[1]):
        if (os.path.isdir(sys.argv[1] + slash + i)):
            if not os.path.exists(sys.argv[2] + slash + i):
                os.makedirs(sys.argv[2] + slash + i)
            print("creating " + sys.argv[2] + slash + i + slash + i + "_resources.zip" + "...")
            shutil.make_archive(sys.argv[2] + slash + i + slash + i + "_resources", 'zip', sys.argv[1] + slash + i)
else:
    print("len(sys.argv) < 3")