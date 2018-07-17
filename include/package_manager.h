#pragma once

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <utility>

using Package = std::pair<std::string, std::chrono::system_clock::duration>;

class Reporter {
    public:
    virtual void output(const Package& package, std::ostringstream& oss) = 0;
};

class PackageManager
{
    public:
    PackageManager(std::size_t bulkSize = 3)
        : bulkSize{ bulkSize }
    { }

    void run() {
        std::string cmd;

        while (getline(std::cin, cmd)) {
            proceedCommand(cmd);
        }

        if (brackets == 0)
            outputPackage();
    }

    void subscribe(Reporter *obs) {
        subs.push_back(obs);
    }

    private:
    void proceedCommand(std::string cmd) {
        std::size_t foundCmd = cmd.find_first_of("cmd");
        if (foundCmd != std::string::npos)
        {
            if (brackets == 0)
            {
                if (packageStorage.empty())
                    firstCommandTime = std::chrono::system_clock::now();

                packageStorage.push_back(cmd);

                if (packageStorage.size() == bulkSize)
                    outputPackage();
            }
            else
            {
                packageStorage.push_back(cmd);
            }
        }
        else if (cmd == "{")
        {
            ++brackets;

            if (brackets == 1)
            {
                outputPackage();
                firstCommandTime = std::chrono::system_clock::now();
            }
        }
        else if (cmd == "}")
        {
            --brackets;

            if (brackets == 0)
                outputPackage();
        }
    }

    std::vector<Reporter *> subs;

    void notify(Package& package) {
        for (auto s : subs) {
            auto oss = std::ostringstream{};
            s->output(package, oss);
        }
    }

    auto createPackage() {
        std::ostringstream oss;

        oss << "bulk: ";
        for (std::size_t i = 0; i < packageStorage.size(); ++i) {
            oss << packageStorage[i];

            if (i + 1 != packageStorage.size())
                oss << ", ";
        }

        return Package{ oss.str(), firstCommandTime.time_since_epoch() };
    }

    void outputPackage() {
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
