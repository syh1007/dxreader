LIBA = libdxfreader.a
TEST = test
LIBOBJS = dl_dxf.o dl_writer_ascii.o DxfReader.o DRObject.o
TESTOBJS = Test.o $(LIBOBJS)

INCLUDE = -I../include

AR = ar
CC = g++
RM = rm -f
ARFLAGS = crs
CXXFLAGS = -DLINUX -g -O2 -Wall -fPIC
LDFLAGS = -lpthread

.SUFFIXES: .cpp .h .o
.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(LIBA): $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

$(TEST): $(TESTOBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

all: $(LIBA) $(TEST)

clean:
	$(RM) $(LIBA) $(TEST) $(TESTOBJS)

.PHONY: clean all

