# Process this file with automake to produce Makefile.in

html_HTMLS = runda.html
EXTRA_DIST = $(html_HTMLS) bindings.html dic.html format.html \
             learn.html runda.html unk.html partial.html \
	     dic-detail.html  feature.html index.html \
	     librunda.html  soft.html posid.html runda.css \
	     feature.png  flow.png  result.png en doxygen

update-html:
	@for html in $(html_HTMLS); do \
		cmd=`basename $$html .html`.1 && \
		man2html ../man/$$cmd > $$html; \
	done
