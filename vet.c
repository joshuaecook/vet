/* vet.c */
/* Copyright (C) 2011 by Joshua E Cook */
#include <sys/wait.h>

#include <err.h>
#include <search.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vet.h"
#include "vet_types.h"

/* Tree of test cases. */
void *ttest = NULL;

static int vet_entry_compare (const void *, const void *);

void *vet_intern (struct vet_test *obj, const char *name, vet_status_fn def, vet_test_init_fn init)
    {
    struct vet_entry mock;
    struct vet_entry *anew;
    void *ptr;

    mock.key = name;
    
    ptr = tfind(&mock, &ttest, vet_entry_compare);

    if (ptr != NULL) return *(void **)ptr;

    init();

    anew = (struct vet_entry *)malloc(sizeof(struct vet_entry));
    if (anew == NULL) err(EXIT_FAILURE, "Failed to allocate %s test case node.", name);
    anew->key = name;
    anew->val = obj;

    ptr = tsearch(anew, &ttest, vet_entry_compare);
    if (ptr == NULL) errx(EXIT_FAILURE, "Failed to internalize %s test case node.", name);

    return anew;
    }

struct vet_test *vet_test_init (struct vet_test *obj, const char *name, vet_status_fn def)
    {

    obj->name = name;
    obj->fn = def;
    obj->expects_failure = 0;
    obj->expects_signal = 0;

    return obj;
    }

vet_status vet_test_vet (struct vet_entry *obj)
    {
    struct vet_test *test;
    vet_status stat;
    pid_t pid, wpid;
    int xstat, xsig;

    test = obj->val;

    pid = fork();
    if (0 == pid) 
        {
        /* Child process executes the test and exits. */
        stat = test->fn(test);
        _exit(VET_OK == stat ? EXIT_SUCCESS : EXIT_FAILURE);
        }
    else if (pid < 0) 
        err(EXIT_FAILURE, "Failed to fork %s test case.", test->name);

    printf("testing %s...", test->name);
    do {
        wpid = waitpid(pid, &xstat, 0);
    } while (wpid < 0);

    if (wpid != pid)
        errx(EXIT_FAILURE, "Expected %d from waitpid() but got %d.", pid, wpid);

    if (WIFEXITED(xstat))
        {
        stat = EXIT_SUCCESS == WEXITSTATUS(xstat) ? VET_OK : VET_FAILED;
        }
    else if (WIFSIGNALED(xstat))
        {
        xsig = WTERMSIG(xstat);
        stat = xsig == test->expects_signal ? VET_OK : VET_SIGNALED;
        }
    else if (WIFSTOPPED(xstat))
        {
        stat = VET_STOPPED;
        }
    else if (WCOREDUMP(xstat))
        {
        stat = VET_DUMPED;
        }
    else
        {
        stat = VET_NOT_YET;
        }

    switch (stat)
        {
        case VET_OK:
            printf(" OK!\n");
            break;
        case VET_FAILED:
            printf(" FAIL!\n");
            break;
        case VET_SIGNALED:
            printf(" SIGNALED!\n");
            printf("---\n");
            psignal(xsig, NULL);
            printf("---\n");
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
    entry->val->expects_signal = sig;
    }

static int vet_entry_compare (const void *a, const void *b)
    {
    const struct vet_entry *va = a;
    const struct vet_entry *vb = b;

    if (a == NULL && b == NULL) return  0;
    if (b != NULL && a == NULL) return -1;
    if (a != NULL && b == NULL) return  1;
    return strncmp(va->key, vb->key, VET_MAX_NAME);
    }

