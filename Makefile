APP := rtx
INC_DIR := ./inc
CFLAGS := -g -Wall -std=c++11 -I$(INC_DIR)
SRC_FILES := src/Cam.cpp src/Shapes.cpp src/Triplet.cpp src/View.cpp src/World.cpp src/main.cpp
.PHONY: clean

all: clean app

app:
	$(CXX) -o $(APP) $(SRC_FILES) $(CFLAGS)

clean:
	$(RM) -f $(APP) *.o
