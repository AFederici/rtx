APP := rtx
TEST := runTest
INC_DIR := ./inc
CFLAGS := -g -Wall -std=c++11 -I$(INC_DIR)
SRC_FILES := src/Cam.cpp src/Shapes.cpp src/Triplet.cpp src/View.cpp src/World.cpp
.PHONY: clean

all: clean app
t: clean runTest
runTest:
	$(CXX) -o $(TEST) $(SRC_FILES) test/mp1Test.cpp -pthread -lgtest -lgtest_main $(CFLAGS)

app:
	$(CXX) -o $(APP) $(SRC_FILES) src/main.cpp $(CFLAGS)

clean:
	$(RM) -f $(APP) $(TEST) *.o
