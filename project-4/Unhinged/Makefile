OBJECTS = AttributeTranslator.o MatchMaker.o MemberDatabase.o PersonProfile.o
HEADERS = $(wildcard *.h)

.PHONY: main test clean

%.o: %.cpp $(HEADERS)
	g++ -c $< -o $@

main: main.o $(OBJECTS) 
	g++ main.o $(OBJECTS) -o $@

test: test.o $(OBJECTS)
	g++ test.o $(OBJECTS) -o $@

clean:
	rm -f *.o main test
