#pragma once

#include <iostream>
#include <tuple>
#include <string>
#include <chrono>
#include <fstream>

using Package = std::pair<std::string, std::chrono::system_clock::duration>;

class PackageManager;
class Reporter;

class ConsoleHandler : Reporter {
    public:
    ConsoleHandler(PackageManager* manager) {
        if(manager)
            manager->subscribe(this);
    }

    void output(const Package& package) override {
        std::string pack;
        
        std::tie(pack, std::ignore) = package;
        std::cout << pack << '\n';
    }
};

class FileHandler : Reporter {
    public:
    FileHandler(PackageManager* manager) {
        if(manager)
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