#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <chrono>

class PackageManager
{
public:
  PackageManager(std::size_t bulkSize = 3)
    : bulkSize{ bulkSize }
  { }

  void run() {
    std::string line;
    while (getline(std::cin, line))
    {
      std::size_t foundCmd = line.find_first_of("cmd");
      if (foundCmd != std::string::npos)
      {
        if (brackets == 0)
        {
          if (packageStorage.empty())
            firstPackageOfTheBulkCameTime = std::chrono::system_clock::now();

          packageStorage.push_back(line);

          if (packageStorage.size() == bulkSize)
            outputStorage();
        }
        else
        {
          packageStorage.push_back(line);
        }
      }
      else if (line == "{")
      {
        ++brackets;

        if (brackets == 1)
        {
          outputStorage();
          firstPackageOfTheBulkCameTime = std::chrono::system_clock::now();
        }
      }
      else if (line == "}")
      {
        --brackets;

        if (brackets == 0)
          outputStorage();
      }
    }

    if(brackets == 0)
      outputStorage();
  }

  void setBulkSize(std::size_t bulkSize) {
    this->bulkSize = bulkSize;
  }

private:
  auto createPackage() {
    std::ostringstream oss;

    oss << "bulk: ";
    for (std::size_t i = 0; i < packageStorage.size(); ++i)
    {
      oss << packageStorage[i];

      if (i + 1 != packageStorage.size())
        oss << ", ";
    }

    return oss.str();
  }

  void saveToFile(std::string package) {
    std::chrono::system_clock::duration dtn = firstPackageOfTheBulkCameTime.time_since_epoch();

    auto timeStamp = dtn.count();
    std::string fileName = "bulk" + std::to_string(timeStamp) + ".log";

    std::ofstream myfile;
    myfile.open(fileName);
    myfile << package;
    myfile.close();
  }

  void outputStorage() {
    if (packageStorage.empty())
      return;

    auto package = createPackage();
    std::cout << package << '\n';

    saveToFile(package);

    packageStorage.clear();
  }

  std::size_t bulkSize;
  std::vector<std::string> packageStorage;
  std::size_t brackets{ 0 };
  std::chrono::system_clock::time_point firstPackageOfTheBulkCameTime;
};

int main(int argc, char* argv[])
{
  try
  {
    std::size_t bulkSize = 3;

    if (argc == 2)
      bulkSize = atoi(argv[1]);
    else
      throw std::invalid_argument("Invalid input");

    PackageManager manager{ bulkSize };

    manager.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
}
