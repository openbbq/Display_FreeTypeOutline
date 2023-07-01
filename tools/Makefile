TARGET=../bin/gen

CC=gcc
CFLAGS=`pkg-config --cflags freetype2`
LIBS=`pkg-config --libs freetype2` 
OBJDIR=obj
BINDIR=../bin

.PHONY: default
default: $(TARGET)

.PHONY: all
all: clean default

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

$(OBJDIR)/%.o: %.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir $@

$(BINDIR)/gen: $(OBJDIR)/gen.o $(BINDIR)
	$(CC) $(OBJDIR)/gen.o $(LIBS) -lstdc++ -o $@
