//#define BOOST_PYTHON_STATIC_LIB

#include <iostream>

#include <vector>

#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "MyRandom.hpp"


// ================================================ //


class PyRandom : public MyAll::MyRandom {

 public:

    //typedef std::vector<int> int_vector;

    void Seed(
        const std::string &seed_type__ = "device", // "seed", "time", and "device"
        const int &seed_val__ = -1
    ) {

        MyAll::MyRandom::Seed(seed_type__,seed_val__);

    }

    //

    int GetSeed() {

        std::string seed_type;
        int seed_val;

        MyAll::MyRandom::GetSeed(seed_type, seed_val);
        std::cout << seed_type << " : " << seed_val << std::endl;
        return seed_val;
    }


    //

    std::vector<int> GetVector_uni(
        const int &rand_min__,
        const int &rand_max__,
        const int &size__,
        const bool &overlap = false
    ) {

        std::vector<int> result;
        MyAll::MyRandom::MakeVector_uni(rand_min__,rand_max__,size__,result,overlap);
        return result;

    }

 private:

    //int_vector result_;

};


// ================================================ //


BOOST_PYTHON_MODULE( PyRandom ){

    using namespace boost::python;

    //

    class_<PyRandom>("PyRandom")
    .def("Seed", &PyRandom::Seed)
        .def("GetSeed", &PyRandom::GetSeed)
        .def("GetVector_uni", &PyRandom::GetVector_uni);

    //

    class_<std::vector<int>>("int_vector")
        .def(vector_indexing_suite<std::vector<int>>());

};


// ================================================ //
