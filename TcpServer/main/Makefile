CC=g++

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %cpp,%o,$(SRCS))

CXXFLAGS= -w -g -O2 -std=c++11 -I. -I../tcpserver -I../mysql -I/usr/local/include/Poco
#CXXFLAGS= -w -g -O2 -std=c++11
LDFLAGS=-L/usr/local/lib -L../lib 
LIBS=-lPocoUtil -lPocoXML -lPocoNet -lPocoFoundation -lPocoData -lPocoJSON -lPocoMongoDB -lPocoDataMySQL -lcrypto -lssl -lpthread -lrt -ldl -lmysql -ltcpserver

TARGET=a.out

.PHONY:all clean

all:$(TARGET)
	
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS) 

%o:%cpp
	$(CC) -o $^  $(CXXFLAGS) 

clean:
	rm -fr $(OBJS) $(TARGET)
	
