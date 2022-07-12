const N = 4000000000; // Number of 8-bit integers to add
let pages = Math.ceil(N / (64.0 * 1024.0)) + 2; // Add a couple pages for things Clang generates
print("Need " + pages + " Wasm pages");
const mem = new WebAssembly.Memory({initial: pages}); // Allocated all memory at once
const module = new WebAssembly.Module(readbuffer('micro.wasm'));
const instance = new WebAssembly.Instance(module, {"env": {"memory": mem}}).exports;

const __heap_base = instance["__heap_base"];
let data = new Uint8Array(mem.buffer, __heap_base); // Only care about memory we are safe to tamper with

print("Bytes available for writing: " + data.length);
print("Bytes to be written: " + N);

// Write N 8-bit entries each equal to 1 starting at heap base
data.fill(1, 0, N);

print("");

var tStart = Date.now();
var sum = instance["sum_scalar"](__heap_base, N); // Pass real offset to memory
let tScalar = Date.now() - tStart;
print("Scalar: " + (sum >>> 0) + ", " + tScalar + " ms");
tStart = Date.now();
sum = instance["sum_autovec"](__heap_base, N);
let tAutovec = Date.now() - tStart;
print("Auto: " + (sum >>> 0) + ", " + tAutovec + " ms");
tStart = Date.now();
var sum = instance["sum"](__heap_base, N);
let tVec = Date.now() - tStart;
print("Intrinsics: " + (sum >>> 0) + ", " + tVec + " ms");

