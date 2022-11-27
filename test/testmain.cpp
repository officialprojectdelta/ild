#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <sstream>

int main()
{
    std::string ext(".il");
    auto cFileList = std::filesystem::recursive_directory_iterator("tests/");
    size_t num_tests = 0;
    size_t num_successful = 0;

    // Test valid il files
    for (std::filesystem::path cFile : cFileList)
    {
        if (cFile.extension() != ext) continue;
        num_tests++;
        std::stringstream str; 
        std::string inStr = cFile.string();
        cFile.replace_extension(".S");
        std::string outStr = cFile.string();
        std::filesystem::remove(outStr);
        str << "./bin/ILD " << inStr << " " << outStr;
        std::cout << "testing: " << inStr << std::endl;
        system(str.str().c_str());
        if (std::filesystem::exists(outStr)) 
        {
            num_successful++;
            std::cout << "successful test" << std::endl; 
            // std::filesystem::remove(outStr);
        }
        else std::cout << "test failed" << std::endl;
        std::cout << std::endl;
    }
    std::cout << num_successful << " succeeded out of " << num_tests << " tests " << std::endl;

    return 0;
}