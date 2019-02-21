ki18n_wrap_ui(libkhipu_SRCS
  dictionarycollection.ui
  plotsbuilder.ui
  plotseditor.ui
  spaceinformation.ui
  spaceoptions.ui
  filter.ui
  dashboard.ui
)

add_library(libkhipu STATIC ${libkhipu_SRCS}
  spaceitem.cpp
  spacesmodel.cpp
  dictionarycollection.cpp
  plotsbuilder.cpp
  plotseditor.cpp
  spaceinformation.cpp
  spaceoptions.cpp
  filter.cpp
  spaceplotsproxymodel.cpp
  spacesdelegate.cpp
  datastore.cpp
  dashboard.cpp
  mainwindow.cpp)

add_executable(khipu main.cpp)

target_link_libraries(khipu libkhipu)

target_link_libraries(libkhipu
PUBLIC
  Qt5::Widgets
  KF5::I18n
  KF5::ItemViews
  KF5::ItemModels
  KF5::GuiAddons
  KF5::WidgetsAddons
  KF5::XmlGui
  KF5::NewStuff
  KF5::KDELibs4Support
  KF5::AnalitzaGui
  KF5::AnalitzaWidgets
  KF5::AnalitzaPlot
)

install (TARGETS khipu ${INSTALL_TARGETS_DEFAULT_ARGS})
