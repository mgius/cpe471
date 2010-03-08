HANDINFILES=*.py Vector3D/*.py Makefile README
HANDINDIR=csc471p4
PROGRAMNAME=plinko
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

test:
	python Vector3D/VectorTests.py

clean:
	rm -f $(TARFILE)
	find . -iname '*.pyc' -exec rm {} \;
