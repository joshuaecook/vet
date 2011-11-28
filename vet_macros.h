#ifndef VET_MACROS_H
#define VET_MACROS_H

/* vet_macros.h */
/* Copyright (C) 2011 by Joshua E Cook */

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)

#define VET_MAX_NAME 256


#define VET_TEST(NAME)                                                  \
  VET_TEST_PREAMBLE(NAME)                                               \
  static vet_status vet_fn__ ## NAME (struct vet_test *vet)             \
  {                                                                     \
  

#define VET_END                                 \
  return VET_OK;                                \
  }                                             \


#define VET_TEST_PREAMBLE(NAME) \
  static const char *vet_test_name__ ## NAME = QUOTE(NAME);             \
  static struct vet_test vet_test__ ## NAME;                            \
  static vet_status vet_fn__ ## NAME (struct vet_test *);               \
  static void vet_test_init__ ## NAME ()                                \
  {                                                                     \
    vet_test_init( &vet_test__ ## NAME , vet_test_name__ ## NAME , vet_fn__ ## NAME ); \
    return;                                                             \
  }                                                                     \


#define VET_NAME(NAME) vet_test_name__ ## NAME
#define VET_STATUS_FN(NAME) vet_fn__ ## NAME
#define VET_INIT_FN(NAME) vet_test_init__ ## NAME                       

#define VET_ID(NAME)                                                    \
  vet_intern( &vet_test__ ## NAME , VET_NAME(NAME) , VET_STATUS_FN(NAME) , VET_INIT_FN(NAME) )

#define VET(ID) vet_test_vet( ID )                          

#endif
