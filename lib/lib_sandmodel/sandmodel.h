#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include "lib/lib_errors/errors.h"
#include "lib/lib_help_functions/help_functions.h"
#include "lib/lib_bmp/bmp.h"


#pragma pack(push, 1)
struct SandModel {
    const char* input_file_path;
    char* output_bmp_path;
    uint64_t max_iterations;
    uint64_t frequency;
    uint16_t width = 0;
    int8_t over_width = 0;
    int8_t over_height = 0;
    uint16_t height = 0;
    int8_t shift_resized_x = 0;
    int8_t shift_resized_y = 0;
    uint64_t** grid;
    uint64_t kQueueCapacity = 100000;
    uint64_t* queue_for_scatter;
    size_t queue_size = 0;
    bool scatter_left = false;
    bool scatter_right = false;
    bool scatter_down = false;
    bool scatter_up = false;
    const int16_t kZero = 0;
    const uint8_t kNormalQuantityOfSands = 3;
    SandModel(const char* input, char* output_path, uint64_t max_iter, uint64_t freq);
    ~SandModel();
    void ReadFromTsv();
    void ScatterSand();
    void QueueFilling();
    void ResizeQueueForward();
    void ChangeScatterFlags(uint64_t y, uint64_t x);
    void ResizeGridAndCopy();
    void ResizeUp(uint64_t**& new_grid);
    void ResizeDown(uint64_t**& new_gird);
    void ResizeLeft(uint64_t**& new_grid);
    void ResizeRight(uint64_t**& new_grid);
    void CopyGrid(uint64_t**& new_grid);
    void CreateImageOfSandModel(size_t iter);
    void DeleteQueue();
    void DeleteGrid();
    void Scattering();
};
#pragma pack(pop)
