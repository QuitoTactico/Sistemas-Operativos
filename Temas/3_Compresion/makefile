CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp RLE.cpp LZ77.cpp LZ78.cpp LZW.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp RLE.h LZ77.h LZ78.h LZW.h
	$(CXX) $(CXXFLAGS) -c main.cpp

RLE.o: RLE.cpp RLE.h
	$(CXX) $(CXXFLAGS) -c RLE.cpp

LZ77.o: LZ77.cpp LZ77.h
	$(CXX) $(CXXFLAGS) -c LZ77.cpp

LZ78.o: LZ78.cpp LZ78.h
	$(CXX) $(CXXFLAGS) -c LZ78.cpp

LZW.o: LZW.cpp LZW.h
	$(CXX) $(CXXFLAGS) -c LZW.cpp

clean:
	$(RM) $(TARGET) $(OBJS)
