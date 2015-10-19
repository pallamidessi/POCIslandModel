CXXFLAGS += -fopenmp -O2 -g -Wall 
OBJS_SERVER = PoCServer.o
OBJS_CLIENT = PoCClient.o
LDFLAGS += -lpthread -fopenmp -lboost_system -lboost_serialization -lboost_iostreams 

TARGET_SERVER = PoCServer
TARGET_CLIENT = PoCClient

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER):	$(OBJS_SERVER)
	$(CXX) -o $(TARGET_SERVER) $(OBJS_SERVER) $(LDFLAGS) -g 
	
$(TARGET_CLIENT):	$(OBJS_CLIENT)
	$(CXX) -o $(TARGET_CLIENT) $(OBJS_CLIENT) $(LDFLAGS) -g 

%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $^

clean:
	rm *.o $(TARGET_SERVER) $(TARGET_CLIENT)
