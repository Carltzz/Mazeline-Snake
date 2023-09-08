set(SNAKE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/SnakeMain.cpp"
)

include_directories("..\\Engine")
link_directories("..\\..\\lib")
add_executable(Snake ${SNAKE_SOURCES})
target_link_libraries(Snake MazelineEngine)
