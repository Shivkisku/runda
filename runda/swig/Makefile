SWIG = swig
PREFIX = RuNda

all: perl ruby python java csharp

clean:
	rm -f      *.pm *.cxx *.py
	
perl:	
	$(SWIG) -perl -shadow -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../perl
	mv -f $(PREFIX).pm ../perl

ruby:	
	$(SWIG) -ruby -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../ruby/$(PREFIX)_wrap.cpp
	
python:	
	$(SWIG) -python -shadow -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../python
	mv -f $(PREFIX).py ../python
	
csharp:	
	$(SWIG) -csharp -namespace RuNda -c++ $(PREFIX).i
	mv -f $(PREFIX)_wrap.cxx ../csharp
	mv -f *.cs ../csharp

java:
	$(SWIG) -java -package org.chasen.runda -c++ $(PREFIX).i
	mkdir -p ../java/org/chasen/runda
	mv -f $(PREFIX)_wrap.cxx ../java
#	rm -fr exception.java SWIGTYPE_*.java
	mv -f *.java ../java/org/chasen/runda
	
	

