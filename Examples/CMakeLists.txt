### Find VTK
FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE( IterativeWarp IterativeWarp.cxx )
TARGET_LINK_LIBRARIES( IterativeWarp vtkKinshipFilters ${VTK_LIBRARIES})

ADD_EXECUTABLE( DeformableMesh DeformableMesh.cxx )
TARGET_LINK_LIBRARIES( DeformableMesh vtkKinshipFilters ${VTK_LIBRARIES})

ADD_EXECUTABLE( StraightenedCurvedPlanarReformation StraightenedCurvedPlanarReformation.cxx )
TARGET_LINK_LIBRARIES( StraightenedCurvedPlanarReformation vtkKinshipFilters ${VTK_LIBRARIES} )

ADD_EXECUTABLE( SplineDrivenImageSlicer SplineDrivenImageSlicer.cxx )
TARGET_LINK_LIBRARIES( SplineDrivenImageSlicer vtkKinshipFilters ${VTK_LIBRARIES} )

ADD_EXECUTABLE( DualPolyDataFilter DualPolyDataFilter.cxx )
TARGET_LINK_LIBRARIES( DualPolyDataFilter vtkKinshipFilters ${VTK_LIBRARIES} )

ADD_EXECUTABLE( ProbeToImageData ProbeToImageData.cxx )
TARGET_LINK_LIBRARIES( ProbeToImageData ${VTK_LIBRARIES} )

ADD_EXECUTABLE( PolyDataToBinaryImage PolyDataToBinaryImage.cxx )
TARGET_LINK_LIBRARIES( PolyDataToBinaryImage vtkKinshipFilters ${VTK_LIBRARIES} )
