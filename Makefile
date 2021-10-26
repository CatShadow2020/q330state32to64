PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = q330state32to64.o binq.o bint.o util.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g -std=c++11
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2 -std=c++11 
else ifeq ($(BUILD_MODE),linuxtools)
	CFLAGS += -g -pg -fprofile-arcs -ftest-coverage -std=c++11
	LDFLAGS += -pg -fprofile-arcs -ftest-coverage
else
    $(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	q330state32to64

q330state32to64:	$(OBJS)
	$(CXX) $(LDFLAGS) -lstdc++ -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr q330state32to64 $(OBJS)
