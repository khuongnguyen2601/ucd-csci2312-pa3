//
// Created by kai on 3/29/16.
//Khuong Nguyen         101384005
//Instructor: Ivo George
//Class: CSCI 2312 002  Springs 2016
//HW#: PA3              Due Date: 03/30/2016
#include <iostream>
#include "KMeans.h"


namespace Clustering{

    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
        __dimensionality = dim;
        __k = k;
        __iFileName = filename;
        __maxIter = maxIter;
    }
    KMeans::~KMeans() {

    }

    // Accessor

    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }

    unsigned int KMeans::getNumIters() {
        return __numIter;
    }

    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }

    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }

    // element access (for testing, no bounds checking)

    Cluster &KMeans::operator[](unsigned int u) {
        return *__clusters[u];
    }

    const Cluster &KMeans::operator[](unsigned int u) const {
        return *__clusters[u];
    }

    // write out the results to a file
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans){
        os << kmeans << std::endl;
        return os;
    }
    // clustering functions
    void KMeans::run() {

    }
}
