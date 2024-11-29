#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

class Solver
{
public:
    Solver(int nx, int ny, double hx, double hy, double eps);

    void setBoundaryConditions(double platePotential, double fixedPotential, int boundaryType);
    void setPlates(int x1, int x2, int y1, int y2, double potential);
    void solve();
    const std::vector<std::vector<double>> &getSolution() const;

    double computeCharge();
    double computeCapacitance();

private:
    int nx, ny;
    double hx, hy;
    double eps;
    std::vector<std::vector<double>> phi;

    void initializeGrid();
    void applyBoundaryConditions();
};

#endif // SOLVER_H
