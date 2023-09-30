# INCLUDE = ../include
# LIBS = ../lib
# EXT_LIBS = -llhp -lgmp -lpbc -lssl -lcrypto

EXT_LIBS = -lgmp -lpbc -lssl -lcrypto -lmcl -lmsgpackc


# MCL_LIB_DIR=~/Documents/Gitrepos/Coderepos/mcl/lib
# MCL_LIB=$(MCL_LIB_DIR)/libmcl.a
# BN256_SLIB_LDFLAGS+=-lmcl -L./lib

TARGET   = testlibs
# TARGET   = khprf

DEBUG=n
CC       = g++

# CFLAGS   = -std=gnu++11 -I ${INCLUDE} -L ${LIBS} ${EXT_LIBS}

CFLAGS   = -std=gnu++11

ifeq ($(DEBUG),y)
	CFLAGS += -g -pg
else
	CFLAGS += -O3
endif

LINKER   = g++ 
# LFLAGS   = -llhp -lgmp -lpbc -lssl -lcrypto

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
 
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm        = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@ $(EXT_LIBS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(LINKER) -c $< -o $@

.PHONY: clean
clean:
	$(rm) $(OBJECTS) 
	$(rm) $(BINDIR)/$(TARGET)

.PHONY: remove
remove: clean
	$(rm) $(BINDIR)/$(TARGET)