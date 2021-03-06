
XML = calibration_funcref.xml command_funcref.xml dio_funcref.xml \
	deprecated_funcref.xml error_funcref.xml extensions_funcref.xml \
	drivers.xml funcref.xml glossary.xml \
	install.xml intro.xml other.xml reference.xml tutorial.xml \
	driverwriting.xml comedilib.xml bindings.xml comedilib.ent

TUTORIALS = $(srcdir)/../demo/tut1.c $(srcdir)/../demo/tut2.c \
	    $(srcdir)/../demo/tut3.c

EXTRA_DIST = $(XML) calibration_funcref.txt command_funcref.txt dio_funcref.txt \
	deprecated_funcref.txt error_funcref.txt extensions_funcref.txt \
	funcref mkref drivers.txt mkdr FAQ \
	acq-seq.gif doc_html man comedilib_html_config.xsl \
	comedilib_dblatex_config.xsl \
	comedilib.css

BUILT_SOURCES = calibration_funcref.xml command_funcref.xml dio_funcref.xml \
	deprecated_funcref.xml error_funcref.xml extensions_funcref.xml \
	funcref.xml drivers.xml bindings.xml

if HAVE_XMLTO
all_html = $(srcdir)/doc_html
install_html = install_html
uninstall_html = uninstall_html
all_man = $(srcdir)/man
install_man = install_man
uninstall_man = uninstall_man
else
all_html =
install_html =
uninstall_html =
all_man =
install_man =
uninstall_man =
endif

if BUILD_PDF
dist_pdf_DATA = $(srcdir)/pdf/comedilib.pdf
else
dist_pdf_DATA =
endif

if BUILD_PDF_USING_DBLATEX
XMLTO_PDF_OPTS = --with-dblatex -p "-p $(abs_srcdir)/comedilib_dblatex_config.xsl"
else
if BUILD_PDF_USING_FOP
XMLTO_PDF_OPTS = --with-fop
else
XMLTO_PDF_OPTS =
endif
endif

all-local: $(all_html) $(all_man)

install-data-local: $(install_html) $(install_man)

uninstall-local: $(uninstall_html) $(uninstall_man)

#named this doc_html to avoid phony html target that is automatically generated
#(at least by automake1.8)
$(srcdir)/doc_html: $(XML) $(TUTORIALS) comedilib_html_config.xsl
	{ $(XMLTO) -o $(srcdir)/doc_html -m $(srcdir)/comedilib_html_config.xsl --skip-validation html $(srcdir)/comedilib.xml && \
		(cd $(srcdir)/doc_html && $(LN_S) -f ../comedilib.css . && $(LN_S) -f ../acq-seq.gif .) && \
		touch $(srcdir)/doc_html;} || \
		{ $(RM) -r $(srcdir)/doc_html; exit 1; }

install_html:
	$(mkdir_p) $(DESTDIR)$(htmldir)/html
	chmod u+w $(DESTDIR)$(htmldir)/html
	for each in $(srcdir)/doc_html/*.html $(srcdir)/*.gif $(srcdir)/*.css; do \
		$(INSTALL_DATA) $$each $(DESTDIR)$(htmldir)/html ; done

uninstall_html:
	for each in $(srcdir)/doc_html/*.html $(srcdir)/*.gif $(srcdir)/*.css; do \
		$(RM) $(DESTDIR)$(htmldir)/html/`basename $$each` ; done

$(srcdir)/man: $(XML)
	{ $(XMLTO) -o $(srcdir)/man --skip-validation man $(srcdir)/comedilib.xml && touch $(srcdir)/man; } || { $(RM) -r $(srcdir)/man; exit 1; }

install_man:
	$(mkdir_p) -m 755 $(DESTDIR)$(mandir)/man3
	chmod u+w $(DESTDIR)$(mandir)/man3
	for each in `find $(srcdir)/man/ -name '*.3'`; do $(INSTALL_DATA) $$each $(DESTDIR)$(mandir)/man3 ; done

uninstall_man:
	for each in `find $(srcdir)/man/ -name '*.3'`; do $(RM) $(DESTDIR)$(mandir)/man3/`basename $$each` ; done

$(srcdir)/pdf/comedilib.pdf: $(XML) $(TUTORIALS) comedilib_dblatex_config.xsl
	$(XMLTO) -o $(srcdir)/pdf --skip-validation $(XMLTO_PDF_OPTS) pdf $(srcdir)/comedilib.xml

funcref.xml: funcref mkref
	$(srcdir)/mkref $(srcdir)/funcref >$(srcdir)/funcref.xml

calibration_funcref.xml: calibration_funcref.txt mkref
	$(srcdir)/mkref $(srcdir)/calibration_funcref.txt >$(srcdir)/calibration_funcref.xml

command_funcref.xml: command_funcref.txt mkref
	$(srcdir)/mkref $(srcdir)/command_funcref.txt >$(srcdir)/command_funcref.xml

dio_funcref.xml: dio_funcref.txt mkref
	$(srcdir)/mkref $(srcdir)/dio_funcref.txt >$(srcdir)/dio_funcref.xml

deprecated_funcref.xml: deprecated_funcref.txt mkref
	$(srcdir)/mkref $(srcdir)/deprecated_funcref.txt >$(srcdir)/deprecated_funcref.xml

error_funcref.xml: error_funcref.txt mkref
	$(srcdir)/mkref $(srcdir)/error_funcref.txt >$(srcdir)/error_funcref.xml

extensions_funcref.xml: extensions_funcref.txt mkref
	$(srcdir)/mkref $(srcdir)/extensions_funcref.txt >$(srcdir)/extensions_funcref.xml

drivers.xml: drivers.txt mkdr
	$(srcdir)/mkdr $(srcdir)/drivers.txt >$(srcdir)/drivers.xml

maintainer-clean-local:
	$(RM) -r $(srcdir)/doc_html $(srcdir)/man $(srcdir)/pdf

locales = de

messages: .phony
	xgettext -k_ -k_s `find .. -name '*.c'`
	for i in $(locales);do \
	  msgmerge po/$$i.po messages.po -o $$i.po; \
	  mkdir -p locale/$$i/LC_MESSAGES; \
	  msgfmt $$i.po -o locale/$$i/LC_MESSAGES/comedilib.mo; \
	done
