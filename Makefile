MAIN            := aldente
TEST            := test

SRC_DIR         := src
MAIN_DIR        := $(SRC_DIR)/mains
BIN_DIR         := bin
BUILD_DIR       := .build

CC              := clang++
CFLAGS          := -std=c++1y -O0 -g
INCS            := -I$(SRC_DIR)
LIBS            := -lGLEW -lglfw -lSOIL

HEADERS         := $(shell find $(SRC_DIR) \( -path $(MAIN_DIR) -prune -o -name '*.h' \) -type 'f' | sort)
SOURCES         := $(shell find $(SRC_DIR) \( -path $(MAIN_DIR) -prune -o -name '*.cpp' \) -type 'f' ! -name '*_test.cpp' | sort)
OBJECTS         := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_SOURCES    := $(shell find $(SRC_DIR) \( -path $(MAIN_DIR) -prune -o -name '*.cpp' \) -type 'f' | sort)
TEST_OBJECTS    := $(TEST_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ifeq ($(shell uname),Darwin)
	LIBS += -framework OpenGL -framework GLUT
else
	LIBS += -lGL -lGLU
endif

all: $(MAIN) $(TEST)

$(TEST): $(MAIN_DIR)/$(TEST).cpp $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(INCS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(MAIN): $(MAIN_DIR)/$(MAIN).cpp $(OBJECTS)
	$(CC) $(CFLAGS) $(INCS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(MAIN) $(TEST)
