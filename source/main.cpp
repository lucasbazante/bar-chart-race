/*!
 * This program implements the Bar Chart Race, making use of all of the built classes and utils.
 *
 * @author Lucas Bazante
 * @file main.cpp
 */


// utils
#include "utils/common.h"
#include "utils/text_color.h"

// models
#include "models/dataset.h"
#include "models/barChart.h"
#include "models/fileHandler.h"

int main( int argc, char *argv[] )
{
    if ( argc == 1 ) {
        std::cout << Color::tcolor( "\n>>> [ERROR] : no filepath provided! Terminating execution.\n", Color::BRIGHT_RED, Color::BOLD ) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << Color::tcolor( "\n>>> Welcome to the Bar Chart Race! Please enjoy!", Color::BRIGHT_BLUE, Color::BOLD ) << std::endl;
    std::cout << Color::tcolor( ">>> Reading your file. please wait...", Color::BRIGHT_BLUE, Color::BOLD ) << std::endl;

    Options op;
    
    for ( int i{ 1 }; i < argc; i++ )
    {
        if ( string(argv[i]) == "-b" )
            op.max_bar = std::stoul( argv[i + 1] );
        else if ( string(argv[i]) == "-f" )
            op.fps = std::stoul( argv[i + 1] );
        else
            op.filepath = string( argv[i] );
    }

    op.tune_options();

    FileHandler handler( op.filepath );

    if ( not handler.exists() )
    {
        std::cout << Color::tcolor( "\n>>> [ERROR]: you've provided an invalid filepath! Terminating excution.\n", Color::BRIGHT_RED, Color::BOLD ) << std::endl;
        return EXIT_FAILURE;
    }

    Dataset *ds = new Dataset();

    if ( not handler.get_header( ds ) )
    {
        std::cout << Color::tcolor( "\n>>> [ERROR]: your file has less information than needed! Please double check it.\n", Color::BRIGHT_RED, Color::BOLD ) << std::endl;
        return EXIT_FAILURE;
    }

    if ( not handler.get_data( ds, op.max_bar ) )
    {
        std::cout << Color::tcolor( "\n>>> [ERROR]: your file contains corrupted information! Please double check it.\n", Color::BRIGHT_RED, Color::BOLD ) << std::endl;
        return EXIT_FAILURE;
    }

    ds->display_initial_info( op );
    std::cin.ignore(); // "press enter to continue..."

    auto charts{ ds->get_charts() }; // get the charts
    int size = charts.size(), curr = 0;

    for ( auto chart : charts )
    {
        curr++;
    
        ds->display_header( chart.first );
        chart.second->print_chart( ds );
        chart.second->print_footer( ds->get_label(), ds->get_source() );
        ds->display_categories();

        if ( curr != size ) // only flushes screen if its not the last one
            std::cout << "\033[2J\033[1;1H";

        std::chrono::milliseconds duration{ 1000 / op.fps };
        std::this_thread::sleep_for( duration );

        std::cout << std::endl;
    }

    std::cout << Color::tcolor( "\n>>> That's it for this race! Hope you enjoyed it!\n", Color::BRIGHT_BLUE, Color::BOLD ) << std::endl;
 
    return EXIT_SUCCESS;
}
