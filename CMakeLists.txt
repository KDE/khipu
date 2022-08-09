cmake_minimum_required(VERSION 3.5)

project(khipu)

set(QT_MIN_VERSION "5.12.0")
set(KF_MIN_VERSION "5.69.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)
include(ECMAddAppIcon)

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Test PrintSupport Xml)

find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS I18n ItemModels ItemViews GuiAddons WidgetsAddons NewStuff XmlGui KDELibs4Support DocTools)
find_package(Analitza5 REQUIRED)

#TODO: enable after adding to store.kde.org
#install(FILES khipu.knsrc DESTINATION  ${KDE_INSTALL_CONFDIR})


add_definitions(-DQT_NO_URL_CAST_FROM_STRING
                -DQT_NO_CAST_FROM_BYTEARRAY
                -DQT_USE_QSTRINGBUILDER
                -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
               )

add_subdirectory(src) #app ... use the library
add_subdirectory(misc) #icons, pics, function library, etc
add_subdirectory(tests)
add_subdirectory(doc)
