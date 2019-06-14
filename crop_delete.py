import os
import sys

def delete_crop(filename):
    if os.path.isfile(filename):
            os.remove(filename)

filename = sys.argv[1]

delete_crop(filename)
