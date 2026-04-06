/*
 * Benchmark runner for mruby/c
 * Usage: bench_runner <file.mrb> <iterations>
 *
 * Runs the given .mrb file the specified number of times and reports
 * wall-clock timing statistics.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mrubyc.h"

#if !defined(MRBC_MEMORY_SIZE)
#define MRBC_MEMORY_SIZE (1024*60)
#endif

static uint8_t *load_mrb_file(const char *filename, size_t *out_size)
{
  FILE *fp = fopen(filename, "rb");
  if( fp == NULL ) {
    fprintf(stderr, "File not found (%s)\n", filename);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  uint8_t *p = malloc(size);
  if( p != NULL ) {
    fread(p, sizeof(uint8_t), size, fp);
  } else {
    fprintf(stderr, "Memory allocate error.\n");
  }
  fclose(fp);

  if( out_size ) *out_size = size;
  return p;
}

static double get_time_sec(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char *argv[])
{
  if( argc < 2 || argc > 3 ) {
    fprintf(stderr, "Usage: %s <file.mrb> [iterations]\n", argv[0]);
    return 1;
  }

  int iterations = 10;
  if( argc == 3 ) {
    iterations = atoi(argv[2]);
    if( iterations < 1 ) iterations = 1;
  }

  size_t mrb_size = 0;
  uint8_t *mrbbuf = load_mrb_file(argv[1], &mrb_size);
  if( mrbbuf == NULL ) return 1;

  double *times = malloc(sizeof(double) * iterations);
  double total = 0;
  double min_t = 1e30;
  double max_t = 0;

  printf("Benchmark: %s (%zu bytes)\n", argv[1], mrb_size);
  printf("Iterations: %d\n", iterations);
  printf("---\n");

  for( int i = 0; i < iterations; i++ ) {
    /* Fresh memory pool each iteration */
    uint8_t *memory_pool = malloc(MRBC_MEMORY_SIZE);
    if( !memory_pool ) {
      fprintf(stderr, "Failed to allocate memory pool\n");
      free(mrbbuf);
      free(times);
      return 1;
    }

    hal_init();
    mrbc_init_alloc(memory_pool, MRBC_MEMORY_SIZE);
    mrbc_init_global();
    mrbc_init_class();

    mrbc_vm *vm = mrbc_vm_open(NULL);
    if( vm == NULL ) {
      fprintf(stderr, "Error: Can't assign VM.\n");
      free(memory_pool);
      continue;
    }

    if( mrbc_load_mrb(vm, mrbbuf) != 0 ) {
      fprintf(stderr, "Error: Can't load mrb.\n");
      mrbc_vm_close(vm);
      free(memory_pool);
      continue;
    }

    mrbc_vm_begin(vm);

    double t0 = get_time_sec();
    mrbc_vm_run(vm);
    double t1 = get_time_sec();

    mrbc_vm_end(vm);
    mrbc_vm_close(vm);
    free(memory_pool);

    double elapsed = t1 - t0;
    times[i] = elapsed;
    total += elapsed;
    if( elapsed < min_t ) min_t = elapsed;
    if( elapsed > max_t ) max_t = elapsed;

    printf("  Run %2d: %.6f s\n", i+1, elapsed);
  }

  double avg = total / iterations;

  /* Compute stddev */
  double variance = 0;
  for( int i = 0; i < iterations; i++ ) {
    double d = times[i] - avg;
    variance += d * d;
  }
  double stddev = 0;
  if( iterations > 1 ) {
    stddev = sqrt(variance / (iterations - 1));
  }

  printf("---\n");
  printf("Min:    %.6f s\n", min_t);
  printf("Max:    %.6f s\n", max_t);
  printf("Avg:    %.6f s\n", avg);
  printf("Stddev: %.6f s\n", stddev);
  printf("Total:  %.6f s\n", total);

  free(times);
  free(mrbbuf);
  return 0;
}
