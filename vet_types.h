#ifndef VET_TYPES_H
#define VET_TYPES_H

/* vet_types.h */
/* Copyright (C) 2011 by Joshua E Cook */

struct vet_entry 
    {
    vet_status_fn fn;
    int expects_failure;
    int expects_signal;
    const char *name;
    };

#endif
