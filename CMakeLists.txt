cmake_minimum_required(VERSION 3.11)
project(LSystem)


IF (WIN32)
set(VTK_DIR "Y:/ThirdParty/vtk811.bin" CACHE PATH "VTK Bin Dir" FORCE)
set(ITK_DIR "Y:/ThirdParty/itk413.bin" CACHE PATH "ITK Bin Dir" FORCE)
else()
set(CMAKE_CXX_STANDARD 11)
set(VTK_DIR "/home/rickf/projects/VTK.bin/" CACHE PATH "VTK Bin Dir" FORCE)
endif()


find_package(VTK COMPONENTS
        vtkCommonCore
        vtkFiltersSources
        vtkGUISupportQt
        vtkIOImage
        vtkInteractionImage
        )
include(${VTK_USE_FILE})


find_package(ITK 4.13.0 REQUIRED) 
include(${ITK_USE_FILE})


set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui 3DExtras 3DCore 3DRender OpenGL OpenGLExtensions Widgets SerialPort Network Xml XmlPatterns Sql  Svg REQUIRED)
find_package(OpenGL REQUIRED)


set(Qt5Includes
        ${Qt5Core_INCLUDES}
        ${Qt5Gui_INCLUDES}
        ${Qt5OpenGL_INCLUDES}
        ${Qt5OpenGLExtensions_INCLUDES}
        ${Qt5Widgets_INCLUDES}
        ${Qt5SerialPort_INCLUDES}
        ${Qt5Network_INCLUDES}
        ${Qt5WebEngineWidgets_INCLUDES}
        ${Qt5Xml_INCLUDES}
        ${Qt5XmlPatterns_INCLUDES}
        ${Qt5Sql_INCLUDES}
        ${Qt5Svg_INCLUDES}
        )

set(Qt5Libraries
        ${Qt5Core_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        ${Qt5OpenGL_LIBRARIES}
        ${Qt5OpenGLExtensions_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5SerialPort_LIBRARIES}
        ${Qt5Network_LIBRARIES}
        ${Qt5WebEngineWidgets_LIBRARIES}
        ${Qt5Xml_LIBRARIES}
        ${Qt5XmlPatterns_LIBRARIES}
        ${Qt5Sql_LIBRARIES}
        ${Qt5Svg_LIBRARIES}
        )
include_directories(
        ${Qt5Includes}
)

QT5_WRAP_UI(UISrcs
        ${UI_FILES}
        )

QT5_WRAP_CPP(MOCSrcs
        ${HEADERS_MOC_HPP}
        )

QT5_ADD_RESOURCES(Rscs
        ${RESC_FILES}
        )

set (SOURCE
        main.cpp
        LSystem.cpp
        LSystem.h
        Properties.h
        Production.h
		Turtle.h
		TransformInfo.h
        )



add_executable(LSystem
        ${SOURCE}
        ${HEADERS_MOC_HPP}

        ${UISrcs}
        ${MOCSrcs}
        ${Rscs}

        )

target_link_libraries(LSystem
        ${Qt5Libraries}
    ${VTK_LIBRARIES}
	${ITK_LIBRARIES}
        )