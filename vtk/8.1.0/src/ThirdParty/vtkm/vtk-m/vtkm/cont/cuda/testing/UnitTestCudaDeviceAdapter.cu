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

#ifdef VTKM_DEVICE_ADAPTER
#undef VTKM_DEVICE_ADAPTER
#endif
#define VTKM_DEVICE_ADAPTER VTKM_DEVICE_ADAPTER_ERROR

#include <vtkm/cont/cuda/DeviceAdapterCuda.h>

#include <vtkm/cont/cuda/internal/testing/Testing.h>
#include <vtkm/cont/testing/TestingDeviceAdapter.h>

int UnitTestCudaDeviceAdapter(int, char* [])
{
  int result = vtkm::cont::testing::TestingDeviceAdapter<vtkm::cont::DeviceAdapterTagCuda>::Run();
  return vtkm::cont::cuda::internal::Testing::CheckCudaBeforeExit(result);
}
