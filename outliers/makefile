INC_DIRS      = -I.
LIB_DIRS      = -L.
LIBS          = 
#CC_FLAGS      = -Wall -g
CC_FLAGS      = -Wall
CPP           = g++

objects = methods.o outlier-detection.o demo.o

Outlier: ${objects}
	${CPP} ${CC_FLAGS} -o $@ ${objects} $(INC_DIRS) $(LIB_DIRS) $(LIBS)

methods.o: methods.cpp
	${CPP} ${CC_FLAGS} -c -o $@ $< $(INC_DIRS) $(LIB_DIRS) $(LIBS)

outlier-detection.o: outlier-detection.cpp
	${CPP} ${CC_FLAGS} -c -o $@ $< $(INC_DIRS) $(LIB_DIRS) $(LIBS)

demo.o: demo.cpp
	${CPP} ${CC_FLAGS} -c -o $@ $< $(INC_DIRS) $(LIB_DIRS) $(LIBS)

clean:
	rm -rf Outlier *.o
