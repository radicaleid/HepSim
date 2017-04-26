# Makefile example for Promc+ROOT+fastjet
# S.Chekanov (ANL) 

ifndef LCIO_DIR 
$(error LCIO_DIR env variable is not set. Did you install iLCSoft/LCIO/?)
endif

ifndef ROOTSYS 
$(error ROOTSYS env variable is not set. Install ROOT first)
endif

include ${ROOTSYS}/etc/Makefile.arch

# Root variables
ROOTCFLAGS    = $(shell root-config --nonew --cflags)
ROOTLIBS      = $(shell root-config --nonew --libs)
ROOTGTTLIBS   = $(shell root-config --nonew --glibs)
# Assign or add variables
CXXFLAGS     += $(ROOTCFLAGS)
LIBS         += $(ROOTLIBS)
LIBS         += -L${LCIO_DIR}/lib -llcio -lsio -llcioF77

INCLUDE= -I./inc -I./ -I./src -I${LCIO_DIR}/include  

Tasks:     clean example

SOURCE_FILES := $(shell ls -1 example.cc)
SOURCE_FILES += $(shell ls -1 src/*.cc)


# build object files 
objects       = $(patsubst %.cc,%.o,$(SOURCE_FILES))


%.o: %.cc
	$(CXX) $(OPT) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

LIBOBJS = $(patsubst %.cc,%.o,$(SOURCE_FILES))

example: $(objects)
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
clean:
	        @rm -f *.o example *~ src/*.o;  echo "Clear.." 
