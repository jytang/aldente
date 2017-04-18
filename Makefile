MAIN_TARGET     := aldente

SRC_DIR         := src
INC_DIR         := src
BUILD_DIR       := .build

CC              := clang++
CFLAGS          := -std=c++1y -O0 -g
INCS            := -I$(INC_DIR)
LDFLAGS         :=
LIBS            := -lGLEW -lglfw -lSOIL
HEADERS         := $(shell find $(INC_DIR) -name '*.h' -type 'f' | sort)
MAIN_SOURCES    := $(shell find $(SRC_DIR) -name '*.cpp' -type 'f' | sort)
MAIN_OBJECTS    := $(MAIN_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ifeq ($(shell uname),Darwin)
	LIBS += -framework OpenGL -framework GLUT
else
	LIBS += -lGL -lGLU
endif

all : $(MAIN_TARGET)

$(MAIN_TARGET): $(MAIN_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(MAIN_TARGET)
