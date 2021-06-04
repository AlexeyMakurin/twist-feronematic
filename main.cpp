#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
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


std::vector<std::pair<std::set<double>, TwistFerronematic>> parse_json(char* argv[]) {
	std::ifstream input(argv[1]);
	nlohmann::json objJson;
	input >> objJson;

	objJson = objJson["twist_ferronematics"];

	std::vector<std::pair<std::set<double>, TwistFerronematic>> reader;
	std::set<double> h;

	for (auto& twist_ferronematic: objJson) {
		h = twist_ferronematic["h"].get<std::set<double>>();

		TwistFerronematic tw(
			twist_ferronematic["configuration"]["alpha"].get<double>(),
			twist_ferronematic["configuration"]["b"].get<double>(),
			twist_ferronematic["configuration"]["sigma"].get<double>(),
			twist_ferronematic["configuration"]["kappa"].get<double>(),
			twist_ferronematic["configuration"]["nodes"].get<int>()
		);

		reader.push_back(std::make_pair(h, tw));
	}

	return reader;
}


int main(int argc, char* argv[]) {

	if (argc != 2) {
		return -1;
	}


	for (auto& [h_set, twist_ferronematic]: parse_json(argv)) {
		double current_h = 0.00;
		
		for (auto h = h_set.begin(); h != h_set.end(); ++h) {

			double step = *h > 0.00 ? (*h - current_h) / 100 : (*h - *next(h)) / 100;
			while (step > 0.01) {
				step /= 2;
			}


			while (current_h <= *h + step / 2) {
				twist_ferronematic.Calculation(current_h);
				std::cout << "h: " << current_h << std::endl;
				/*if (current_h >= 5.65700) {
					std::cout << std::endl;
				}*/
				current_h += step;
			}
			write(twist_ferronematic);
		}
		std::cout << twist_ferronematic.Name() << std::endl;
	}

	return 0;
}