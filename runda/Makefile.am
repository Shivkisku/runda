# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.7 2006/05/29 15:36:08 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies
SUBDIRS = src man doc tests
confdir = $(sysconfdir)
conf_DATA = rundarc
EXTRA_DIST = LGPL GPL BSD Makefile.train
EXTRA_DIR = swig example
bin_SCRIPTS = runda-config
ACLOCAL_AMFLAGS=-I m4

dist-hook:
	for subdir in $(EXTRA_DIR); do \
	  cp -rp $$subdir $(distdir); \
	  rm -f $(distdir)/$$subdir/*~; \
	  rm -f $(distdir)/$$subdir/*.{bak,orig}; \
	  rm -rf $(distdir)/$$subdir/CVS; \
	  rm -rf $(distdir)/$$subdir/*/CVS; \
	  rm -rf $(distdir)/$$subdir/.svn; \
	  rm -rf $(distdir)/.svn; \
	  rm -rf $(distdir)/*/.svn; \
	  rm -rf $(distdir)/*/*/.svn; \
	  rm -rf $(distdir)/$$subdir/*/CVS; \
	done

rpm: dist
	env PATH="/usr/bin:$$PATH" rpmbuild -ta @PACKAGE@-@VERSION@.tar.gz

win:
	make
	(cd src; wcmd /c make.bat)

win-dic:	
	(cd dic; ../src/runda-dict-index -f EUC-JP -t SHIFT-JIS)

win-dist:
	rm -fr wintmp @PACKAGE@-@VERSION@.exe
	cp -fr example wintmp
	perl -pe 's#\@prefix\@/lib/runda/dic/ipadic#\$$(rcpath)\\..\\dic\\ipadic#g;' \
	     < rundarc.in > wintmp/rundarc
	(cd wintmp; qkc -m -s -c *.c *.cpp)
	mkdir wintmp/dic
	cp -f dic/*.csv dic/*.def dic/dicrc wintmp/dic
	sed s/EUC-JP/SHIFT-JIS/ dic/dicrc > wintmp/dic/dicrc
	(cd wintmp/dic; qkc -m -s -c *)	
	wine ~/.root/bin/inno/ISCC.exe runda.iss
	rm -fr wintmp

script-dist:
	for subdir in perl ruby python java; do \
	  rm -fr runda-$${subdir}-@VERSION@; \
	  mkdir runda-$${subdir}-@VERSION@; \
	  cp -r COPYING BSD LGPL GPL AUTHORS runda-$${subdir}-@VERSION@; \
	  cp -r $${subdir}/* runda-$${subdir}-@VERSION@; \
	  cp -r doc/bindings.html  runda-$${subdir}-@VERSION@; \
	  find ./runda-$${subdir}-@VERSION@ -type d -name CVS | xargs rm -fr; \
	  find ./runda-$${subdir}-@VERSION@ -type d -name .svn | xargs rm -fr; \
	  rm -f runda-$${subdir}-@VERSION@/*~; \
	  tar zcfv runda-$${subdir}-@VERSION@.tar.gz runda-$${subdir}-@VERSION@; \
	  rm -fr runda-$${subdir}-@VERSION@; \
	done

script-clean:
	(cd perl; $(MAKE) clean; rm -f Makefile.old Makefile)
	(cd ruby; $(MAKE) clean; rm -f Makefile *.log)
	(cd python; python setup.py clean --all)
	(cd java; $(MAKE) clean;)

dist-all-package:
	(cd swig; $(MAKE))
	$(MAKE) script-clean
	(test -f Makefile) && $(MAKE) distclean
	./configure
	$(MAKE) dist
	$(MAKE) script-dist
	$(MAKE) clean
	$(MAKE) win
	$(MAKE) win-dic
	$(MAKE) win-dist

export-package:
	python googlecode_upload.py -p runda -s runda        -u taku@chasen.org -w `cat ~/.googlecode_upload_password` runda-@VERSION@.tar.gz
	python googlecode_upload.py -p runda -s runda-win    -u taku@chasen.org -w `cat ~/.googlecode_upload_password` runda-@VERSION@.exe
	python googlecode_upload.py -p runda -s runda-perl   -u taku@chasen.org -w `cat ~/.googlecode_upload_password` runda-perl-@VERSION@.tar.gz
	python googlecode_upload.py -p runda -s runda-ruby   -u taku@chasen.org -w `cat ~/.googlecode_upload_password` runda-ruby-@VERSION@.tar.gz
	python googlecode_upload.py -p runda -s runda-python -u taku@chasen.org -w `cat ~/.googlecode_upload_password` runda-python-@VERSION@.tar.gz
	python googlecode_upload.py -p runda -s runda-java   -u taku@chasen.org -w `cat ~/.googlecode_upload_password` runda-java-@VERSION@.tar.gz
	
