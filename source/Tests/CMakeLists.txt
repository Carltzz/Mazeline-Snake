set(TEST_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

include_directories("../Engine")
link_directories("../../lib")

add_executable(EngineTest ${TEST_SOURCES})
target_link_libraries(EngineTest MazelineEngine)