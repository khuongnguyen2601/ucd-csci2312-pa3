//
// Created by kai on 3/29/16.
//Khuong Nguyen         101384005
//Instructor: Ivo George
//Class: CSCI 2312 002  Springs 2016
//HW#: PA3              Due Date: 03/30/2016

#include "Point.h"
#include "Exceptions.h"
#include <cmath>
#include <cassert>
#include <string>
#include <sstream>

namespace Clustering{

    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';
    // Static ID Generator
    static void Point::rewindIdGen() {
        __idGen--;
    }

    unsigned int Point::Point(unsigned int dim) {
        if(dim == 0)
            throw ZeroDimensionsEx();

        __id = __idGen;
        ++__idGen;

        __dim = dim;
        __values = new double [__dim];

        for (unsigned int i = 0; i < __dim; i++){
            setValue(1,0);
        }
    }

    unsigned int Point::Point(unsigned int dim, double *value) {
        if (dim == 0)
            throw ZeroDimensionsEx();
        __id = __idGen;
        ++__idGen;

        __dim = dim;
        __values = new double [__dim];

        for (unsigned int i = 0; i<__dim; i++){
            __values[i] = value[i];
        }
    }

    // Big three: cpy ctor, overloaded operator=, dtor

    Point::Point(const Point &dim) {
        __dim = dim.__dim;
        __values = new double [__dim];

        for (int i = 0; i <__dim; i++){
            __values[i]= dim.__values[i];
            __id = dim.__id;
        }

    }

    Point &Point::operator=(const Point &dim) {
        __dim = dim.__dim;
        __id = dim.__id;

        if (__dim > dim.__dim)
            throw DimensionalityMismatchEx(__dim,dim.__dim);

        if(this != &dim){
            this ->__dim = dim.__dim;
            this ->__id = dim.__id;
            this ->__values = new double [__dim];

            for (unsigned int i=0; i< __dim; i++){
                this ->__values[i] = dim.__values[i];
            }
        }
        else return *this;

        return *this;
    }

    Point::~Point() {
        delete[] __values;
    }

    // Accessors & mutators
    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }

    void Point::setValue(unsigned int dim, double value) {

        __values[dim] = value;
    }

    double Point::getValue(unsigned int dim) const {
        return __values[dim];
    }

    // Functions
    // Calculate distance between two points | equation (x2 - x1)^2
    double Point::distanceTo(const Point &dis) const {
        if (__dim != dis.__dim)
            throw DimensionalityMismatchEx(__dim, dis.__dim);

        double distance = 0;
        int dim;
        dim = dis.__dim;

        for (int i=0; i< __dim; i++){
            if(dis.__values[i] == __values[i]){
                distance +=0;
            }
            else
            {
                double square = std::pow (dis.__values[i] - __values[i], 2);
                distance += square;
            }
        }
        distance = std::sqrt(distance);
        return distance;
    }

    // Overloaded operators

    // Members

    Point &Point::operator*=(double dim) // p *= 6; p.operator*=(6)
    {
        int value= __dim;

        for (int i=0; i< __dim; i++){
            this ->__values[i] *= dim;
        }
        return *this;
    }

    Point &Point::operator/=(double dim) {
        int value = __dim;

        for (int i=0; i < __dim; i++){
            this -> __values[i] /= dim;
        }
        return *this;
    }

    const Point Point::operator*(double dim) const // prevent (p1 * 2) = p2;
    {
        unsigned int value = __dim;
        Point p(value);

        for (int i = 0; i< __dim; i++){
            p.__values[i] = __values[i] * dim;
        }
        return p;

    }

    const Point Point::operator/(double dim) const // p3 = p2/2;
    {
        unsigned int value = __dim;
        Point p(value);

        for (int i = 0; i < __dim; i++){
            p.__values[i] = __values[i] / dim;
        }
        return p;
    }

    double &Point::operator[](unsigned int index) {
        if(index >= __dim)
            throw OutOfBoundsEx(index, __dim);
        return __values[index];
    }

    const double &Point::operator[](unsigned int index) const {
        if (index >= __dim)
            throw OutOfBoundsEx(index, __dim);
        return __values[index];
    }

    // friends
    Point &operator+=(Point &rhs, const Point &lhs){
        if(rhs.__dim != lhs.__dim)
            throw DimensionalityMismatchEx(rhs.__dim,lhs.__dim);
        int value = rhs.__dim;
        for (int i = 0; i < value; i++){
            rhs.__values[i] += lhs.__values[i];
        }
        return rhs;
    }

    Point &operator-=(Point &rhs, const Point &lhs){
        if(rhs.__dim != lhs.__dim)
            throw DimensionalityMismatchEx(rhs.__dim,lhs.__dim);
        int value = rhs.__dim;
        for (int i=0; i < value; i++){
            rhs.__values[i] -= lhs.__values[i];
        }
        return rhs;
    }

    const Point operator+(const Point &rhs, const Point &lhs){
        if (rhs.__dim != lhs.__dim)
            throw DimensionalityMismatchEx(rhs.__dim,lhs.__dim);
        Point p(rhs);
        p += lhs;
        return p;
    }

    const Point operator-(const Point &rhs, const Point &lhs){
        if (rhs.__dim != lhs.__dim)
            throw DimensionalityMismatchEx(rhs.__dim,lhs.__dim);
        Point p(rhs);
        p -= lhs;
        return p;
    }

    bool operator==(const Point &rhs, const Point &lhs){
        bool results = true;
        if(rhs.__id != lhs.__id)
            results = false;
        else
        {
            for (int i=0; i < rhs.__dim;i++){
                if(rhs.__values[i] != lhs.__values[i])
                results = false;
            }
        }
        return results;
    }
    bool operator!=(const Point &rhs, const Point &lhs){
        return (!(rhs == lhs));
    }

    bool operator<(const Point &rhs, const Point &lhs){
        bool results = false;
        unsigned int value = rhs.__dim;
        if (value < lhs.__dim || value > lhs.__dim)
            throw DimensionalityMismatchEx(value,lhs.__dim);
        for (int i =0; i<value; i++){
            if (rhs.__values[i] < lhs.__values[i]){
                results = true;
            }
            if (rhs.__values[i] > lhs.__values[i]){
                results = false;
                return results;
            }
        }
        return results;

    }
    bool operator>(const Point &rhs, const Point &lhs){
        return (lhs < rhs);
    }
    bool operator<=(const Point &rhs, const Point &lhs){
        return !(rhs > lhs);
    }
    bool operator>=(const Point &rhs, const Point &lhs){
        return !(rhs < lhs);
    }

    std::ostream &operator<<(std::ostream &os, const Point &p){
        int i;
        for (i = 0; i< p.__dim - 1; i++)
            os << p.__values[i] << p.POINT_VALUE_DELIM;

        os << p.__values[i] << std::endl;

        return os;
    }
    std::istream &operator>>(std::istream &is, Point &p){
        double dim;
        std::string value;

        for (int i=0; i<p.__dim; i++){
            getline(is, value, ',');
            std::stringstream input(value);
            input >> dim;
            p.__values[i] = dim;
        }
        return is;
    }


}