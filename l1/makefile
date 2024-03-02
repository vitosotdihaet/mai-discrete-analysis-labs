CC = gcc
CFLAGS = -Wall -Wextra -ggdb
TEST_NUMBERS = $(ls *.txt | wc -l)


main.out: main.c
	$(CC) -o main.out $(CFLAGS) main.c -lm

test: main.out
	@find . -name 'ti*.txt' | sort | while read -r test; do \
		test_number=$$(echo "$$test" | sed 's/[^0-9]*//g'); \
		\
		echo -n "TEST #$$test_number:"; \
		\
		./main.out < "$$test" > temp.txt; \
		\
		if diff -q "to$$test_number.txt" temp.txt >/dev/null; then \
			echo " PASSED"; \
		else \
			echo " FAILED"; \
		fi; \
	done
	rm -f temp.txt

clear:
	rm -f *.out *.txt