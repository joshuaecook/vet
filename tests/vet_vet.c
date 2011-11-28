/* vet_vet.c */
/* Copyright (C) 2011 by Joshua E Cook */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <vet.h>

#include "vet.c"

/* The prototypical trivial test case. 
 * Unused. 
 */
static struct vet_entry vet_entry___unused_;
static const char *vet_name___unused_ = "unused";
static vet_status vet_fn___unused_ (struct vet_entry *);
static void vet_init___unused_ (struct vet_entry *vet)
    {
    vet_entry_init(vet, "_unused_", vet_fn___unused_);
    return;
    }
static vet_status vet_fn___unused_ (struct vet_entry *vet)
    {
    return VET_OK;
    }

/* The same test case named "vet" using Vet macros. */
VET_TEST(vet)
VET_END

/* Some basic initialization assertions. */
VET_TEST(vet_init)
    {
    int cmp = strncmp(vet->name, "vet_init", VET_MAX_NAME);
    assert(0 == cmp);
    assert(0 == vet->expects_failure);
    assert(0 == vet->expects_signal);
    }
VET_END

/* Manual test creation. */
static vet_status my_manual_test (struct vet_entry *vec)
    {
    return VET_OK;
    }

static void my_manual_test_init (struct vet_entry *obj)
    {
    vet_entry_init(obj, "my_manual_test", my_manual_test);
    return;
    }

VET_TEST(vet_manual)
    {
    struct vet_entry *manual;

    manual = vet_intern("my_manual_test", my_manual_test_init);

    vet_entry_vet(manual);
    }
VET_END

/* Test that always aborts. */
VET_TEST(vet_abort)
    {
    assert(0);
    }
VET_END

/* Test that crashes with a segmentation fault. */
VET_TEST(vet_crash)
    {
    char *ptr = NULL;
    *ptr = 0;
    }
VET_END

/* Running tests. */
VET_TEST(vet_main)
    {
    struct vet_entry *vet;
    struct vet_entry *vet_manual;
    struct vet_entry *vet_init;
    struct vet_entry *vet_abort;
    struct vet_entry *vet_crash;

    vet = VET_ID(vet);
    vet_manual = VET_ID(vet_manual);
    vet_init = VET_ID(vet_init);
    vet_abort = VET_ID(vet_abort);
    vet_crash = VET_ID(vet_crash);

    vet_expects_signal(vet_abort, SIGABRT);
    vet_expects_signal(vet_crash, SIGSEGV);

    VET(vet);
    VET(vet_manual);
    VET(vet_init);
    VET(vet_abort);
    VET(vet_crash);
    }
VET_END

int main (int argc, char *argv[])
    {
    struct vet_entry *vet_main;

    vet_main = VET_ID(vet_main);

    VET(vet_main);

    return EXIT_SUCCESS;
    }
