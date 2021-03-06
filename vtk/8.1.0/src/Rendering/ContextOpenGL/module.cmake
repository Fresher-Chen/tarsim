vtk_module(vtkRenderingContextOpenGL
  LEGACY 8.1 "Please switch to using `OpenGL2` VTK_RENDERING_BACKEND to replace this module."
  IMPLEMENTS
    vtkRenderingContext2D
  BACKEND
    OpenGL
  IMPLEMENTATION_REQUIRED_BY_BACKEND
  KIT
    vtkOpenGL
  DEPENDS
    vtkCommonCore
    vtkCommonDataModel
    vtkRenderingContext2D
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingOpenGL
  PRIVATE_DEPENDS
    vtkCommonMath
    vtkCommonTransforms
  )