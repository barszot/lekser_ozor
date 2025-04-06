# Kompilator i flagi
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Pliki źródłowe i nagłówki
HEADERS = lexer.h lexer_errors.h token.h token_printer.h
OBJECTS = main.o lexer.o

# Domyślny cel
all: main

# Kompilacja pliku wykonywalnego
main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS)

# Kompilacja main.cpp
main.o: main.cpp lexer.h token_printer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Kompilacja lexer.cpp
lexer.o: lexer.cpp lexer.h lexer_errors.h token.h
	$(CXX) $(CXXFLAGS) -c lexer.cpp

# Czyszczenie plików obiektowych i binarki
clean:
	rm -f *.o main
