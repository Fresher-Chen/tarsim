//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2014 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2014 UT-Battelle, LLC.
//  Copyright 2014 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_internal_Assume_h
#define vtk_m_internal_Assume_h

#include <vtkm/Assert.h>

// Description:
// VTKM_ASSUME instructs the compiler that a certain non-obvious condition will
// *always* be true. Beware that if cond is false at runtime, the results are
// unpredictable (and likely catastrophic). A runtime assertion is added so
// that debugging builds may easily catch violations of the condition.
//
// A useful application of this macro is when a method is passed in a
// vtkm::Vec that is uninitialized and conditional fills the vtkm::Vec
// based on other runtime information such as cell type. This allows you to
// assert that only valid cell types will be used, producing more efficient
// code.
//
#define VTKM_ASSUME(cond)                                                                          \
  VTKM_SWALLOW_SEMICOLON_PRE_BLOCK                                                                 \
  {                                                                                                \
    const bool c = cond;                                                                           \
    VTKM_ASSERT("Bad assumption in VTKM_ASSUME: " #cond&& c);                                      \
    VTKM_ASSUME_IMPL(c);                                                                           \
    (void)c; /* Prevents unused var warnings */                                                    \
  }                                                                                                \
  VTKM_SWALLOW_SEMICOLON_POST_BLOCK

// VTKM_ASSUME_IMPL is compiler-specific:
#if defined(__CUDA_ARCH__)
//For all versions of CUDA this is a no-op while we look
//for a CUDA asm snippet that replicates this kind of behavior
#define VTKM_ASSUME_IMPL(cond) (void)0 /* no-op */
#else

#if defined(VTKM_MSVC)
#define VTKM_ASSUME_IMPL(cond) __assume(cond)
#elif defined(VTKM_ICC)
#define VTKM_ASSUME_IMPL(cond) __assume(cond)
#elif defined(VTKM_GCC) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
// Added in 4.5.0:
#define VTKM_ASSUME_IMPL(cond)                                                                     \
  if (!(cond))                                                                                     \
  __builtin_unreachable()
#elif defined(VTKM_CLANG)
#define VTKM_ASSUME_IMPL(cond)                                                                     \
  if (!(cond))                                                                                     \
  __builtin_unreachable()
#else
#define VTKM_ASSUME_IMPL(cond) (void)0 /* no-op */
#endif

#endif

#endif // vtk_m_internal_Assume_h
