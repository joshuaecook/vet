#ifndef VET_H
#define VET_H

/* vet.h */
/* Copyright (C) 2011 by Joshua E Cook */

/* Vet, a testing library for C 
 *
 */

#include "vet_macros.h"

typedef enum { VET_NOT_YET = -1, VET_OK, VET_FAILED, VET_SIGNALED, VET_STOPPED, VET_DUMPED } vet_status;


/* The test case type. */
struct vet_test;

/* The test function type. */
typedef vet_status(*vet_status_fn)(struct vet_test *);

/* The test case init function type. */
typedef void(*vet_test_init_fn)(void);

struct vet_test *vet_test_init (struct vet_test *, const char *, vet_status_fn);

struct vet_entry;
vet_status vet_test_vet (struct vet_entry *);
void *vet_intern (struct vet_test *, const char *, vet_status_fn, vet_test_init_fn);

void vet_expects_signal (struct vet_entry *, int);

#endif
