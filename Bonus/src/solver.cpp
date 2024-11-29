#include "solver.h"
#include <cmath>
#include <stdexcept>

Solver::Solver(int nx, int ny, double hx, double hy, double eps)
    : nx(nx), ny(ny), hx(hx), hy(hy), eps(eps), phi(nx, std::vector<double>(ny, 0.0))
{
    if (nx <= 0 || ny <= 0 || hx <= 0 || hy <= 0 || eps <= 0)
        throw std::invalid_argument("Некорректные параметры сетки или материала");
    initializeGrid();
}

void Solver::initializeGrid()
{
    for (auto &row : phi)
        std::fill(row.begin(), row.end(), 0.0);
}

void Solver::setBoundaryConditions(double platePotential, double fixedPotential, int boundaryType)
{
    if (boundaryType == 1) // Dirichlet
    {
        for (int i = 0; i < nx; ++i)
        {
            phi[i][0] = fixedPotential;
            phi[i][ny - 1] = fixedPotential;
        }
        for (int j = 0; j < ny; ++j)
        {
            phi[0][j] = fixedPotential;
            phi[nx - 1][j] = fixedPotential;
        }
    }
}

void Solver::setPlates(int x1, int x2, int y1, int y2, double potential)
{
    for (int i = x1; i <= x2; ++i)
        for (int j = y1; j <= y2; ++j)
            phi[i][j] = potential;
}

void Solver::solve()
{
    std::vector<std::vector<double>> newPhi = phi;

    double tol = 1e-6;
    bool converged = false;

    while (!converged)
    {
        converged = true;

        for (int i = 1; i < nx - 1; ++i)
        {
            for (int j = 1; j < ny - 1; ++j)
            {
                newPhi[i][j] = 0.25 * (phi[i + 1][j] + phi[i - 1][j] + phi[i][j + 1] + phi[i][j - 1]);

                if (std::abs(newPhi[i][j] - phi[i][j]) > tol)
                    converged = false;
            }
        }

        phi.swap(newPhi);
    }
}

const std::vector<std::vector<double>> &Solver::getSolution() const
{
    return phi;
}

double Solver::computeCharge()
{
    double charge = 0.0;
    for (int i = 0; i < nx - 1; ++i)
        charge += eps * (phi[i + 1][0] - phi[i][0]) / hx;
    return std::abs(charge);
}

double Solver::computeCapacitance()
{
    double charge = computeCharge();
    return charge / phi[nx / 2][ny - 1];
}
