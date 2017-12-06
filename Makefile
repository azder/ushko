TARGETS = primer
LLDLIBS = -lglut32 -lopengl32 -lglu32
default: $(TARGETS)
all: default
.c.o:
	gcc -c $<
$(TARGETS): $$@.o
	gcc $@.o -L . $(LLDLIBS) -o $@
clean:  
	-rm -f *.o $(TARGETS)
