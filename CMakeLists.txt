cmake_minimum_required(VERSION 3.23)
project(adsP1)

set(CMAKE_CXX_STANDARD 17)

add_executable(P1_dict
        main.cpp
        ./external/porter2_stemmer.cpp
        ./external/porter2_stemmer.h
        ./external/util/hash.h
        ./external/util/string_view.h)
add_executable(Query
        query.cpp
        ./external/porter2_stemmer.cpp
        ./external/porter2_stemmer.h
        ./external/util/hash.h
        ./external/util/string_view.h)
