#include "graphkit.h"
#include "graphkitutils.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>

namespace gkit {
Eigen::VectorXd SimpleUndiGraph::degrVec()
{
    Eigen::VectorXd degr(n);
    for (int i = 0; i < n; i++)
        degr[i] = adjs[i].size();
    return degr;
}
Eigen::SparseMatrix<double> SimpleUndiGraph::degrMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec(n);
    for (int i = 0; i < n; i++)
        tripletVec[i] = { i, i, static_cast<double>(adjs[i].size()) };
    Eigen::SparseMatrix<double> degr(n, n);
    degr.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return degr;
}
Eigen::SparseMatrix<double> SimpleUndiGraph::adjMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec;
    tripletVec.reserve(m << 1);
    for (node_t u = 0; u < n; u++)
        for (const node_t& v : adjs[u])
            tripletVec.emplace_back(u, v, 1);
    Eigen::SparseMatrix<double> adjSpMat(n, n);
    adjSpMat.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return adjSpMat;
}

Eigen::VectorXd SimpleDiGraph::degrVec()
{
    Eigen::VectorXd degr(n);
    for (int i = 0; i < n; i++)
        degr[i] = adjs[i].size();
    return degr;
}
Eigen::SparseMatrix<double> SimpleDiGraph::degrMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec(n);
    for (int i = 0; i < n; i++)
        tripletVec[i] = { i, i, static_cast<double>(adjs[i].size()) };
    Eigen::SparseMatrix<double> degr(n, n);
    degr.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return degr;
}
Eigen::SparseMatrix<double> SimpleDiGraph::adjMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec;
    tripletVec.reserve(m);
    for (node_t u = 0; u < n; u++)
        for (const node_t& v : adjs[u])
            tripletVec.emplace_back(u, v, 1);
    Eigen::SparseMatrix<double> adjSpMat(n, n);
    adjSpMat.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return adjSpMat;
}

Eigen::VectorXd SignedUndiGraph::degrVec()
{
    Eigen::VectorXd degr(n);
    for (int i = 0; i < n; i++)
        degr[i] = posAdjs[i].size() + negAdjs[i].size();
    return degr;
}
Eigen::SparseMatrix<double> SignedUndiGraph::degrMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec(n);
    for (int i = 0; i < n; i++)
        tripletVec[i] = { i, i, static_cast<double>(posAdjs[i].size() + negAdjs[i].size()) };
    Eigen::SparseMatrix<double> degr(n, n);
    degr.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return degr;
}
Eigen::SparseMatrix<double> SignedUndiGraph::adjMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec;
    tripletVec.reserve(m << 1);
    for (node_t u = 0; u < n; u++) {
        for (const node_t& v : posAdjs[u])
            tripletVec.emplace_back(u, v, 1);
        for (const node_t& v : negAdjs[u])
            tripletVec.emplace_back(u, v, -1);
    }
    Eigen::SparseMatrix<double> adjSpMat(n, n);
    adjSpMat.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return adjSpMat;
}

Eigen::VectorXd SignedDiGraph::degrVec()
{
    Eigen::VectorXd degr(n);
    for (int i = 0; i < n; i++)
        degr[i] = posAdjs[i].size() + negAdjs[i].size();
    return degr;
}
Eigen::SparseMatrix<double> SignedDiGraph::degrMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec(n);
    for (int i = 0; i < n; i++)
        tripletVec[i] = { i, i, static_cast<double>(posAdjs[i].size() + negAdjs[i].size()) };
    Eigen::SparseMatrix<double> degr(n, n);
    degr.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return degr;
}
Eigen::SparseMatrix<double> SignedDiGraph::adjMat()
{
    std::vector<Eigen::Triplet<double>> tripletVec;
    tripletVec.reserve(m);
    for (node_t u = 0; u < n; u++) {
        for (const node_t& v : posAdjs[u])
            tripletVec.emplace_back(u, v, 1);
        for (const node_t& v : negAdjs[u])
            tripletVec.emplace_back(u, v, -1);
    }
    Eigen::SparseMatrix<double> adjSpMat(n, n);
    adjSpMat.setFromTriplets(tripletVec.cbegin(), tripletVec.cend());
    return adjSpMat;
}
}