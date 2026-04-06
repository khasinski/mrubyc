/*
 * Benchmark runner for mruby/c
 * Runs a .mrb file multiple iterations and reports timing.
 *
 * Usage: bench_runner <file.mrb> [iterations]
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mrubyc.h"

#if !defined(MRBC_MEMORY_SIZE)
#define MRBC_MEMORY_SIZE (1024*60)
#endif
static uint8_t memory_pool[MRBC_MEMORY_SIZE];

static uint8_t *load_mrb_file(const char *filename)
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
  }
  fclose(fp);
  return p;
}

static double run_once(uint8_t *mrbbuf)
{
  mrbc_init_alloc(memory_pool, MRBC_MEMORY_SIZE);
  mrbc_init_global();
  mrbc_init_class();

  mrbc_vm *vm = mrbc_vm_open(NULL);
  if( vm == NULL ) return -1.0;

  if( mrbc_load_mrb(vm, mrbbuf) != 0 ) {
    mrbc_vm_close(vm);
    return -1.0;
  }

  mrbc_vm_begin(vm);

  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  mrbc_vm_run(vm);
  clock_gettime(CLOCK_MONOTONIC, &t1);

  mrbc_vm_end(vm);
  mrbc_vm_close(vm);

  double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;
  return elapsed;
}

int main(int argc, char *argv[])
{
  if( argc < 2 ) {
    fprintf(stderr, "Usage: %s <file.mrb> [iterations]\n", argv[0]);
    return 1;
  }

  int iterations = 10;
  if( argc >= 3 ) iterations = atoi(argv[2]);
  if( iterations < 1 ) iterations = 1;

  uint8_t *mrbbuf = load_mrb_file(argv[1]);
  if( mrbbuf == NULL ) return 1;

  hal_init();

  double total = 0.0;
  double best = 1e30;
  double worst = 0.0;

  for( int i = 0; i < iterations; i++ ) {
    double t = run_once(mrbbuf);
    if( t < 0 ) {
      fprintf(stderr, "Error on iteration %d\n", i);
      free(mrbbuf);
      return 1;
    }
    total += t;
    if( t < best ) best = t;
    if( t > worst ) worst = t;
  }

  printf("Benchmark: %s\n", argv[1]);
  printf("Iterations: %d\n", iterations);
  printf("Total:   %.6f s\n", total);
  printf("Average: %.6f s\n", total / iterations);
  printf("Best:    %.6f s\n", best);
  printf("Worst:   %.6f s\n", worst);

  free(mrbbuf);
  return 0;
}
