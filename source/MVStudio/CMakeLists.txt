add_subdirectory(World)

set(MV_STUDIO_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/MVStudio.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MVStudio.h"
	${MV_WORLD_SOURCES}
)

include_directories("..//Engine")
include_directories("..//..//lib")

add_executable(MVStudio ${MV_STUDIO_SOURCES})
target_link_libraries(MVStudio MazelineEngine)