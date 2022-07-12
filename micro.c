#include <stdint.h>
#include <wasm_simd128.h>

/* 8 to 32 bit extension using Wasm SIMD intrinsics, add N 8-bit integers into
 * a 32-bit value
 */
uint32_t sum(uint8_t * input, uint32_t N) {
  uint32_t n_vec, n_scalar, res = 0;
  n_vec = N / 16;
  n_scalar = N % 16;

  v128_t vres = wasm_i32x4_const_splat(0);

  for (uint32_t i = 0; i < n_vec; ++i) {
    v128_t v8x16 = wasm_v128_load(input);
    v128_t v16x8 = wasm_u16x8_extend_high_u8x16(v8x16);
    v128_t v32x4 = wasm_u32x4_extend_high_u16x8(v16x8);
    vres = wasm_i32x4_add(vres, v32x4);
    v32x4 = wasm_u32x4_extend_low_u16x8(v16x8);
    vres = wasm_i32x4_add(vres, v32x4);
    v16x8 = wasm_u16x8_extend_low_u8x16(v8x16);
    v32x4 = wasm_u32x4_extend_high_u16x8(v16x8);
    vres = wasm_i32x4_add(vres, v32x4);
    v32x4 = wasm_u32x4_extend_low_u16x8(v16x8);
    vres = wasm_i32x4_add(vres, v32x4);
    input += 16;
  }

  res = wasm_u32x4_extract_lane(vres, 0) + wasm_u32x4_extract_lane(vres, 1)
      + wasm_u32x4_extract_lane(vres, 2) + wasm_u32x4_extract_lane(vres, 3);

  for (uint32_t i = 0; i < n_scalar; ++i) {
    res += *(input++);
  }

  return res;
}

/* Scalar version for comparison */
uint32_t sum_scalar(uint8_t * input, uint32_t N) {
  uint32_t res = 0;

#pragma clang loop vectorize(disable) unroll(disable) interleave(disable)
  for (uint32_t i = 0; i < N; ++i) {
    res += *(input++);
  }

  return res;
}

/* Autovectorized variety */
uint32_t sum_autovec(uint8_t * input, uint32_t N) {
  uint32_t res = 0;

  for (uint32_t i = 0; i < N; ++i) {
    res += *(input++);
  }

  return res;
}

