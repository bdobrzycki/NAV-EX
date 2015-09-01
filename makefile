#-Wall	"warn all" -- turns on all the warnings
#-c	compile and assemble, but do not link
#-g	produces debug symbols that become part of the executable file so that a debugger can figure out
#	where the functions, variables, etc are when you use the debugger.

CC = clang++
CXXFLAGS = -Wall -std=c++0x
GLFLAGS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

link: compile
	$(CC) main.o aeroplane.o xlocator.o varrow2d.o wv.o navleg.o alphanumdisplay.o simulation.o -o Navex $(CXXFLAGS) $(GLFLAGS)
	rm *.o

compile:
	$(CC) $(CXXFLAGS) -c main.cxx
	$(CC) $(CXXFLAGS) -c aeroplane.cxx
	$(CC) $(CXXFLAGS) -c xlocator.cxx
	$(CC) $(CXXFLAGS) -c varrow2d.cxx
	$(CC) $(CXXFLAGS) -c wv.cxx
	$(CC) $(CXXFLAGS) -c navleg.cxx  
	$(CC) $(CXXFLAGS) -c alphanumdisplay.cxx
	$(CC) $(CXXFLAGS) -c simulation.cxx