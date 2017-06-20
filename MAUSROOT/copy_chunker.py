import os

TARGETDIR = "/homes/schiel/MAUS/TOOL/Chunker"
THISDIR = os.path.abspath(os.path.dirname(__file__))
CHUNKERDIR = os.path.join(THISDIR, "Chunker")
ALLFILES = os.path.join(CHUNKERDIR, "*")

cmd = "rm -r " + os.path.join(TARGETDIR, "*")
os.system(cmd)

cmd = "cp -r " + ALLFILES + " " + TARGETDIR
os.system(cmd)
