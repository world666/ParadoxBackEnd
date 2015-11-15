CC=g++
CFLAGS=-c -Wall -Iinclude/ -D _LOG
LDFLAGS=
LIBS= -lpthread  -llog4cplus
SRCDIR=src/
SOURCES=main.cpp log.cpp TcpClient.cpp TcpServer.cpp
OBJDIR=obj/
OBJECTS=$(addprefix $(OBJDIR),$(SOURCES:.cpp=.o))

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


