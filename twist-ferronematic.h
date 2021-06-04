#ifndef TWIST_FERRONEMATIC_H
#define TWIST_FERRONEMATIC_H

#include <Eigen/LU>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const double PI_2 = 1.57079632679489661923;

class TwistFerronematic {
public:
	TwistFerronematic(const double& alpha, const double& b, const double& sigma, 
		const double& kappa, const int& nodes);

	std::string Name() const;

	std::vector<double> Zeta() const;

	std::vector<double> Phi() const;

	std::vector<double> Psi() const;

	std::vector<double> G() const;

	void Calculation(const double& h);

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

	double h_;

	Eigen::VectorXd TwistFerronematic::EquationPhi(const Eigen::VectorXd& vars);

	Eigen::VectorXd EquationPsi(const Eigen::VectorXd& vars);

	double ExpOfQ(const int& i, const Eigen::VectorXd& vars) const;

	double IntegralQ(const Eigen::VectorXd& vars);

	void EquationG(const Eigen::VectorXd& vars);


	void FillingMatrix(Eigen::MatrixXd& variables, Eigen::MatrixXd& functions);
};



#endif
