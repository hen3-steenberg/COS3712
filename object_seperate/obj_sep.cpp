import read_obj;
import write_obj;

#include <filesystem>
#include <format>
#include <iostream>

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: obj_sep <filename>" << std::endl;
        return 1;
    }

    std::filesystem::path read_path = std::filesystem::absolute(argv[1]);
    auto new_objects = read_obj_file(read_path);
    for (size_t idx = 0; idx < new_objects.size(); ++idx) {
        std::filesystem::path write_path =
            read_path.parent_path() / std::format("{}_{}.obj", read_path.stem().string(), idx);
        write_file(write_path, new_objects[idx].second, new_objects[idx].first);
    }
    return 0;
}