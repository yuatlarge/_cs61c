#include "simd.h"
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

long long int sum(int vals[NUM_ELEMS]) {
  clock_t start = clock();

  long long int sum = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  long long int sum = 0;

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      if (vals[i] >= 128)
        sum += vals[i];
      if (vals[i + 1] >= 128)
        sum += vals[i + 1];
      if (vals[i + 2] >= 128)
        sum += vals[i + 2];
      if (vals[i + 3] >= 128)
        sum += vals[i + 3];
    }

    // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
    // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
    // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(
      127); // This is a vector with 127s in it... Why might you need this?
  long long int result = 0; // This is where you should put your final result!
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    int tail = NUM_ELEMS % 4;
    for (int i = 0; i < tail; ++i) {
      if (vals[i] >= 128)
        result += vals[i];
    }
    for (int i = tail; i < NUM_ELEMS; i += 4) {
      __m128i compare_vec = _mm_setzero_si128();
      compare_vec = _mm_loadu_si128((__m128i *)(vals + i));
      __m128i out = _mm_cmpgt_epi32(compare_vec, _127);
      __m128i rest = _mm_and_si128(out, compare_vec);
      int tmp[4];
      _mm_storeu_si128((__m128i *)tmp, rest);
      result += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    }
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(127);
  long long int result = 0;
  /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    int tail = NUM_ELEMS % 16;
    for (int i = 0; i < tail; ++i)
      if (vals[i] >= 128)
        result += vals[i];

    for (int i = tail; i < NUM_ELEMS; i += 16) {
      int tmp[4];

      __m128i compare_vec1 = _mm_setzero_si128();
      compare_vec1 = _mm_loadu_si128((__m128i *)(vals + i));
      __m128i out1 = _mm_cmpgt_epi32(compare_vec1, _127);
      __m128i rest1 = _mm_and_si128(out1, compare_vec1);
      _mm_storeu_si128((__m128i *)tmp, rest1);
      result += tmp[0] + tmp[1] + tmp[2] + tmp[3];

      __m128i compare_vec2 = _mm_setzero_si128();
      compare_vec2 = _mm_loadu_si128((__m128i *)(vals + i + 4));
      __m128i out2 = _mm_cmpgt_epi32(compare_vec2, _127);
      __m128i rest2 = _mm_and_si128(out2, compare_vec2);
      _mm_storeu_si128((__m128i *)tmp, rest2);
      result += tmp[0] + tmp[1] + tmp[2] + tmp[3];

      __m128i compare_vec3 = _mm_setzero_si128();
      compare_vec3 = _mm_loadu_si128((__m128i *)(vals + i + 8));
      __m128i out3 = _mm_cmpgt_epi32(compare_vec3, _127);
      __m128i rest3 = _mm_and_si128(out3, compare_vec3);
      _mm_storeu_si128((__m128i *)tmp, rest3);
      result += tmp[0] + tmp[1] + tmp[2] + tmp[3];

      __m128i compare_vec4 = _mm_setzero_si128();
      compare_vec4 = _mm_loadu_si128((__m128i *)(vals + i + 12));
      __m128i out4 = _mm_cmpgt_epi32(compare_vec4, _127);
      __m128i rest4 = _mm_and_si128(out4, compare_vec4);
      _mm_storeu_si128((__m128i *)tmp, rest4);
      result += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    }
  }

  /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}
