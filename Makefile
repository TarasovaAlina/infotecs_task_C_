NAME_PROJECT := LogLibrary

BUILD_DIR := build
GTEST_BUILD_DIR := $(BUILD_DIR)/gtest

COVERAGE_INFO := $(GTEST_BUILD_DIR)/coverage.info
FILTERED_INFO := $(GTEST_BUILD_DIR)/coverage_filtered.info
COVERAGE_DIR := $(GTEST_BUILD_DIR)/coverage

all: install

static_library:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target LoggerLibrary

dynamic_library:
	cmake -S . -B $(BUILD_DIR) -DBUILD_SHARED_LIBS=ON
	cmake --build $(BUILD_DIR) --target Program

install:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target Program

test:
	cmake -S . -B $(GTEST_BUILD_DIR)
	cmake --build $(GTEST_BUILD_DIR)
	ctest --test-dir $(GTEST_BUILD_DIR) --output-on-failure

gcov_report:
	find $(GTEST_BUILD_DIR) -name "*.gcda" -delete
	find $(GTEST_BUILD_DIR) -name "*.gcov" -delete

	cmake -S . -B $(GTEST_BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(GTEST_BUILD_DIR)

	ctest --test-dir $(GTEST_BUILD_DIR) --output-on-failure

	lcov --capture \
		--directory $(GTEST_BUILD_DIR) \
		--output-file $(COVERAGE_INFO) \
		--ignore-errors mismatch

	lcov --extract \
		$(COVERAGE_INFO) \
		"*/ThreadClasses/*.cpp" \
		"*/src/*.cpp" \
		--output-file $(FILTERED_INFO)

	genhtml \
		$(FILTERED_INFO) \
		--output-directory $(COVERAGE_DIR)

dvi:
	doxygen Doxyfile
	@echo -e "\nДля просмотра отчета откройте файл ./docs/html/index.html"

dist:
	mkdir -p arc/$(NAME_PROJECT)
	cp -r include arc/$(NAME_PROJECT)/
	cp Makefile CMakeLists.txt main.cpp arc/$(NAME_PROJECT)/
	tar -czf $(NAME_PROJECT)_v2.2.tar.gz -C arc $(NAME_PROJECT)
	rm -rf arc

uninstall:
	rm -rf build

clean:
	rm -rf docs *.gz LogLibrary
