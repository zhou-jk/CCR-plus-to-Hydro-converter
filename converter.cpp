#include "tinyxml.cpp"
#include "tinystr.cpp"
#include "tinyxmlerror.cpp" 
#include "tinyxmlparser.cpp"
#include "tinyxml.h"
#include "tinystr.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

struct subtask {
    int score;
    int time_limit;
    int memory_limit;
    std::vector<std::pair<std::string, std::string>> data;

    subtask() {}
    subtask(int score, int time_limit, int memory_limit, std::vector<std::pair<std::string, std::string>> data) :
      score(score), time_limit(time_limit), memory_limit(memory_limit), data(data) {}
};

int main() {
    std::ofstream fout("config.yaml");

    TiXmlDocument prb;
    if (!prb.LoadFile(".prb")) {
        std::cout << "Failed to load file \".prb\"." << std::endl;
        return -1;
    }

    TiXmlElement *root = prb.FirstChildElement();
    if (root == NULL || root->ValueTStr() != "problem") {
        std::cout << "No subtasks." << std::endl;
        return 1;
    }

    std::string filename;
    std::vector<subtask> sub;
    for (TiXmlElement *dd = root->FirstChildElement(); dd != NULL; dd = dd->NextSiblingElement()) {
        if (dd->ValueTStr() == "source") {
            filename = dd->Attribute("file");
            continue;
        }
        if (dd->ValueTStr() != "task") {
            continue;
        }
        for (TiXmlElement *d = dd->FirstChildElement(); d != NULL; d = d->NextSiblingElement()) {
            if (d->ValueTStr() != "subtask") {
                continue;
            }
            int score = atoi(d->Attribute("score"));
            int time_limit = 0, memory_limit = 0;
            std::vector<std::pair<std::string, std::string>> data;
            for (TiXmlElement *c = d->FirstChildElement(); c != NULL; c = c->NextSiblingElement()) {
                if (c->ValueTStr() != "point") {
                    continue;
                }
                data.emplace_back(c->Attribute("in"), c->Attribute("out"));
                time_limit = std::max(time_limit, atoi(c->Attribute("time")));
                memory_limit = std::max(memory_limit, atoi(c->Attribute("mem")));
            }
            if (data.empty()) {
                std::cout << "No test cases." << std::endl;
                return 1;
            }
            sub.emplace_back(score, time_limit, memory_limit, data);
        }
    }

    if (sub.empty()) {
        std::cout << "No subtasks." << std::endl;
        return 1;
    }

    fout << "type: default\n";
    fout << "\n";
    fout << "filename: " << filename << "\n";
    fout << "\n";
    fout << "checker_type: default\n";
    fout << "\n";
    fout << "subtasks:\n";
    for (const auto &x : sub) {
        fout << "  - score: " << x.score << "\n";
        fout << "    time: " << x.time_limit << "s\n";
        fout << "    memory: " << x.memory_limit << "m\n";
        fout << "    cases:\n";
        for (const auto [in, out] : x.data) {
            fout << "      - input: " << in << "\n";
            fout << "        output: " << out << "\n";
        }
    }
    std::cout << "Success." << std::endl;
    return 0;
}
