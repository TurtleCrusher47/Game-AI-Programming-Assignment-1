set(PROJECT_NAME AI)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Source/Application.h"
    "Source/Camera.h"
    "Source/GameObject.h"
    "Source/Geometry.h"
    "Source/Light.h"
    "Source/LoadOBJ.h"
    "Source/LoadTGA.h"
    "Source/Material.h"
    "Source/Mesh.h"
    "Source/MeshBuilder.h"
    "Source/Scene.h"
    "Source/SceneBase.h"
    "Source/SceneMovement.h"
    "Source/SceneTicTacToe.h"
    "Source/shader.hpp"
    "Source/Utility.h"
    "Source/Vertex.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Source/Application.cpp"
    "Source/Camera.cpp"
    "Source/GameObject.cpp"
    "Source/LoadOBJ.cpp"
    "Source/LoadTGA.cpp"
    "Source/main.cpp"
    "Source/Mesh.cpp"
    "Source/MeshBuilder.cpp"
    "Source/SceneBase.cpp"
    "Source/SceneMovement.cpp"
    "Source/SceneTicTacToe.cpp"
    "Source/shader.cpp"
    "Source/Utility.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE AI)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Source;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../glew/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "_MBCS"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
        >
        /permissive-;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /DEBUG:FULL;
        /SUBSYSTEM:CONSOLE
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Common
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "Common;"
    "winmm;"
    "opengl32;"
    "glu32;"
    "glew32;"
    "glfw3;"
    "kernel32;"
    "user32;"
    "gdi32;"
    "winspool;"
    "comdlg32;"
    "advapi32;"
    "shell32;"
    "ole32;"
    "oleaut32;"
    "uuid;"
    "odbc32;"
    "odbccp32"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/$<CONFIG>;"
    "${CMAKE_SOURCE_DIR}//glfw/lib-vc2015;"
    "${CMAKE_SOURCE_DIR}//glew/lib"
)

