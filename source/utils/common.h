#ifndef _COMMON_H_
#define _COMMON_H_

/*!
 *  Set of common definitions to avoid circular imports.
 *
 *  @author Lucas Bazante
 *  @file common.h
 */

#include <sstream>
#include <string>
#include <array>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>
#include <cmath>

using std::ostringstream;
using std::string;
using std::array;

// struct for a bar item
struct Bar
{
    string label;
    string category;
    int value;
    float width;
};

// struct for running options
struct Options
{
    unsigned int max_bar{ 5 }; // maximum number of bars
    unsigned int fps{ 24 };    // FPS animation speed
    string filepath;  // the data file path

    // tunes the options if it is wrongly chosen
    void tune_options( void )
    {
        max_bar = ( max_bar > 15 ) ? 15 : max_bar; // we cant have more than 15 bars
        fps = ( fps > 24 ) ? 24 : fps; // no more than 24 for fps
    }
};

//! Compares two bars
/*! This function compares two bars, checking if bar a is greater than bar b, comparing its values
 *
 *  @param a First bar
 *  @param b Second bar
 *
 *  @return True if a.value > b.value, false otherwise
 */
inline bool cmp_bar( const Bar &a, const Bar &b )
{
    return a.value > b.value;
}  

#endif
