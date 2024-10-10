
CC = g++

FE = cpp

# Флаги компилятора
FLAGS = 
DEBUG = -g
MAIN_DIR = ./build
OUT_DIR = $(MAIN_DIR)/out
BUILD_DIR = $(MAIN_DIR)/bin
PROGRAM_MAIN = main.$(FE)

NOT_INCLUDE_FILES := ! -name 'main.$(FE)'  # Исключаем main.cpp
NOT_INCLUDE_DIRS := -not -path "./build/*"

# Находим все исходные файлы, исключая указанные
ALL_SOURCES := $(shell find . -name '*.$(FE)' $(NOT_INCLUDE_FILES) $(NOT_INCLUDE_DIRS))
ALL_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %.$(FE),%.o,$(ALL_SOURCES)))) 

# Основное правило
all: clean dirCreation $(PROGRAM_MAIN)

# Создание директорий
dirCreation:
	mkdir -p $(OUT_DIR)
	mkdir -p $(BUILD_DIR)

# Компиляция основного исполняемого файла
$(PROGRAM_MAIN): $(ALL_OBJECTS) | print_end 
	$(CC) $(PROGRAM_MAIN) $(DEBUG) $^ $(FLAGS) -o $(OUT_DIR)/EnviroMonitor
	@echo "Build complete."

# Устанавливаем путь для поиска файлов
vpath %.$(FE) $(sort $(dir $(ALL_SOURCES)))

# Правило компиляции объектов
$(BUILD_DIR)/%.o: %.$(FE)
	$(CC) -c $(FLAGS) $< -o $@

.PHONY: clean

print_end:
	@echo "Compiled Build objects successfully."

# Очистка
clean:
	@clear
	rm -rf $(MAIN_DIR)
	@echo "Cleaned successfully."

# Правило для git
git:
	git add .
	git commit -m "$(commit)"
	git push
	@echo "Compiled and pushed successfully."
