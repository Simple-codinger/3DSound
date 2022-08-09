
CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
LIBDIR := lib
TARGET := bin/3DSound
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++17 -O0 -Wall
LIB := -lrt -lasound -ljack -lpthread -lportaudio -lpython2.7 
INC := -I include -I/usr/include/python2.7 -DWITHOUT_NUMPY



$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR) $(BUILDDIR)/wav
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $< && echo "[OK] $@"

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)

.PHONY: clean

all: $(TARGET)