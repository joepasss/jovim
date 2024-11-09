BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/jovim

TARGET_FILES = \
							 ./src/main.c \
							 ./src/jovim_error.c \
							 ./src/input.c \
							 ./src/output.c \
							 ./src/welcome.c

TARGET_HEADERS = \
								 ./src/common.h \
								 ./src/jovim.h \
								 ./src/jovim_error.h \
								 ./src/input.h \
								 ./src/output.h \
								 ./src/welcome.h

$(TARGET): $(TARGET_FILES) | $(BUILD_DIR)
	$(CC) $(TARGET_FILES) -o $(TARGET) -Wall -Wextra -pedantic -std=c99

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

%.o: %.c $(TARGET_HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(BUILD_DIR)
