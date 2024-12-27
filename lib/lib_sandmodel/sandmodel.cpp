#include "sandmodel.h"

SandModel::SandModel(const char* input, char* output_path, uint64_t max_iter, uint64_t freq) {
    input_file_path = input;
    output_bmp_path = output_path;
    if (!std::filesystem::exists(output_bmp_path)) {
        RiseErrorPathIsNotExist();
    }
    output_bmp_path = ReformatPath(output_bmp_path);
    max_iterations = max_iter;
    frequency = freq;
    ReadFromTsv();
    ScatterSand();
}

SandModel::~SandModel() {
    DeleteGrid();
    DeleteQueue();
}

void InitialiseGrid(uint64_t**& grid, uint16_t width, uint16_t height) {
    grid = new uint64_t*[height]{};
    for (uint16_t i = 0; i < height; ++i) {
        grid[i] = new uint64_t[width]{};
    }
}

void SandModel::ReadFromTsv() {
    std::ifstream input_file(input_file_path);
    if (!input_file.is_open()) {
        RiseErrorFileIsNotOpen();
    }
    int16_t x;
    int16_t y;
    int16_t shift_x;
    int16_t shift_y;
    uint64_t grains_quantity;
    int16_t min_x = std::numeric_limits<int16_t>::max();
    int16_t max_x = std::numeric_limits<int16_t>::min();
    int16_t min_y = std::numeric_limits<int16_t>::max();
    int16_t max_y = std::numeric_limits<int16_t>::min();
    while (input_file >> x >> y >> grains_quantity) {
        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
    }
    input_file.clear();
    input_file.seekg(0);
    width = max_x - min_x + 1;
    height = max_y - min_y + 1;
    shift_x = kZero - min_x;
    shift_y = kZero - min_y;
    InitialiseGrid(grid, width, height);
    queue_for_scatter = new uint64_t[kQueueCapacity]{0};
    while (input_file >> x >> y >> grains_quantity) {
        grid[y + shift_y][x + shift_x] = grains_quantity;
    }
    input_file.close();
}

void SandModel::DeleteQueue() {
    delete [] queue_for_scatter;
    queue_for_scatter = nullptr;
}

void SandModel::DeleteGrid() {
    for (int y = 0; y < height - over_height; ++y) {
        delete [] grid[y];
    }
    delete [] grid;
}

void SandModel::ResizeQueueForward() {
    if (queue_size == kQueueCapacity) {
        kQueueCapacity *= 2;
        uint64_t* new_queue = new uint64_t[kQueueCapacity];
        for (size_t i = 0; i < queue_size; ++i) {
            new_queue[i] = queue_for_scatter[i];
        }
        DeleteQueue();
        queue_for_scatter = new_queue;
        new_queue = nullptr;
    }
}

void SandModel::ChangeScatterFlags(uint64_t y, uint64_t x) {
    if (y == 0) {
        scatter_up = true;
    } 
    if (y == (height - 1)) {
        scatter_down = true;
    }
    if (x == 0) {
        scatter_left = true;
    } 
    if (x == (width - 1)) {
        scatter_right = true;
    }
}

void SandModel::QueueFilling() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] > kNormalQuantityOfSands) {
                ChangeScatterFlags(y, x);
                ResizeQueueForward();
                queue_for_scatter[queue_size] = y;
                queue_for_scatter[++queue_size] = x;
                ++queue_size;
            }
        }
    }
}

void SandModel::ResizeUp(uint64_t**& new_grid) {
    ++shift_resized_y;
    ++over_height;
    ++height;
    InitialiseGrid(new_grid, width, height);
}

void SandModel::ResizeDown(uint64_t**& new_grid) {
    ++height;
    ++over_height;
    InitialiseGrid(new_grid, width, height);
}

void SandModel::ResizeLeft(uint64_t**& new_grid) {
    ++shift_resized_x;
    ++over_width;
    ++width;
    InitialiseGrid(new_grid, width, height);
}

void SandModel::ResizeRight(uint64_t**& new_grid) {
    ++over_width;
    ++width;
    InitialiseGrid(new_grid, width, height);
}

void SandModel::CopyGrid(uint64_t**& new_grid) {
    for (int y = 0; y < height - over_height; ++y) {
        for (int x = 0; x < width - over_width; ++x) {
            new_grid[y + shift_resized_y][x + shift_resized_x] = grid[y][x];
        }
    }
}

void SandModel::ResizeGridAndCopy() {
    uint64_t** new_grid;
    if (scatter_up) {
        scatter_up = false;
        ResizeUp(new_grid);
    }
    if (scatter_down) {
        scatter_down = false;
        ResizeDown(new_grid);
    }
    if (scatter_left) {
        scatter_left = false;
        ResizeLeft(new_grid);
    }
    if (scatter_right) {
        scatter_right = false;
        ResizeRight(new_grid);
    }
    if (!(scatter_down || scatter_up || scatter_left || scatter_right)) {
        InitialiseGrid(new_grid, width, height);
    }
    CopyGrid(new_grid);
    DeleteGrid();
    grid = new_grid;
    new_grid = nullptr;
}

void SandModel::Scattering() {
    for (size_t i = 0; i < queue_size; ++i) {
        uint64_t y, x, grains;
        y = queue_for_scatter[i];
        x = queue_for_scatter[++i];
        grid[y + shift_resized_y][x + shift_resized_x] -= 4;
        grid[y + shift_resized_y - 1][x + shift_resized_x] += 1;
        grid[y + shift_resized_y + 1][x + shift_resized_x] += 1;
        grid[y + shift_resized_y][x + shift_resized_x + 1] += 1;
        grid[y + shift_resized_y][x + shift_resized_x - 1] += 1;
    }
    queue_size = 0;
    over_width = 0;
    over_height = 0;
    shift_resized_x = 0;
    shift_resized_y = 0;
}

void SandModel::ScatterSand() {
    if (frequency != 0) {
        CreateImageOfSandModel(0);
    }
    for (size_t i = 1; i <= max_iterations; ++i) {
        QueueFilling();
        if (queue_size == 0) {
            break;
        }
        ResizeGridAndCopy();
        Scattering();
        if (frequency != 0 && i % frequency == 0) {
            CreateImageOfSandModel(i);
        }
    }
    if (frequency == 0) {
        CreateImageOfSandModel(-1);
    }
}
void NumToString(char*& str, size_t iter) {
    int i = 0;
    while (iter > 0) {
        str[i] = (iter % 10) + '0';
        iter /= 10;
        ++i;
        char new_str[i + 1];
        for (int j = 0; j < i; ++j) {
            new_str[j] = str[j];
        }
        delete [] str;
        str = new_str;
    }
    for (int j = 0; j < i / 2; ++j) {
        std::swap(str[j], str[i - 1 - j]);
    }
    str[i] = '\0';
}
void LargeStringAndMerge(char*& str_1, const char* str_2) {
    char* new_str = new char[strlen(str_1) + strlen(str_2)]{};
    strcpy(new_str, str_1);
    strcat(new_str, str_2);
    delete [] str_1;
    str_1 = new_str;
    new_str = nullptr;
}

void SandModel::CreateImageOfSandModel(size_t iter) {
    char* output_file = new char[strlen(output_bmp_path)]{};
    strcpy(output_file, output_bmp_path);
    LargeStringAndMerge(output_file, "sand_pile");
    if (iter != -1) {
        LargeStringAndMerge(output_file, "_");
        if (iter > 0) {
            char* str_num = new char[1]{};
            NumToString(str_num, iter);
            LargeStringAndMerge(output_file, str_num);
            delete [] str_num;
            str_num = nullptr;
        } else {
            LargeStringAndMerge(output_file, "0");
        }    
    }
    LargeStringAndMerge(output_file, ".bmp");
    Image image(width, height);
    image.Export(output_file, grid);
    delete [] output_file;
    output_file = nullptr;
}