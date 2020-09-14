CC=g++

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %cpp,%o,$(SRCS))

CXXFLAGS= -w -g -O2 -std=c++11 -fPIC -I. -I../config -I../mysql  -I/usr/local/include/Poco
LINKFLAGS= -fPIC -shared 
#CXXFLAGS= -w -g -O2 -std=c++11
LDFLAGS=-L/usr/local/lib  -L../lib
LIBS=-lPocoUtil -lPocoXML -lPocoNet -lPocoFoundation -lPocoData -lPocoJSON -lPocoMongoDB -lPocoDataMySQL -lcrypto -lssl -lpthread -lrt -ldl -lmysql

LIBNAME=$(notdir $(CURDIR))
TARGET=../lib/lib$(LIBNAME).so

.PHONY:all clean

all:$(TARGET)
	
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS) $(LINKFLAGS)

%o:%cpp
	$(CC) -o $^  $(CXXFLAGS) 

clean:
	rm -fr $(OBJS) $(TARGET)
	
