add_library(help_functions help_functions.cpp help_functions.h)
target_include_directories(help_functions PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(help_functions PRIVATE errors)