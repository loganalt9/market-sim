CC = clang

CFLAGS = -Wall -Wextra -g -fsanitize=address \
         -I./market_exchange/infra \
         -I./market_exchange/matching

LDFLAGS = -fsanitize=address

INFRA_SRC = $(wildcard market_exchange/infra/*.c)
MATCH_SRC = $(wildcard market_exchange/matching/*.c)

SRC = $(INFRA_SRC) $(MATCH_SRC)

TESTS = $(wildcard tests/*.c)
BINS = $(patsubst tests/%.c, build/%,$(TESTS))

LIB = build/liblob.so

.PHONY: all clean test lib

all: test lib

build:
	mkdir -p build

# Shared library (for Python)
lib: $(LIB)

$(LIB): $(SRC) | build
	$(CC) -shared -fPIC -O2 \
	      -I./market_exchange/infra \
	      -I./market_exchange/matching \
	      $(SRC) -o $(LIB)

# Test executables (with ASan)
build/%: tests/%.c $(SRC) | build
	$(CC) $(CFLAGS) $< $(SRC) -o $@ $(LDFLAGS)

test: $(BINS)
	@for t in $(BINS); do \
		echo "==> $$t"; \
		./$$t || exit 1; \
	done

clean:
	rm -rf build