add_library(sandmodel sandmodel.cpp sandmodel.h)
target_include_directories(sandmodel PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(sandmodel PRIVATE errors help_functions bmp)
