/*
 * Benchmark runner for mruby/c
 * Usage: bench_runner <file.mrb> <iterations>
 *
 * Runs the given .mrb file <iterations> times and reports timing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mrubyc.h"

#if !defined(MRBC_MEMORY_SIZE)
#define MRBC_MEMORY_SIZE (1024*256)
#endif
static uint8_t memory_pool[MRBC_MEMORY_SIZE];

uint8_t * load_mrb_file(const char *filename)
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
  return p;
}

static double get_time_ms(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

int main(int argc, char *argv[])
{
  if( argc < 2 || argc > 3 ) {
    printf("Usage: %s <xxxx.mrb> [iterations]\n", argv[0]);
    return 1;
  }

  int iterations = 1;
  if( argc == 3 ) {
    iterations = atoi(argv[2]);
    if( iterations < 1 ) iterations = 1;
  }

  uint8_t *mrbbuf = load_mrb_file(argv[1]);
  if( mrbbuf == NULL ) return 1;

  double total_ms = 0.0;
  double min_ms = 1e9;
  double max_ms = 0.0;

  for( int i = 0; i < iterations; i++ ) {
    hal_init();
    mrbc_init_alloc(memory_pool, MRBC_MEMORY_SIZE);
    mrbc_init_global();
    mrbc_init_class();

    mrbc_vm *vm = mrbc_vm_open(NULL);
    if( vm == NULL ) {
      fprintf(stderr, "Error: Can't assign VM.\n");
      free(mrbbuf);
      return 1;
    }

    if( mrbc_load_mrb(vm, mrbbuf) != 0 ) {
      fprintf(stderr, "Error: Can't load mrb.\n");
      mrbc_print_exception(&vm->exception);
      free(mrbbuf);
      return 1;
    }

    mrbc_vm_begin(vm);

    double start = get_time_ms();
    int ret = mrbc_vm_run(vm);
    double elapsed = get_time_ms() - start;

    mrbc_vm_end(vm);
    mrbc_vm_close(vm);

    if( ret != 1 ) {
      fprintf(stderr, "Warning: VM returned %d on iteration %d\n", ret, i);
    }

    total_ms += elapsed;
    if( elapsed < min_ms ) min_ms = elapsed;
    if( elapsed > max_ms ) max_ms = elapsed;
  }

  double avg_ms = total_ms / iterations;
  printf("Benchmark: %s\n", argv[1]);
  printf("  Iterations: %d\n", iterations);
  printf("  Total:  %.2f ms\n", total_ms);
  printf("  Avg:    %.2f ms\n", avg_ms);
  printf("  Min:    %.2f ms\n", min_ms);
  printf("  Max:    %.2f ms\n", max_ms);

  free(mrbbuf);
  return 0;
}
