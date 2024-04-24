
ROOT_LIB := $(shell root-config --libs) #-lSpectrum
ROOT_CXXFLAG := $(shell root-config --cflags)

INCLUDES += -I./RootCore/include/
CXXFLAGS = -g -shared -fPIC -I. $(INCLUDES) $(ROOT_CXXFLAG) 


# define our libraries 
LIBS := $(ROOT_LIB) -lRooFit -lRooFitCore -lFoam -lMinuit
LIBS += -L./RootCore/lib -lProcessCORARS 


#
OBJS = main.o 

all : main.exe

main.exe : ${OBJS}
	@echo -n "Building exe : $@ ... "
	@echo $(CXX) -g -L./ $(LIBS)   ${OBJS} -o $@
	@$(CXX) -g -L./ $(LIBS)   ${OBJS} -o $@ 
	@echo "Done"


lib%.so : %.o 
	@echo -n "Building $@ ... "
	@echo $(CXX)  $(LIBS)  $< -o $@
	@$(CXX) $(CXXFLAGS)  $< -o $@
	@echo "Done"


%.o : %.cxx   
	@echo Compiling object file $@
	@echo $(CXX) $(CXXFLAGS) -c $< -o $@
	@$(CXX) -g $(CXXFLAGS) -c $< -o $@
	@echo "Done"


%.o : %.cpp
	@echo I compile source $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

rmold:
	rm -f *~

# clean 
clean: rmold
	rm -f *.o 
	rm -f *.exe 
	rm -f *.so 
	rm -f *.a 
	rm -f */*.o 
	rm -f */*.d
	rm -f */*.exe
	rm -f */*.so 
	rm -f */*.a 

