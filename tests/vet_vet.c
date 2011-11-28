/* vet_vet.c */
/* Copyright (C) 2011 by Joshua E Cook */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <vet.h>
#include "vet_types.h"

#include "vet.c"

/* The prototypical trivial test case. Unused. */
static const char *vet_test_name___unused_ = "unused";
static struct vet_test vet_test___unused_;
static vet_status vet_fn___unused_ (struct vet_test *);
static void vet_test_init___unused_ ()
    {
    vet_test_init(&vet_test___unused_, "_unused_", vet_fn___unused_);
    return;
    }
static vet_status vet_fn___unused_ (struct vet_test *t)
    {
    return VET_OK;
    }

/* The same test case named "vet" using Vet macros. */
VET_TEST(vet)
VET_END

VET_TEST(vet_fail)
    {
    assert(0);
    }
VET_END

/* Some basic initialization assertions. */
VET_TEST(vet_init)
    {
    int cmp = strncmp(vet_test__vet_init.name, "vet_init", VET_MAX_NAME);
    assert(cmp == 0);
    }
VET_END

/* Running tests. */
VET_TEST(vet_main)
    {
    struct vet_entry *vet;
    struct vet_entry *vet_fail;
    struct vet_entry *vet_init;

    vet = VET_ID(vet);
    vet_fail = VET_ID(vet_fail);
    vet_init = VET_ID(vet_init);

    vet_expects_signal(vet_fail, SIGABRT);

    VET(vet);
    VET(vet_fail);
    VET(vet_init);
    }
VET_END

int main (int argc, char *argv[])
    {
    struct vet_entry *vet_main;

    vet_main = VET_ID(vet_main);

    VET(vet_main);

    return EXIT_SUCCESS;
    }
