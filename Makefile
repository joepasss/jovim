BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/jovim

$(TARGET): ./src/main.c | $(BUILD_DIR)
	$(CC) ./src/main.c -o $(TARGET) -Wall -Wextra -pedantic -std=c99

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
