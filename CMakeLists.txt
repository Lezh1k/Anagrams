cmake_minimum_required(VERSION 3.5)

project(anagrams VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE resources resources/*)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES 
  main.cpp 
  Mainwindow.cpp 
  Mainwindow.h 
  Mainwindow.ui
)

set(PROJECT_RESOURCES
  resource.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(anagrams
      MANUAL_FINALIZATION
      ${PROJECT_RESOURCES}
      ${PROJECT_SOURCES}
  )
else()
    message(FATAL_ERROR "required qt >= 6.0")
endif()

target_link_libraries(anagrams PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set(warning_level -Wall -Wextra -pedantic)
set(cxxflags -pipe -O0 -g)

target_compile_options(anagrams PUBLIC ${warning_level} ${cxxflags})
file(COPY ${resources} DESTINATION resources)

include(GNUInstallDirs)
install(TARGETS anagrams
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(anagrams)
endif()
