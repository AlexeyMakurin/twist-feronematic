
#ifndef TWIST_FERRONEMATIC_H
#define TWIST_FERRONEMATIC_H

#include <vector>
#include <string>
#include <cmath>
#include <sstream>

const double PI_2 = 1.57079632679489661923;


class TwistFerronematic {
public:
	TwistFerronematic(const double& alpha, const double& b, const double& sigma, const double& kappa, const int& nodes);

	void Calculation(const double& h);

	std::string Name() const;

	std::vector<double> Zeta() const;

	std::vector<double> Phi() const;

	std::vector<double> Psi() const;

	std::vector<double> G() const;


private:
	double alpha_;
	double b_;
	double sigma_;
	double kappa_;
	int nodes_;

	std::vector<double> zeta;
	std::vector<double> phi;
	std::vector<double> psi;
	std::vector<double> g;

	double error;
	double h_ = 0.00;

	void EquationPhi(const double& h);

	void EquationPsi(const double& h);

	double ExpOfQ(const int& i, const double& h) const;

	double IntegralQ(const double& h);

	void EquationG(const double& h);
};


#endif
