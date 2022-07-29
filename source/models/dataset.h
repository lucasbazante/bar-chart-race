#ifndef _DATASET_H_
#define _DATASET_H_

/*!
 *  This file contains a class to store the information about the data in the passed file
 *
 *  @author Lucas Bazante
 *  @file dataset.h
 */

#include "../utils/common.h"
#include "../utils/text_color.h"
#include "barChart.h"

class Dataset
{
    private:
        std::map< string, BarChart* > charts; // maps a chart pointer with a timestamp
        std::map< string, short > categories; // maps each category with a short integer reprenting its color
        
        bool single_colored = false; // if we have more than 14 categories, this'll be set to true
        short color = 31; // initial color

        // header info
        string title;
        string label;
        string source;

    public:
        //! Inserts a chart
        /*! This method inserts a pointer to a BarChart object to our map `charts`, mapping
         *  the pointer with a timestamp, the timestamp from the chart.
         *
         *  @param chart Pointer to a chart object that'll be stored.
         */
        void push_a_chart( BarChart *chart )
        {
            chart->sort_bars(); // sorts the received chart
            chart->purge_bars(); // removes exceeding bars
            chart->set_widths(); // sets the widths
            this->charts.insert( { chart->get_timestamp(), chart } ); // inserts
        }

        //! Inserts a category
        /*! This method inserts a category into our categories map, mapping it with a short integer,
         *  which represents one of the 14 colors available. If the categories size reaches a value greater than
         *  14, sets the bool `single_colored` to true, because we only have 14 colors, so if there's more, it'll
         *  set all categories to a single color.
         *
         *  @param category The category to be inserted
         */
        void push_a_category( string category )
        {
            if ( this->categories.size() > 14 )
                this->single_colored = true;

            if ( this->categories.insert( { category, this->color } ).second )
                if ( not this->single_colored ) // only increases if we have enough
                    this->color = ( this->color + 1 == 38 ) ? 90 : this->color + 1; // sequentially define the color, past 37 the next is 90
        }

        //! Gets label
        /*! This method gets the dataset's label.
         *  
         *  @return The dataset's label.
         */
        string get_label( void )
        {
            return this->label;
        }

        //! Gets source
        /*! This method gets the dataset's source.
         *
         *  @return The dataset's source.
         */
        string get_source( void )
        {
            return this->source;
        }

        //! Gets the charts
        /*! This method returns the map `charts`, so the client can access it and flag
         *  the print functions
         *
         *  @return The dataset's charts map.
         */
        std::map< string, BarChart* > get_charts( void )
        {
            return this->charts;
        }

        //! Sets header info
        /*! This method sets the dataset's header info, meaning its title, source and label.
         *  
         *  @param t The title
         *  @param l The label
         *  @param s The source
         */
        void set_info( string t, string l, string s )
        {
            this->title = t;
            this->label = l;
            this->source = s;
        }

        //! Gets color short
        /*! This method returns a short representing a color based on the passed category,
         *  returning the short integer mapped to the category. If there are more than 14 categories,
         *  i.e. `single_colored` is true, it'll return `Color::RED` no matter the category.
         *
         *  @param category The category to get the color
         *
         *  @return The short mapped to the category or a single color for all.
         */
        short get_color( string category )
        {
            if ( this->single_colored )
                return Color::RED;
            return this->categories[category];
        }

        //! Display dataset information
        /*! This method prints the information about the dataset, printing its title, label, source,
         *  how many categories, how many charts, all the categories colored by its related colors (or all in a single color if thats the case).
         *  It also prints the FPS, i.e. the animation speed.
         *
         *  @param op The running options, from where we'll extract the defined FPS
         */
        void display_initial_info( Options op )
        {
            std::stringstream msg;
            msg << "\n\n>>> We have " << this->charts.size() << " charts.\n\n";

            msg << ">>> Animation speed is " << op.fps;
            msg << "\n>>> Title: " << this->title;
            msg << "\n>>> Value: " << this->label;
            msg << "\n>>> Source: " << this->source;
            
            msg << "\n\n>>> We have " << this->categories.size() << " categories among the data:\n";

            std::cout << Color::tcolor( msg.str(), Color::GREEN, Color::BOLD ) << std::endl;

            msg.str( string() ); // resets buffer

            for ( auto cat : this->categories ) // prints all categories
            { 
                msg << "[" << cat.first << "]"; 
                std::cout << Color::tcolor( msg.str(), ( single_colored ? Color::RED : cat.second ), Color::BOLD ); // either the category color or a single color
                std::cout << " ";
                msg.str( string() ); // resets buffer    
            }

            std::cout << Color::tcolor( "\n\n>>> Press ENTER to begin the race: \n", Color::BLUE, Color::BOLD );
        }

        //! Display the categories and its colors as a legend
        /*! This method prints the categories and its mapped colors, as a legend, in the format "Color: category".
         *  If `singe_color` is true, it'll print only a single color (red) and the category "All", informing us that all
         *  the categories will be mapped to the same color, due to lack of colors to represent everything.
         */
        void display_categories( void )
        {
            std::stringstream msg;
            
            if ( single_colored )
            {
                msg << Color::UNIT << ": All";
                std::cout << Color::tcolor( msg.str(), Color::RED ) << std::endl;
                return;
            }


            for ( auto cat : this->categories )
            {
                msg << Color::UNIT << ": " << cat.first;
                std::cout << Color::tcolor( msg.str(), cat.second, Color::BOLD ) << "  ";
                msg.str( string() );
            }
            std::cout << std::endl;
        }

        //! Display header of the dataset
        /*! This method prints the header of the current chart, printing the title (which is the same for every chart)
         *  and the current timestamp.
         */
        void display_header( string timestamp )
        {
            std::cout << std::endl;
            std::cout << std::setw( 80 - ( this->title.length() / 2 ) ) << "" << Color::tcolor( this->title, Color::WHITE, Color::BOLD ) << std::endl << std::endl; 
            std::cout << std::setw( 75 - ( timestamp.length() / 2 ) ) << "" 
                      << Color::tcolor( "Timestamp: ", Color::WHITE, Color::BOLD ) 
                      << Color::tcolor( timestamp, Color::WHITE, Color::BOLD ) << std::endl << std::endl;
        }
};

#endif
