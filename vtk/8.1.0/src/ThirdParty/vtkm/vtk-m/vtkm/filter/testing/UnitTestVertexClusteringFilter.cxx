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

#include <vtkm/cont/testing/MakeTestDataSet.h>
#include <vtkm/cont/testing/Testing.h>

#include <vtkm/filter/VertexClustering.h>

using vtkm::cont::testing::MakeTestDataSet;

namespace
{
}

void TestVertexClustering()
{
  vtkm::cont::testing::MakeTestDataSet maker;
  vtkm::cont::DataSet dataSet = maker.Make3DExplicitDataSetCowNose();

  vtkm::filter::VertexClustering clustering;
  vtkm::filter::Result result;

  clustering.SetNumberOfDivisions(vtkm::Id3(3, 3, 3));
  result = clustering.Execute(dataSet);

  VTKM_TEST_ASSERT(result.IsValid(), "results should be valid");
  VTKM_TEST_ASSERT(clustering.MapFieldOntoOutput(result, dataSet.GetPointField("pointvar")),
                   "Point field mapping failed.");
  VTKM_TEST_ASSERT(clustering.MapFieldOntoOutput(result, dataSet.GetCellField("cellvar")),
                   "Cell field mapping failed.");

  vtkm::cont::DataSet output = result.GetDataSet();
  VTKM_TEST_ASSERT(output.GetNumberOfCoordinateSystems() == 1,
                   "Number of output coordinate systems mismatch");

  using FieldArrayType = vtkm::cont::ArrayHandle<vtkm::Float32>;
  FieldArrayType pointvar = output.GetPointField("pointvar").GetData().Cast<FieldArrayType>();
  FieldArrayType cellvar = output.GetCellField("cellvar").GetData().Cast<FieldArrayType>();

  // test
  const vtkm::Id output_points = 7;
  vtkm::Float64 output_point[output_points][3] = {
    { 0.0174716, 0.0501928, 0.0930275 }, { 0.0307091, 0.1521420, 0.05392490 },
    { 0.0174172, 0.1371240, 0.1245530 }, { 0.0480879, 0.1518740, 0.10733400 },
    { 0.0180085, 0.2043600, 0.1453160 }, { -.000129414, 0.00247137, 0.17656100 },
    { 0.0108188, 0.1527740, 0.1679140 }
  };

  vtkm::Float32 output_pointvar[output_points] = { 28.f, 19.f, 25.f, 15.f, 16.f, 21.f, 30.f };
  vtkm::Float32 output_cellvar[] = { 145.f, 134.f, 138.f, 140.f, 149.f, 144.f };

  {
    typedef vtkm::cont::CellSetSingleType<> CellSetType;
    CellSetType cellSet;
    output.GetCellSet(0).CopyTo(cellSet);
    auto cellArray =
      cellSet.GetConnectivityArray(vtkm::TopologyElementTagPoint(), vtkm::TopologyElementTagCell());
    std::cerr << "output_pointIds = " << cellArray.GetNumberOfValues() << "\n";
    std::cerr << "output_pointId[] = ";
    vtkm::cont::printSummary_ArrayHandle(cellArray, std::cerr, true);
  }

  {
    typedef vtkm::Vec<vtkm::Float64, 3> PointType;
    vtkm::cont::ArrayHandle<PointType> pointArray;
    output.GetCoordinateSystem(0).GetData().CopyTo(pointArray);
    std::cerr << "output_points = " << pointArray.GetNumberOfValues() << "\n";
    std::cerr << "output_point[] = ";
    vtkm::cont::printSummary_ArrayHandle(pointArray, std::cerr, true);
  }

  vtkm::cont::printSummary_ArrayHandle(pointvar, std::cerr, true);
  vtkm::cont::printSummary_ArrayHandle(cellvar, std::cerr, true);

  typedef vtkm::Vec<vtkm::Float64, 3> PointType;
  vtkm::cont::ArrayHandle<PointType> pointArray;
  output.GetCoordinateSystem(0).GetData().CopyTo(pointArray);
  VTKM_TEST_ASSERT(pointArray.GetNumberOfValues() == output_points,
                   "Number of output points mismatch");
  for (vtkm::Id i = 0; i < pointArray.GetNumberOfValues(); ++i)
  {
    const PointType& p1 = pointArray.GetPortalConstControl().Get(i);
    PointType p2 = vtkm::make_Vec(output_point[i][0], output_point[i][1], output_point[i][2]);
    VTKM_TEST_ASSERT(test_equal(p1, p2), "Point Array mismatch");
  }

  {
    auto portal = pointvar.GetPortalConstControl();
    VTKM_TEST_ASSERT(portal.GetNumberOfValues() == output_points, "Point field size mismatch.");
    for (vtkm::Id i = 0; i < portal.GetNumberOfValues(); ++i)
    {
      VTKM_TEST_ASSERT(test_equal(portal.Get(i), output_pointvar[i]), "Point field mismatch.");
    }
  }

  {
    auto portal = cellvar.GetPortalConstControl();
    VTKM_TEST_ASSERT(portal.GetNumberOfValues() == 6, "Cell field size mismatch.");
    for (vtkm::Id i = 0; i < portal.GetNumberOfValues(); ++i)
    {
      VTKM_TEST_ASSERT(test_equal(portal.Get(i), output_cellvar[i]), "Cell field mismatch.");
    }
  }
}

int UnitTestVertexClusteringFilter(int, char* [])
{
  return vtkm::cont::testing::Testing::Run(TestVertexClustering);
}
