HANDINFILES=*.py Vector3D/*.py Makefile README
HANDINDIR=csc471final
PROGRAMNAME=plinko
SITEFILES=writeup/*
TARFILE=mgius_$(PROGRAMNAME).tar

all: $(PROGRAMNAME)

$(PROGRAMNAME):
	@echo "Python doesn't need to be compiled :D.  You can run the program with"
	@echo "python plinko.py"

mkpackage:
	tar -cf $(TARFILE) $(HANDINFILES)

handin: mkpackage
	handin zwood $(HANDINDIR) $(TARFILE)
	handin zwood $(HANDINDIR)

handindoc: 
	tar -cf mgius_$(PROGRAMNAME)_doc.tar $(SITEFILES)
	handin zwood csc471web mgius_$(PROGRAMNAME)_doc.tar
	handin zwood csc471web

test:
	python Vector3D/VectorTests.py

clean:
	rm -f $(TARFILE)
	find . -iname '*.pyc' -exec rm {} \;
