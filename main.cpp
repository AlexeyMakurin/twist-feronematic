#include <iostream>
#include "read_write.h"
#include "twist-ferronematic.h"




int main(int argc, char* argv[]) {

	if (argc != 2) {
		return -1;
	}
	
	for (auto& [h_set, twist_ferronematic] : parse_json(argv)) {
		for (const auto& h : h_set) {
			twist_ferronematic.Calculation(h); 
			std::cout << "h = " << h << std::endl;
			write(twist_ferronematic);
		}
		std::cout << twist_ferronematic.Name() << std::endl;
		

	}


	return 0;
}