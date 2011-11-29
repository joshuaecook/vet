#ifndef VET_MACROS_H
#define VET_MACROS_H

/* vet_macros.h */
/* Copyright (C) 2011 by Joshua E Cook */

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)

#define VET_MAX_NAME 256


#define VET_TEST(NAME)                                                  \
  VET_TEST_PREAMBLE(NAME)                                               \
  static vet_status vet_fn__ ## NAME (struct vet_entry *vet)            \
  {                                                                     \
  

#define VET_END                                 \
  return VET_OK;                                \
  }                                             \


#define VET_TEST_PREAMBLE(NAME)                                         \
  static vet_status vet_fn__ ## NAME (struct vet_entry *);              \
  static void vet_init__ ## NAME (struct vet_entry *vet)                \
  {                                                                     \
    vet_entry_init( vet , VET_NAME(NAME) , VET_STATUS_FN(NAME) );       \
    return;                                                             \
  }                                                                     \
  

#define VET_NAME(NAME) QUOTE(NAME)
#define VET_STATUS_FN(NAME) vet_fn__ ## NAME
#define VET_INIT_FN(NAME) vet_init__ ## NAME                       

#define VET_ID(NAME) vet_intern( VET_NAME(NAME) , VET_INIT_FN(NAME) )

#define VET(ID) vet_entry_vet( ID )                          

#endif
