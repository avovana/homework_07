#include <iostream>

#include "package_manager.h"
#include "reporter.h"

int main(int argc, char* argv[])
{
    try
    {
        std::size_t bulkSize = 2;

        if (argc == 2)
            bulkSize = atoi(argv[1]);
        else
            throw std::invalid_argument("Invalid input");

        PackageManager manager{ bulkSize };

        ConsoleHandler ch(&manager);
        FileHandler fh(&manager);

        manager.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
