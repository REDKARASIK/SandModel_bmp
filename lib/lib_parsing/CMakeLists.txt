add_library(parsing parsing.cpp parsing.h)
target_include_directories(parsing PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(parsing PRIVATE errors help_functions)