OBJECTS = AttributeTranslator.o MatchMaker.o MemberDatabase.o PersonProfile.o utility.o
HEADERS = $(wildcard *.h)
OPTIONS = -std=c++17 -g -fsanitize=undefined

.PHONY: clean

%.o: %.cpp $(HEADERS)
	g++ $(OPTIONS) -c $< -o $@

main: main.o $(OBJECTS) 
	g++ $(OPTIONS) main.o $(OBJECTS) -o $@

test: test.o $(OBJECTS)
	g++ $(OPTIONS) test.o $(OBJECTS) -o $@

clean:
	rm -f *.o main test
