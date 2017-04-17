SRC_DIR         := src
MAIN_DIR        := $(SRC_DIR)/mains
BIN_DIR         := bin
BUILD_DIR       := .build

CC              := clang++
CFLAGS          := -std=c++1y -O0 -g
INCS            := -I$(SRC_DIR)
LIBS            := -lGLEW -lglfw -lSOIL

define find_in_src
	$(shell \
		find $(SRC_DIR) \( -path $(MAIN_DIR) -prune -o -name '$(1)' \) -type 'f' \
		| sort)
endef

SOURCES         := $(call find_in_src,'*.cpp')
HEADERS         := $(call find_in_src,'*.h')
OBJECTS         := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ifeq ($(shell uname),Darwin)
	LIBS += -framework OpenGL -framework GLUT
else
	LIBS += -lGL -lGLU
endif

all: $(BIN_DIR)/aldente $(BIN_DIR)/test

$(BIN_DIR)/%: $(MAIN_DIR)/%.cpp $(OBJECTS)
	@echo $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(MAIN_TARGET) $(TEST_TARGET)
