include_directories (
  ${CMAKE_SOURCE_DIR} 
  ${CMAKE_BINARY_DIR} 
  ${KDE4_INCLUDES} 
  ${LIBKDEEDU_INCLUDE_DIR}/qtmml 
  ${ANALITZA_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${qjson_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  .
  ..
)

set ( khipu_SRCS
  dictionaryitem.cpp
  dictionariesmodel.cpp

  dictionarycollection.cpp
  plotsbuilder.cpp
  plotseditor.cpp
  functionlibraryedit.cpp
  functionlibrarymodel.cpp
  spaceinformation.cpp
  spaceoptions.cpp
  filter.cpp

  spaceplotsproxymodel.cpp
  
  spacesdelegate.cpp

  datastore.cpp
  document.cpp

  dashboard.cpp
  mainwindow.cpp
  main.cpp

)

kde4_add_ui_files ( khipu_SRCS
  dictionarycollection.ui
  plotsbuilder.ui
  plotseditor.ui
  spaceinformation.ui
  spaceoptions.ui
#   spaceeditor.ui
  filter.ui
  dashboard.ui
)



kde4_add_executable ( khipu ${khipu_SRCS} )

target_link_libraries( khipu
  ${KDE4_KFILE_LIBS}
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY} ${ANALITZAPLOT_LIBRARY}
  ${qjson_LIBRARIES}
    #OpenGL
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QTMML_LIBRARY}
)

install (TARGETS khipu ${INSTALL_TARGETS_DEFAULT_ARGS} )
