//
// Created by kai on 3/29/16.
////Khuong Nguyen         101384005
//Instructor: Ivo George
//Class: CSCI 2312 002  Springs 2016
//HW#: PA3              Due Date: 03/30/2016

#include "Exceptions.h"
#include <iostream>
#include "Point.h"
#include "Cluster.h"
#include "KMeans.h"


namespace Clustering{
    // Out of Bounds
    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
        __current = c;
        __rhs = r;
        __name = "OutOfBoundsEx";
    }

    unsigned int OutOfBoundsEx::getCurrent() const {
        return __current;
    }

    std::string OutOfBoundsEx::getName() const {
         return __name;
     }

    int OutOfBoundsEx::getRhs() const {
         return __rhs;
     }
    std::ostream &operator << (std::ostream &os, const OutOfBoundsEx &ex){
        os << "The current side " << ex.__current << " is out of bounds of " << ex.__rhs << std::endl;
        return os;
    }

    // Dimensionality Exs
    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
        __current = c;
        __rhs = r;
        __name = "DimensionalityMismatchEx";
    }

    unsigned int DimensionalityMismatchEx::getRhs() const {
        return __rhs;
    }

    unsigned int DimensionalityMismatchEx::getCurrent() const {
        return __current;
    }

    std::string DimensionalityMismatchEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex){
        os << "The current size " << ex.__current << " isn't match the other side of size " <<ex.__rhs << std::endl;
        return os;
    }

    // Zero Clusters Exs

    ZeroClustersEx::ZeroClustersEx() {
        __name = "ZeroClustersEx";
    }

    std::string ZeroClustersEx::getName() const { return __name; }

    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex){
        os << ex.__name;
        return os;
    }

    // Datafile Open Exs

    DataFileOpenEx::DataFileOpenEx(std::string filename) {
        __filename = filename;
        __name = "DataFileOpenEx";
    }

    std::string DataFileOpenEx::getFilename() const {
        return __filename;
    }

    std::string DataFileOpenEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex){
        os << ex.__name << "The filename is " <<ex.__filename << " . " << std::endl;
        return os;
    }

    // Zero Dimensions Exs

    ZeroDimensionsEx::ZeroDimensionsEx() {
        __name = "ZeroDimensionsEx";
    }

    std::string ZeroDimensionsEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex){
        os << ex.getName();
        return os;
    }

    // Empty Clusters Exs

    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }

    std::string EmptyClusterEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex){
        os << ex.__name;
        return os;
    }


}
