#include "read_write.h"


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

	for (auto& twist_ferronematic : objJson) {
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