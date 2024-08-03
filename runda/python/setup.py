#!/usr/bin/env python

from distutils.core import setup,Extension,os
import string

def cmd1(str):
    return os.popen(str).readlines()[0][:-1]

def cmd2(str):
    return string.split (cmd1(str))

setup(name = "runda-python",
	version = cmd1("runda-config --version"),
	py_modules=["RuNda"],
	ext_modules = [
		Extension("_RuNda",
			["RuNda_wrap.cxx",],
			include_dirs=cmd2("runda-config --inc-dir"),
			library_dirs=cmd2("runda-config --libs-only-L"),
			libraries=cmd2("runda-config --libs-only-l"))
			])
