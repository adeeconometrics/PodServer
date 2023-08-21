#ifndef __UTILS_H__
#define __UTILS_H__

#include <filesystem>
#include <stdexcept>
#include <string_view>

// file handler

// data object
struct FileObject {
public: 
    FileObject(std::filesystem::path t_base_path, std::string_view t_name): file_name(t_name) {
        if(std::filesystem::exists(t_base_path)) {
            throw std::runtime_error("file path does not exist: " + static_cast<std::string>(t_base_path));
        }
        file_path = t_base_path;
    }

    auto get_file_type() const -> const std::string_view {
        const std::size_t pos = file_name.find_last_not_of('.');

        return file_name.substr(pos+1);
    }

public:
    std::filesystem::path file_path;
    std::string_view file_name;
};

#endif // __UTILS_H__