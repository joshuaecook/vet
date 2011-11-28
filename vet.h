#ifndef VET_H
#define VET_H

/* vet.h */
/* Copyright (C) 2011 by Joshua E Cook */

#include "vet_macros.h"

typedef enum { VET_NOT_YET = -1, VET_OK, VET_FAILED, VET_SIGNALED, VET_STOPPED, VET_DUMPED } vet_status;


/* The test case type. */
struct vet_entry;

/* The test function type. */
typedef vet_status(*vet_status_fn)(struct vet_entry *);

/* The test case init function type. */
typedef void(*vet_entry_init_fn)(struct vet_entry *);

void vet_entry_init (struct vet_entry *, const char *, vet_status_fn);

vet_status vet_entry_vet (struct vet_entry *);
void *vet_intern (const char *, vet_entry_init_fn);

void vet_expects_signal (struct vet_entry *, int);

#endif
