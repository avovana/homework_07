#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <utility>
#include <tuple>

using Package = std::pair<std::string, std::chrono::system_clock::duration>;

class Reporter {
public:
  virtual void output(const Package& package) = 0;
};

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
            firstCommandTime = std::chrono::system_clock::now();

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
          firstCommandTime = std::chrono::system_clock::now();
        }
      }
      else if (line == "}")
      {
        --brackets;

        if (brackets == 0)
          outputStorage();
      }
    }

    if (brackets == 0)
      outputStorage();
  }

  void subscribe(Reporter *obs) {
    subs.push_back(obs);
  }

private:
  std::vector<Reporter *> subs;

  void notify(Package& package) {
    for (auto s : subs)
      s->output(package);
  }

  auto createPackage() {
    std::ostringstream oss;

    oss << "bulk: ";
    for (std::size_t i = 0; i < packageStorage.size(); ++i)
    {
      oss << packageStorage[i];

      if (i + 1 != packageStorage.size())
        oss << ", ";
    }

    return Package{ oss.str(), firstCommandTime.time_since_epoch() };
  }

  void outputStorage() {
    if (packageStorage.empty())
      return;

    auto package = createPackage();
    notify(package);

    packageStorage.clear();
  }

  std::size_t bulkSize{};
  std::vector<std::string> packageStorage;
  std::size_t brackets{};
  std::chrono::system_clock::time_point firstCommandTime;
};

class ConsoleHandler : Reporter {
public:
  ConsoleHandler(PackageManager* manager) {
    if (manager)
      manager->subscribe(this);
  }

  void output(const Package& package) override {
    std::cout << package.first << '\n';
  }
};

class FileHandler : Reporter {
public:
  FileHandler(PackageManager* manager) {
    if (manager)
      manager->subscribe(this);
  }

  void output(const Package& package) override {
    std::chrono::system_clock::duration firstCommandTime;
    std::string pack;

    std::tie(pack, firstCommandTime) = package;

    std::string fileName = "bulk" + std::to_string(firstCommandTime.count()) + ".log";

    std::ofstream myfile;
    myfile.open(fileName);
    myfile << pack;
    myfile.close();
  }
};

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
