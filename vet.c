/* vet.c */
/* Copyright (C) 2011 by Joshua E Cook */
#include <sys/wait.h>
#include <sys/mman.h>

#include <err.h>
#include <search.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vet.h"
#include "vet_types.h"

static struct vet_globals vet_global;

static int vet_entry_compare (const void *, const void *);

void *vet_intern (const char *name, vet_entry_init_fn init)
    {
    struct vet_entry mock;
    struct vet_entry *anew;
    void *tentry = vet_global.tentry;
    void *ptr;

    mock.name = name;
    
    ptr = tfind(&mock, &tentry, vet_entry_compare);

    if (ptr != NULL) return *(void **)ptr;

    anew = (struct vet_entry *)malloc(sizeof(struct vet_entry));
    if (anew == NULL) err(EXIT_FAILURE, "Failed to allocate %s test case node.", name);

    init(anew);

    ptr = tsearch(anew, &tentry, vet_entry_compare);
    if (ptr == NULL) errx(EXIT_FAILURE, "Failed to internalize %s test case node.", name);

    return anew;
    }

void vet_entry_init (struct vet_entry *obj, const char *name, vet_status_fn def)
    {

    obj->name = name;
    obj->fn = def;
    obj->expects_failure = 0;
    obj->expects_signal = 0;

    return;
    }

vet_status vet_entry_vet (struct vet_entry *obj)
    {
    struct vet_shared *shm;
    vet_status stat;
    pid_t pid, wpid;
    int xstat, xsig;

#ifndef VET_NFORK
    shm = mmap(NULL, sizeof(struct vet_shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    memset(shm, 0, sizeof(struct vet_shared));
    shm->status = VET_NOT_YET;

    pid = fork();
    if (0 == pid) 
        {
        /* Child process executes the test and exits. */
        stat = obj->fn(obj);
        shm->status = stat;
        _exit(EXIT_SUCCESS);
        }
    else if (pid < 0) 
        err(EXIT_FAILURE, "Failed to fork %s test case.", obj->name);

    printf("testing %s...", obj->name);
    do {
        wpid = waitpid(pid, &xstat, 0);
    } while (wpid < 0);

    if (wpid != pid)
        errx(EXIT_FAILURE, "Expected %d from waitpid() but got %d.", pid, wpid);

    if (WIFEXITED(xstat))
        {
        if (EXIT_SUCCESS == WEXITSTATUS(xstat))
            stat = (obj->expects_failure || obj->expects_signal
                ? VET_FAILED : shm->status);
        else
            stat = (obj->expects_failure || obj->expects_signal
                ? VET_OK : VET_FAILED);
        }
    else if (WIFSIGNALED(xstat))
        {
        xsig = WTERMSIG(xstat);
        stat = xsig == obj->expects_signal ? VET_OK : VET_SIGNALED;
        }
    else if (WIFSTOPPED(xstat))
        stat = VET_STOPPED;
    else if (WCOREDUMP(xstat))
        stat = VET_DUMPED;
    else
        stat = VET_NOT_YET;
#else
    printf("testing %s...", obj->name);
    stat = obj->fn(obj);
#endif

    switch (stat)
        {
        case VET_OK:
            printf(" OK!\n");
            break;
        case VET_FAILED:
            printf(" FAIL!\n");
            break;
        case VET_SIGNALED:
            printf(" SIGNALED! %s\n", strsignal(xsig));
            break;
        case VET_STOPPED:
            printf(" STOPPED!\n");
            break;
        case VET_DUMPED:
            printf(" DUMPED!\n");
            break;
        case VET_NOT_YET:
            printf(" NOT YET!\n");
            break;
        }

    return stat;
    }

void vet_expects_signal (struct vet_entry *entry, int sig)
    {
    entry->expects_signal = sig;
    }

static int vet_entry_compare (const void *a, const void *b)
    {
    const struct vet_entry *va = a;
    const struct vet_entry *vb = b;

    if (a == NULL && b == NULL) return  0;
    if (b != NULL && a == NULL) return -1;
    if (a != NULL && b == NULL) return  1;
    return strncmp(va->name, vb->name, VET_MAX_NAME);
    }

