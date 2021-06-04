#ifndef READ_WRITE_H
#define READ_WRITE_H

#include <filesystem>
#include <fstream>
#include <set>

#include <nlohmann/json.hpp>

#include "twist-ferronematic.h"


void write(const TwistFerronematic& twist_feronematic);

std::vector<std::pair<std::set<double>, TwistFerronematic>> parse_json(char* argv[]);

#endif
