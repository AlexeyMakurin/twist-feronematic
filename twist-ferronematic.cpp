#include "twist-ferronematic.h"


double Error(const double& old_val, const double& new_val) {
	return abs(old_val - new_val) / old_val;
}

double CalculatedVar(const double& det, const Eigen::MatrixXd& matrix, 
	std::vector<double>& errors, const double& old_val) {

	double det_x = (matrix / 10).determinant();

	if (10.0e-9 * abs(det_x) >= 10.0e+9 * abs(det)) {

		std::cout << "det_x: " << det_x << std::endl;
		std::cout << "det: " << det << std::endl;

		throw std::exception("Safety check failed!");
	}
	

	double new_val = det_x / det;
	errors.push_back(abs(old_val - new_val) / old_val);

	return new_val;
}

TwistFerronematic::TwistFerronematic(const double& alpha, const double& b, 
	const double& sigma, const double& kappa, const int& nodes): alpha_(alpha), b_(b),
	sigma_(sigma), kappa_(kappa), nodes_(nodes){

	double integral_q = kappa_ * exp(sigma_ / kappa_) * (1 - exp(-alpha_ / kappa_)) / alpha_;

	double count_layer = static_cast<double>(nodes_) - 1;

	for (int layer = 0; layer < nodes_; ++layer) {
		zeta.push_back(layer / count_layer);
		phi.push_back(PI_2 * zeta[layer]);
		g.push_back(exp((sigma_ - alpha_ * layer / count_layer) / kappa_) / integral_q);
	}

	psi = phi;
}


std::string TwistFerronematic::Name() const {
	std::ostringstream out;
	out.precision(2);
	out << std::fixed << "h = " << h_ << ", " << "alpha = " << alpha_ << ", " <<
		"b = " << b_ << ", " << "sigma = " << sigma_ << ", " <<
		"kappa = " << kappa_ << ", " << "nodes = " << nodes_;
	return out.str();
}

std::vector<double> TwistFerronematic::Zeta() const {
	return zeta;
}

std::vector<double> TwistFerronematic::Phi() const {
	return phi;
}

std::vector<double> TwistFerronematic::Psi() const {
	return psi;
}

std::vector<double> TwistFerronematic::G() const {
	return g;
}

void TwistFerronematic::FillingMatrix(Eigen::MatrixXd& variables, Eigen::MatrixXd& functions) {

	for (int col = 0; col < variables.cols(); ++col) {
		for (int row = 0; row < nodes_; ++row) {

			variables(2 * row, col) = 2 * row - 2 == col and col != 2 * nodes_ - 4 ?
				phi[row] * 1.01 + 0.01 : phi[row];
			variables(2 * row + 1, col) = 2 * row - 1 == col and col != 2 * nodes_ - 4 ?
				psi[row] * 1.01 + 0.01 : psi[row];
		}

		

		EquationG(variables.col(col));

		auto equation_psi = EquationPsi(variables.col(col));
		auto equation_phi = EquationPhi(variables.col(col));

		for (int row = 0; row < equation_psi.rows(); ++row) {
			functions(2 * row + 1, col) = equation_phi(row);
			functions(2 * row + 2, col) = equation_psi(row);
		}

		

		functions(0, col) = 1;
	}

}

void TwistFerronematic::Calculation(const double& h) {
	h_ = h;
	
	if (h_ > 1.0e-5) {

		psi[0] = ((b_ * h_) / (2 * sigma_)) < 1.0 ? asin((b_ * h_) / (2 * sigma_)) : PI_2;

		for (int i = 1; i < phi.size() - 1; ++i) {
			phi[i] = PI_2;
			psi[i] = PI_2 * 0.75;
		}

		Eigen::MatrixXd variables(2 * nodes_, 2 * nodes_ - 3);
		Eigen::MatrixXd functions(2 * nodes_ - 3, 2 * nodes_ - 3);

		double error = 1;
		while (error >= 2.0e-5) {

			FillingMatrix(variables, functions);

			double det = (functions / 10).determinant();
			int scope = 2;
			while (isnan(det)) {
				std::cout << functions << std::endl;
				det = (functions * scope).determinant();
				scope += 2;
			}
			
			std::vector<double> errors;
			for (int row = 1; row < phi.size() - 1; ++row) {
				
				try {

					functions.row(0) = variables.row(2 * row);
					phi[row] = CalculatedVar(det, functions, errors, phi[row]);

					functions.row(0) = variables.row(2 * row + 1);
					psi[row] = CalculatedVar(det, functions, errors, psi[row]);

				} catch (std::exception& e) {

					FillingMatrix(variables, functions);

				}

			}

			error = *std::max_element(errors.begin(), errors.end());
			std::cout << error << std::endl;
		}
	}
}


Eigen::VectorXd TwistFerronematic::EquationPhi(const Eigen::VectorXd& vars) {

	Eigen::VectorXd equation(nodes_ - 2);
	//vars(2 * i) = phi[i]
	//vars(2 * i + 1) = psi[i]
	for (int i = 1; i < nodes_ - 1; ++i) {
		equation(i - 1) = (vars(2 * i - 2) - 2 * vars(2 * i) + vars(2 * i + 2)) / 
			pow(zeta[i] - zeta[i + 1], 2) + 0.5 * h_ * h_ * sin(2 * vars(2 * i)) - 
			sigma_ * g[i] * sin(2 * vars(2 * i) - 2 * vars(2 * i + 1));
	}

	return equation;
}


Eigen::VectorXd TwistFerronematic::EquationPsi(const Eigen::VectorXd& vars) {

	Eigen::VectorXd equation(nodes_ - 2);

	//vars[2 * i] = phi[i]
	//vars[2 * i + 1] = psi[i]
	for (int i = 1; i < nodes_ - 1; ++i) {
		equation(i - 1) = b_ * h_ * cos(vars(2 * i + 1)) + 
			sigma_ * sin(2 * vars(2 * i) - 2 * vars(2 * i + 1));
	}

	return equation; 
}


double TwistFerronematic::ExpOfQ(const int& i, const Eigen::VectorXd& vars) const {
	auto s1 = (b_ * h_ * (sin(vars(2 * i + 1)) - 1)) / kappa_;
	auto s2 = (sigma_ * pow(cos(vars(2 * i) - vars(2 * i + 1)), 2)) / kappa_;
	auto s3 = (-alpha_ * zeta[i]) / kappa_;

	return exp(s1 + s2 + s3);
}


double TwistFerronematic::IntegralQ(const Eigen::VectorXd& vars) {
	double q = 0;
	for (int layer = 0; layer < nodes_ - 1; ++layer) {
		q += ExpOfQ(layer, vars) + ExpOfQ(layer + 1, vars);
	}
	
	return q /= (2 * (nodes_ - 1));
}


void TwistFerronematic::EquationG(const Eigen::VectorXd& vars) {
	double q = IntegralQ(vars);
	for (int layer = 0; layer < nodes_; ++layer) {
		g[layer] = ExpOfQ(layer, vars) / q;
	}
}
