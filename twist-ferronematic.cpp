#include "twist-ferronematic.h"

TwistFerronematic::TwistFerronematic(const double& alpha, const double& b, const double& sigma, const double& kappa, const int& nodes) :
	alpha_(alpha), b_(b), sigma_(sigma), kappa_(kappa), nodes_(nodes) {

	// Integral Q at h = 0;
	double integral_q = kappa_ * exp(sigma_ / kappa_) * (1 - exp(-alpha_ / kappa_)) / alpha_;

	double count_layer = static_cast<double>(nodes_) - 1;

	for (int layer = 0; layer < nodes_; ++layer) {
		zeta.push_back(layer / count_layer);
		phi.push_back(pi_2 * zeta[layer]);
		g.push_back(exp((sigma_ - alpha_ * layer / count_layer) / kappa_) / integral_q);
	}

	psi = phi;
}

void TwistFerronematic:: Calculation(const double& h) {
	h_ = h;

	do {
		EquationPhi(h);
		EquationPsi(h);
		EquationG(h);

	} while (error >= 2.0e-5);
	
}

std::string TwistFerronematic::Name() const {
	std::ostringstream out;
	out.precision(2);
	out << std::fixed << "h = " << h_ << ", " << "alpha = " << alpha_ << ", " << 
		"b = " << b_ << ", " << "sigma = " << sigma_ << ", " <<
		"kappa = " << kappa_ << ", " << "notes = " << nodes_;
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


void TwistFerronematic::EquationPhi(const double& h) {
	std::vector<double> phi_(nodes_);
	phi_[0] = 0;
	phi_[nodes_ - 1] = pi_2;

	error = 0;

	for (int layer = 1; layer < nodes_ - 1; ++layer) {
		phi_[layer] = 0.5 * (phi[layer + 1] + phi[layer - 1] + pow(zeta[layer] - zeta[layer + 1], 2) *
			(h * h * 0.5 * sin(2 * phi[layer]) + sigma_ * g[layer] * sin(2 * phi[layer] - 2 * psi[layer])));

		if (abs(phi_[layer] - phi[layer]) / phi_[layer] > error) {
			error = abs(phi_[layer] - phi[layer]) / phi_[layer];
		}
	}

	phi = phi_;
}

void TwistFerronematic::EquationPsi(const double& h) {
	for (int layer = 0; layer < nodes_; ++layer) {
		double error_psi = 1;
		while (error_psi >= 2.0e-5) {

			double k1 = b_ * h * cos(psi[layer]) + sigma_ * sin(2 * phi[layer] - 2 * psi[layer]);
			double k2 = -b_ * h * sin(psi[layer]) - 2 * sigma_ * cos(2 * phi[layer] - 2 * psi[layer]);

			double psi_ = psi[layer] - k1 / k2;

			error_psi = abs(psi_ - psi[layer]) / psi_;
			psi[layer] = psi_;
		}
	}
}

double TwistFerronematic::ExpOfQ(const int& i, const double& h) const {
	return exp((b_ * h * sin(psi[i]) + sigma_ * cos(phi[i] - psi[i]) *
		cos(phi[i] - psi[i]) - alpha_ * zeta[i]) / kappa_);
}

double TwistFerronematic::IntegralQ(const double& h) {
	double q = 0;
	for (int layer = 0; layer < nodes_ - 1; ++layer) {
		q += ExpOfQ(layer, h) + ExpOfQ(layer + 1, h);
	}

	return q /= (2 * (nodes_ - 1));
}

void TwistFerronematic::EquationG(const double& h) {
	double q = IntegralQ(h);
	for (int layer = 0; layer < nodes_; ++layer) {
		g[layer] = ExpOfQ(layer, h) / q;
	}

}
