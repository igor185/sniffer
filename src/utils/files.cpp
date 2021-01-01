// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <fstream>
#include <vector>

#include "utils/utils.h"

std::vector<std::string> utils::get_recent_files(){
    std::ifstream file(FILE_RECENT_LOGS);
    if(!file.is_open()){
        return {};
    }

    std::vector<std::string> res;
    std::string line;

    while (std::getline(file, line))
        res.push_back(std::move(line));

    file.close();
    return res;
}

void utils::set_recent_file(const std::string& file_name){
    for(auto&& file_name_saved: get_recent_files())
        if(file_name_saved == file_name)
            return;

    std::ofstream file(FILE_RECENT_LOGS, std::ifstream::app);

    file << file_name << "\n";

    file.close();
}