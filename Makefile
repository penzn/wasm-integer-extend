CC=clang
CFLAGS=-O2 --target=wasm32 -msimd128
LFLAGS=-Wl,--import-memory,--no-entry,--export-all -nostdlib

TARGETS=micro.wasm repro.wasm

all: $(TARGETS)

%.wasm: %.c
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

clean:
	$(RM) $(TARGETS)
