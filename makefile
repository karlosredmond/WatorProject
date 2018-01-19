#Karl Redmond 03/10/2017
#Makefile
#Used to contain commands to compile project
#added -I. to look for any header files with the same .cpp name

EXE = watorMainSFML
CC = g++
CFLAGS = -std=c++11 -pthread -I.
CFILES = WatorProjectSFML.cpp

#This rule says that each .o file depends on a .cpp file of the same name
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

ALL: $(CFILES)
	$(CC) $(CFILES) $(CFLAGS) -o $(EXE) $(SFML) $(DEBUGFLAGS)

SFML: SFML = -lsfml-graphics -lsfml-window -lsfml-system
SFML: ALL

DEBUG: DEBUGFLAGS = -g -O0
DEBUG: ALL

doc: ALL
	if [ -e "Doxyfile" ] ; then  rm 'Doxyfile';  fi
	cp ~/Doxyfile .
	if [ -d "docs" ] ; then  rm -rf 'docs';  fi
	mkdir docs
	echo "PROJECT_NAME = \"$L\"" >> Doxyfile
	echo "PROJECT_BRIEF = \"$B\"" >> Doxyfile
	echo "PROJECT_NUMBER = 0.8" >> Doxyfile
	echo "OUTPUT_DIRECTORY = ./docs"  >> Doxyfile
	echo "EXTRACT_ALL = YES" >> Doxyfile
	echo "RECURSIVE = YES" >> Doxyfile
	echo "SOURCE_BROWSER = YES" >> Doxyfile
	echo "GENERATE_LATEX = NO" >> Doxyfile
	echo "DOT_IMAGE_FORMAT = svg" >> Doxyfile
	echo "INPUT = ." >> Doxyfile
	doxygen Doxyfile

#this is customary and just deletes all .o files
CLEAN:
	rm *.o
