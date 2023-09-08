# Root/source/Engine

add_subdirectory("Core")
add_subdirectory("Events")
add_subdirectory("Graphics")
add_subdirectory("Gui")
add_subdirectory("Input")
add_subdirectory("IO")
add_subdirectory("IO/File")
add_subdirectory("Math")
add_subdirectory("Physics")
add_subdirectory("Util")

set(ENGINE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Camera.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/DisplayManager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/DisplayManager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/EngineSubsystem.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/MazelineEngine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MazelineEngine.h"
	${CORE_SOURCES}
	${EVENTS_SOURCES}
	${GRAPHICS_SOURCES}
	${GUI_SOURCES}
	${GLEW_SOURCES}
	${INPUT_SOURCES}
	${IO_SOURCES}
	${IO_FILE_SOURCES}
	${LIBPNG_SOURCES}
	${MATH_SOURCES}
	${PHYSICS_SOURCES}
	${UTIL_SOURCES}
	${ZLIB_SOURCES}
)

include_directories("/")
include_directories("../../extern")

add_library(MazelineEngine STATIC ${ENGINE_SOURCES})
target_link_libraries(MazelineEngine opengl32)