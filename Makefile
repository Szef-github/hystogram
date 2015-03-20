QTINC		:= /usr/include/qt4
QTLIB		:= /usr/lib/qt4

CC			:= g++
TARGETS 	:= hystogram
SOURCES 	:= main.cpp input_image.cpp hist_image.cpp params.cpp
CFLAGS		:= -Wall -ggdb -O0 -std=c++11 -I$(QTINC)
#CPPFLAGS	:= -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64
LDFLAGS		:= -L$(QTLIB)
LDLIBS		:= -lQtCore -lQtGui
OBJS		:= $(SOURCES:.cpp=.o)

all: $(TARGETS)
clean:
	rm -rf $(TARGETS) $(OBJS)
hystogram: $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS) $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

vpath %.cpp src
