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

struct vet_shared
    {
    vet_status status;
    };

struct vet_globals
    {
/* Search tree of test cases. */
    void *tentry;
/* I/O pipes for the test case child process. */
    int vet_stdin_pipe[2];
    int vet_stdout_pipe[2];
    int vet_stderr_pipe[2];
    };

#endif
