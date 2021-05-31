#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <nlohmann/json.hpp>

#include "twist-ferronematic.h"




void write(const TwistFerronematic& twist_feronematic) {
	
	std::filesystem::create_directory("results");
	std::filesystem::path dir("results");
	std::filesystem::path file_name(twist_feronematic.Name() + ".csv");
	std::filesystem::path path_to_file = dir / file_name;

	std::ofstream file;
	file.open(path_to_file);

	file << "zeta" << "," << "phi" << "," << "psi" << "," << "g" << "\n";
	for (size_t i = 0; i < twist_feronematic.Zeta().size(); ++i) {
		file.precision(7);
		file << twist_feronematic.Zeta()[i] << "," << twist_feronematic.Phi()[i] << "," <<
			twist_feronematic.Psi()[i] << "," << twist_feronematic.G()[i] << "\n";
	}

	file.close();
}


std::vector<std::pair<double, TwistFerronematic>> parse_json(char* argv[]) {
	std::ifstream input(argv[1]);
	nlohmann::json objJson;
	input >> objJson;

	objJson = objJson["twist_feronematics"];

	std::vector<std::pair<double, TwistFerronematic>> reader;

	for (auto& structure : objJson) {
		double h = structure["h"].get<double>();

		TwistFerronematic tw(
			structure["alpha"].get<double>(),
			structure["b"].get<double>(),
			structure["sigma"].get<double>(),
			structure["kappa"].get<double>(),
			structure["nodes"].get<int>()
		);

		reader.push_back(std::make_pair(h, tw));
	}

	return reader;
}


int main(int argc, char* argv[]) {

	if (argc != 2) {
		return -1;
	}

	for (auto& [h, twist_feronematic]: parse_json(argv)) {
		double current_h = 0.0;
		double step = 0.05;

		while (current_h <= h + 0.5 * step) {

			twist_feronematic.Calculation(current_h);

			std::cout << "h: " << current_h << std::endl;
			current_h += step;
		}

		std::cout << twist_feronematic.Name() << std::endl;
		write(twist_feronematic);
		
	}

	return 0;
}