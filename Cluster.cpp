//
// Created by kai on 3/29/16.
////Khuong Nguyen         101384005
//Instructor: Ivo George
//Class: CSCI 2312 002  Springs 2016
//HW#: PA3              Due Date: 03/30/2016
#include <iostream>
#include <string>
#include <cassert>
#include "Cluster.h"
#include "Exceptions.h"
#include <sstream>


namespace Clustering{
    unsigned int Cluster::__idGenerator = 0;

    void Cluster::__del() {
        if(__size != 0){
            LNodePtr cursor = __points;
            delete cursor;
        }
        __points = NULL;
        __size = 0;
    }

    void Cluster::__cpy(LNodePtr pts) {
        LNodePtr cursor = pts;
        this->__size = 0;

        while (cursor != NULL){
            this->add(cursor->point);
            cursor = cursor->next;
            __size++;
        }
        __id = __idGenerator++;
    }

    bool Cluster::__in(const Point &p) const {

    }
    LNode::LNode(const Point &p, LNodePtr n) :point(p), next(n) { }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c):__c(c),__p(d) {

        __dimensions = d;
        __valid = false;
    }

    const Point Cluster::Centroid::get() const //doesn't check for validity
    {
        return __p;
    }

    void Cluster::Centroid::set(const Point &p) //sets to valid
    {
        __p = p;
        __valid = true;
    }

    bool Cluster::Centroid::isValid() const {
        return __valid;
    }

    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }

    //Functions
    void Cluster::Centroid::compute() {
        setValid(true);

        __p = Point(__dimensions);

        if(__c.__size > 0)
            for (LNodePtr cursor = __c.__points; cursor != NULL; cursor = cursor->next)
                __p+= cursor->point / __c.__size;
        else toInfinity();
    }

    bool Cluster::Centroid::equal(const Point &p) const {
        bool results = false;
        if (this -> __dimensions == p.getDims()){
            results = true;
            for (unsigned int i = 0; results && i < p.getDims(); i++)
                if (this-> __p.getValue(i) != p.getValue(i))
                    results = false;
        }
        return results;
    }
    void Cluster::Centroid::toInfinity() {
         for (int i = 0; i < __dimensions; i++)
             __p[i] = std::numeric_limits<double>::max();
     }

    Cluster::Cluster(unsigned int d): centroid (d, *this) {
        __size = 0;
        __points = NULL;
        __dimensionality = d;
        __id = __idGenerator++;
    }

    // The big three: cpy ctor, overloaded operator=, dtor
    Cluster::Cluster(const Cluster &dim) :centroid(dim.__dimensionality,*this) {
        __size = dim.__size;

        if (__points != NULL)
            __del();
        LNodePtr cursor = dim.__points;
        this->__size = 0;

        while (cursor!= NULL)
        {
            this->add(cursor->point);
            cursor = cursor->next;
        }
        __id = dim.__id;
    }

    Cluster &Cluster::operator=(const Cluster &dim) {
        if(this == &dim)
            return *this;
        if(__dimensionality != dim.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, dim.__dimensionality);

        __del();
        __cpy(dim.__points);

        __id = dim.__id;

        return *this;
    }

    Cluster::~Cluster() {
        __del();
        assert (__size == 0);
    }

    //Getters

    unsigned int Cluster::getId() const {
        return __id;
    }

    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }

    unsigned int Cluster::getSize() const {
        return __size;
    }

    // Add/remove: They allow calling c1.add(c2.remove(p));
    void Cluster::add(const Point &dim) {
        if(__dimensionality != dim.getDims())
            throw DimensionalityMismatchEx(__dimensionality,dim.getDims());
        LNodePtr  newPoint = new LNode(dim, nullptr);

        if (__points == nullptr){
            __points = newPoint;
            centroid.setValid(false);
        }
        else if (__points->point > dim){
            newPoint->next = __points;
            __points = newPoint;
        }
        else
        {
            LNodePtr current = __points, next = __points->next;
            while(true){
                if(next == nullptr || next->point > dim){
                    current->next = newPoint;
                    newPoint->next = next;
                    break;
                }
                else{
                    current = next;
                    next = next->next;
                }
            }
        }
        __size++;
    }

    const Point &Cluster::remove(const Point &dim) {
        if(__dimensionality !=dim.getDims())
            throw DimensionalityMismatchEx(__dimensionality,dim.getDims());

        LNodePtr previous = nullptr, next = nullptr;

        if (__points->point == dim){
            next = __points;
            __points = __points->next;
            delete next;
            __size--;
        }
        else
        {
            previous= __points;
            next = __points->next;

            while(next!= nullptr){
                if (next->point == dim){
                    previous->next = next->next;
                    delete next;
                    __size--;
                    break;
                }
                previous= next;
                next = next->next;
            }
        }

        if (this->contains(dim))
            this->centroid.setValid(false);

        return dim;
    }
    bool Cluster::contains(const Point &p) const {
        bool results = false;
        LNodePtr cursor;

        if(__dimensionality != p.getDims())
            throw DimensionalityMismatchEx(__dimensionality,p.getDims());

        for (cursor = __points; cursor != nullptr; cursor++){
            if(cursor->point == p){
                if (cursor->point.getId() == p.getId()){
                    results = true;
                }
            }
        }
        return results;
    }
    // Centroid functions
    void Cluster::pickCentroids(unsigned int k, Point **pointArray) // pick k initial centroids
    {
        double p1;
        double p2;
        int distance;
        bool results;

        if (k<__size) {

            if (k <= 100) {


                *pointArray[0] = __points->point;

                for (int i = 1; i < __size; i++) {
                    p1 = 0;
                    distance = 0;
                    for (int index = 0; index < __size; index++) {
                        p2 = 0;
                        results = false;

                        for (int count = 0; count < i; count++) {
                            p2 = p2 + ((*this)[index].distanceTo(*pointArray[count]));
                            if ((*this)[index] == *pointArray[count]) {
                                results = true;
                            }
                        }

                        p2 = p2 / i;

                        if (p2 > p1) {
                            if (results == false) {
                                p1 = p2;
                                distance = index;
                            }
                        }
                    }
                    *pointArray[i] = (*this)[distance];
                }
            }
            else
                {
                    for (int i = 0; i < k; ++i) {
                        *pointArray[i] = (*this)[i];

                    }
                }

        }
        else{
            for(int i = 0; i<__size;i++ ){
                *pointArray[i] =(*this)[i];
            }
            if(k!=__size){
                for(int i = __size; i<k;i++){
                    for(int index = 0; index <__dimensionality; index++){
                        pointArray[i]->setValue(index,std::numeric_limits<double>::max());
                    }
                }
            }
        }
    }




    // Overloaded operators

    // Element access (aka direct access, array operator)
    const Point &Cluster::operator[](unsigned int u) const // const version only
    {
        LNodePtr cursor = __points;
        if(__size==0)
            throw EmptyClusterEx();
        if(u >= __size)
            throw OutOfBoundsEx(u,__size);

        if(cursor!= nullptr){
            for (int i = 0; i < u; i++){
                cursor = cursor->next;
            }
        }
        return cursor->point;
    }


    // IO
    // - Friends
    std::ostream &operator<<(std::ostream &os, const Cluster &cluster){
        LNodePtr cursor = cluster.__points;
        for (int i = 0; i < cluster.__size; i++){
            os<< cursor->point << " " << std::endl;
            cursor = cursor->next;
        }
        return os;
    }
    std::istream &operator>>(std::istream &is, Cluster &cluster){
        while(!is.eof()){
            std::string p1;
            getline(is,p1);
            int dim = cluster.__dimensionality;

            if(p1.length() > dim){
                Point newPoint(dim);
                std::stringstream p2(p1);

                try {
                    p2 >> newPoint;
                    cluster.add(newPoint);
                }
                catch(DimensionalityMismatchEx & index) {
                    std::cerr << index << std::endl;
                    newPoint.rewindIdGen();
                }
                catch (...){
                    std::cerr << "Exception" << std::endl;
                    newPoint.rewindIdGen();
                    }


            }
        }
        return is;
    }

    // - Friends
    bool operator==(const Cluster &lhs, const Cluster &rhs){
        bool results = true;
        if(lhs.__size != rhs.__size)
            return false;

        LNodePtr leftPoint = lhs.__points;
        LNodePtr rightPoint = rhs.__points;

        while(leftPoint != nullptr){
            if(leftPoint->point != rightPoint->point){
                results = false;
            }
            else{
                rightPoint = rightPoint->next;
                leftPoint = leftPoint->next;
            }
        }
        return results;
    }
    bool operator!=(const Cluster &lhs, const Cluster &rhs){
        bool results = false;
        if(lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality,rhs.__dimensionality);

        if (lhs.__size != rhs.__size){
            results = true;
            return results;
        }

        LNodePtr leftPoint = lhs.__points;
        LNodePtr rightPoint = rhs.__points;
        while(leftPoint != nullptr){
            if(leftPoint->point == rightPoint->point){
                rightPoint = rightPoint->next;
                leftPoint = leftPoint->next;
            }
            else if(leftPoint->point != rightPoint->point)
                results = true;
        }
        return results;
    }

    // Operators with Point-s
    // - Members
    Cluster &Cluster::operator+=(const Point &rhs) // allocate point
    {
        add(rhs);
        return *this;
    }
    Cluster &Cluster::operator-=(const Point &rhs)// delete point(s)
    {
        if(this->contains(rhs)){
            remove(rhs);
        }
        return *this;
    }
    // Operators with Cluster-s
    // - Members
    Cluster &Cluster::operator+=(const Cluster &rhs) // union
    {
        if(__dimensionality == rhs.getDimensionality()){
            LNodePtr cursor = rhs.__points;
            while(cursor){
                if(this->contains(cursor->point)){
                    cursor = cursor->next;
                }
                else{
                    add(cursor->point);
                    cursor = cursor->next;
                }
            }
            return *this;
        }
        else{
            throw DimensionalityMismatchEx(__dimensionality,rhs.getDimensionality());
        }
    }
    Cluster &Cluster::operator-=(const Cluster &rhs) // (asymmetric) difference
    {
        if(__dimensionality == rhs.getDimensionality()){
            LNodePtr cursor = rhs.__points;
            while(cursor){
                if(this->contains(cursor->point)){
                    remove(cursor->point);
                }
                cursor = cursor->next;
            }
            return *this;
        }
        else{
            throw DimensionalityMismatchEx(__dimensionality,rhs.getDimensionality());
        }
    }
    // Binary operators for Cluster-s
    // - Friends
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs){
        if(lhs.getDimensionality() == rhs.getDimensionality()){
            Cluster sum(lhs);
            sum += rhs;
            return sum;
        }
        else
            throw DimensionalityMismatchEx(lhs.getDimensionality(),rhs.getDimensionality());
    }
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs){
        if(lhs.getDimensionality() == rhs.getDimensionality()){
            Cluster minus (lhs);
            minus -= rhs;
            return minus;
        }
        else
            throw DimensionalityMismatchEx(lhs.getDimensionality(),rhs.getDimensionality());
    }

    // Binary operators for Cluster-s and Point pointers
    // - Friends
    const Cluster operator+(const Cluster &lhs, const Point &rhs){
        Cluster sum(lhs);
        sum += rhs;

        return sum;
    }
    const Cluster operator-(const Cluster &lhs, const Point &rhs){
        Cluster minus(lhs);
        minus -= rhs;
        return minus;
    }
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to):__p(p),__from(from),__to(to) { }

    void Cluster::Move::perform() {
        if (__from.contains(__p)){
            __to.add(__from.remove(__p));
        }
        if(__from.__size == 0){
            __from.centroid.toInfinity();
        }
        if(__to.__size == 0){
            __to.centroid.toInfinity();
        }
    }

}
