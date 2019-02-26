project(khipu)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.7.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

find_package(Qt5 5.2 REQUIRED NO_MODULE COMPONENTS Widgets Test PrintSupport Xml)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)
include(ECMAddAppIcon)

find_package(KF5 REQUIRED COMPONENTS I18n ItemModels ItemViews GuiAddons WidgetsAddons NewStuff XmlGui KDELibs4Support DocTools)
find_package(Analitza5 REQUIRED)

#TODO: enable after adding to store.kde.org
#install(FILES khipu.knsrc DESTINATION  ${CONFIG_INSTALL_DIR})


add_definitions(-DQT_NO_URL_CAST_FROM_STRING
                -DQT_NO_CAST_FROM_BYTEARRAY
                -DQT_USE_QSTRINGBUILDER
                -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
               )

add_subdirectory(src) #app ... use the library
add_subdirectory(misc) #icons, pics, function library, etc
add_subdirectory(tests)
add_subdirectory(doc)
