CC = gcc -std=c11
CFLAGS = -c -Wall -Wextra -Werror
TEST_FLAGS = -lcheck -g

OS := $(shell uname -s)
ifeq ($(OS), Linux)
    D_ENV=__LINUX__
    TEST_FLAGS += -lrt -lpthread -lsubunit -lm
    OPEN_GCOV= xdg-open coverage/src/index.html
else
    D_ENV=__MAC_OS__
    OPEN_GCOV= open coverage/index.html
endif

TARGET = decimal

C_FILES = arithmetic.c others.c converters.c comparisons.c utils.c long_utils.c
O_FILES = arithmetic.o others.o converters.o comparisons.o utils.o long_utils.o

all: clean decimal.a test gcov_report

$(TARGET).a:
	@$(CC) $(CFLAGS) $(C_FILES) -D$(D_ENV)
	@ar rc lib$(TARGET).a $(O_FILES)
	@ranlib lib$@
	@cp lib$(TARGET).a $(TARGET).a
	@rm *.o

clean:
	@rm -rf *.out *.o *.gch  log.txt logTest.txt
	@rm -rf coverage *.a *.gcda *gcno *.info Test *.cfg
	@rm -rf report/
	@rm -rf temp/
	@rm -rf a.out.dSYM/
	@rm -rf Test.dSYM/
	@rm -rf s21_test.c

test:
	@checkmk clean_mode=1 test.check > test.c
	@clang-format -i --style=Google test.c
	@$(CC) $(C_FILES) test.c $(TEST_FLAGS) -D$(D_ENV) --coverage -o Test
	@./Test

gcov_report: test
	@lcov -c -d . -o coverage.info
	@genhtml coverage.info -o coverage
	@$(OPEN_GCOV)