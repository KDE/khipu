include_directories (
  ${KDE4_INCLUDES}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  .
  ..
)

set ( keomath_SRCS
  function/function.cpp
  function/functionsmodel.cpp
  function/functionsview.cpp
  function/functiondelegate.cpp
  function/functioneditor.cpp
  function/functionlibraryedit.cpp
  function/functionlibrarymodel.cpp

  solvers/solver.cpp
  solvers/solverfactory.cpp

  #2d solvers
  solvers/solvers2d/cartesiancurve.cpp
  solvers/solvers2d/polarcurve.cpp
  solvers/solvers2d/implicitcurve.cpp
  solvers/solvers2d/parametriccurve2d.cpp

  #3d solvers
  solvers/solvers3d/cartesiansurface.cpp
  solvers/solvers3d/cylindricalsurface.cpp
  solvers/solvers3d/sphericalsurface.cpp
  solvers/solvers3d/parametricsurface.cpp
  solvers/solvers3d/parametriccurve3d.cpp
  solvers/solvers3d/implicitsurface.cpp

  #MarchingCubes
  solvers/solvers3d/MarchingCubes/ctab.h
  solvers/solvers3d/MarchingCubes/ctab.cpp
  solvers/solvers3d/MarchingCubes/formulas.h
  solvers/solvers3d/MarchingCubes/glwidget.h
  solvers/solvers3d/MarchingCubes/glwidget.cpp
  solvers/solvers3d/MarchingCubes/marchingcubes.cpp
  solvers/solvers3d/MarchingCubes/marchingcubes.h
  solvers/solvers3d/MarchingCubes/octree.h
  solvers/solvers3d/MarchingCubes/octree.cpp
  solvers/solvers3d/MarchingCubes/qtlogo.h
  solvers/solvers3d/MarchingCubes/qtlogo.cpp

  space/space.cpp
  space/spacesmodel.cpp
  space/spacesview.cpp

  viewers/view2d.cpp
  viewers/view3d.cpp
  viewers/viewer2d.cpp
  viewers/viewer3d.cpp

  #widgets
  widgets/button.cpp

  mathhelper.cpp
)

kde4_add_ui_files ( keomath_SRCS
  function/functioneditorwidget.ui
  viewers/viewer2dwidget.ui
  viewers/viewer3dwidget.ui
)

kde4_add_library( keomath SHARED ${keomath_SRCS} )

find_library(QGLVIEWER_LIBRARY NAMES qglviewer-qt4 QGLViewer)

target_link_libraries ( keomath
  #OpenGL
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  #${GLEW_GLEW_LIBRARY}
  #Qt
  #${QT_QTCORE_LIBRARY}
  #${QT_QTGUI_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${QT_QTXML_LIBRARY}
  #KDE
  #${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${ANALITZA_LIBRARY}
  ${ANALITZAGUI_LIBRARY}
  ${QGLVIEWER_LIBRARY}
)

set_target_properties ( keomath PROPERTIES
  VERSION ${GENERIC_LIB_VERSION}
  SOVERSION ${GENERIC_LIB_SOVERSION}
)

if(MSVC)
  set_target_properties( keomath PROPERTIES OUTPUT_NAME libkeomath )
endif(MSVC)

install ( TARGETS keomath ${INSTALL_TARGETS_DEFAULT_ARGS} )
