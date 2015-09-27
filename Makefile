CC=g++
CFLAGS=-c -Wall
LDFLAGS=
LIBS= -llog4cplus
SRCDIR=src/
SOURCES=main.cpp
OBJDIR=obj/
OBJECTS=$(OBJDIR)$(SOURCES:.cpp=.o)

EXECDIR=bin/
EXECUTABLE=Paradox

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(LIBS)  $(OBJECTS) -o $(EXECDIR)$@

$(OBJECTS): | obj


obj:
	@mkdir -p $@
	@mkdir -p $(EXECDIR)

$(OBJDIR)%.o : $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -rf $(OBJDIR)
	$(RM) -rf $(EXECDIR)	


