CC=gcc
INCLUDE = -Isource/include
DEFINES =
FLAGS = -g -Wall
BUILD_DIR = build
OBJ_DIR = obj
TARGET = app
EXTENCION =
SRC = $(wildcard source/*.c)

OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

ifeq ($(OS), Windows_NT)
DEFINES += -DPLATFORM_WINDOWS
EXTENCION = .exe
else
DEFINES +=  -DPLATFORM_LINUX
EXTENCION = 
endif

all: install app

install:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning..."
	@rm -r -f $(BUILD_DIR)
	@rm -r -f $(OBJ_DIR)

run: install app
	@echo "Running => $(BUILD_DIR)/$(TARGET)$(EXTENCION)"
	@$(BUILD_DIR)/$(TARGET)$(EXTENCION)

app: $(OBJ)
	@echo "Linking => $(BUILD_DIR)/app.exe"
	@$(CC) $(OBJ) $(INCLUDE) $(DEFINES) -o $(BUILD_DIR)/$(TARGET)$(EXTENCION)

$(OBJ_DIR)/%.o: source/%.c
	@echo "Compiling:$<"
	@$(CC) $< $(INCLUDE) $(DEFINES) $(FLAGS) -c -o$@