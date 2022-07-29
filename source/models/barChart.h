#ifndef _BAR_CHART_H_
#define _BAR_CHART_H_

/*!
 *  This file contains a class to store a single chart.
 *  A chart contains n bars, and it is "mapped" to a single timestamp.
 *
 *  @author Lucas Bazante
 *  @file bar_chart.h
 */

#include "../utils/common.h"
#include "../utils/text_color.h"
#include "dataset.h"

class BarChart
{
    private:
        std::vector< Bar > bars; // our vector of Bar items
        string timestamp; // timestamp for the BarChart object
        unsigned int n_bars; // number of bars in the BarChart object

    public:
        //! Appends a bar
        /*! This method appends a Bar item to our vector `bars`.
         *
         *  @param bar The Bar item to be appended
         */
        void push_a_bar( Bar bar )
        {
            this->bars.push_back( bar );
        }

        //! Sets the timestamp
        /*! This method sets the BarChart object `timestamp`, so we can
         *  access it later on and keep control of each BarChart related to its timestamp.
         *
         *  @param ts The textr string containing the timestamp
         */
        void set_timestamp( string ts )
        {
            this->timestamp = ts;
        }

        //! Sets number of bars
        /*! This method sets the number of bars, `n_bars`, of our object.
         *  With this, we can safely use it for knowing how many bars our chart must have.
         *  This will either set it to the number found on the text file or to the picked value from the running options, whichever is smaller.
         *
         *  @param n The number of bars
         */
        void set_n_bars( unsigned int n )
        {
            this->n_bars = n;
        }

        //! Gets the chart's timestamp
        /*! This method gets the timestamp of this chart object.
         *
         *  @return The timestamp
         */
        string get_timestamp( void )
        {
            return this->timestamp;
        }

        //! Sorts the bar vector
        /*! This method sorts our `bars` vector, returning it in a non-decreasing orderly fashion.
         *  It does so by comparing the bar items values with the function cmp_bar, that returns
         *  true if a given bar's value is greater than the other.
         *  
         *  We sort it non-decreasingly because we want to print the bars in the chart precisly in this order,
         *  and by sorting we can also remove the "lesser" bars which exceed our `n_bars` limit, which is done
         *  with `purge_bars()`.
         */
        void sort_bars( void )
        {
            std::sort( this->bars.begin(), this->bars.end(), cmp_bar );
        }

        //! Removes unneeded bars
        /*! This method removes all the unneeded bars from our vector. When this method is called,
         *  we assume that the vector is already sorted, i.e. `sort_bars()` was already called.
         *  So, assuming it is sorted, we use `std::vector.pop_back()`, which removes the last element,
         *  until our vector size is equal to our limit `n_bars`; as we only need to use the greater `n_bars` from each chart,
         *  we can safely remove the unneeded ones.
         */
        void purge_bars( void )
        {
            while ( this->bars.size() != this->n_bars )
                this->bars.pop_back();
        }

        //! Sets the widths for all our bars
        /*! This method set the widths for all of our bars. This is a "relative width"; it is done by
         *  taking the maximum value from the vector, i.e. the first element since it is already sorted at this point,
         *  and making it as "100%" of the width. The rest of the bars will have a percentage of the width based on the max value;
         *  for example, supposing a max value of 500 and a bar with value 250, the second bar will have 250/500 = 0.5 = 50% of the full width.
         *  
         *  The full width is a fixed value of 150.
         */
        void set_widths( void )
        {
            int max = this->bars[0].value; // the vector is already sorted and "purged" at this point

            for ( auto &bar : this->bars )
            {
                float percentage = ( float ) bar.value / ( float ) max;
                bar.width = 150 * percentage;
            }
        }


        //! Prints the chart object
        /*! This method prints the chart object, printing the entire vector. At this point, the vector
         *  is already sorted and freed from exceeding bars, so we don't need to worry about ranges whatsoever.
         *  Each bar gets its color based on its category, making use of the pointer to Dataset object, in which the
         *  relation "category x colors" are stored.
         *
         *  @param ds A pointer to a Dataset object, from which we'll extract the colors for the bars.
         */
        template < class DatasetPointer >
        void print_chart( DatasetPointer ds )
        {
            for ( auto bar : this->bars )
            {
                short color = ds->get_color( bar.category ); // getting the color for the category, or a single color if there are more than 14 categories
                string colored = Color::tcolor( Color::multiply( Color::UNIT, bar.width ), color ); // coloring the bar
                std::cout << colored << " " << Color::tcolor( bar.label, color ) << " [" << bar.value << "]" << std::endl << std::endl; // print label [value]
            }
        }

        //! Prints the footer of the chart
        /*! This method prints the footer for the chart object.
         *  The footer contains the source of the dataset, a label containing the unit the dataset is operating on (example, Population (thousands)),
         *  and a x-axis, containing five evenly spaced points on the min and max values from the chart, marked by "+" on the axis. As each BarChart is printed out,
         *  the axis will change accordingly with the values.
         *
         *  @param label String text containing the label of the chart
         *  @param source String text containing the source of the chart
         */
        void print_footer( string label, string source )
        {
            std::stringstream axis; // the "ticks" ----+---+---->
            std::stringstream points; // the values from each mark +

            // it always start on 0
            axis << "+";
            points << "0";

            std::vector< int > sequence; // the values
            std::vector< int > widths; // the value's widths

            int max = ( this->bars.begin() )->value;
            int min = ( this->bars.end() - 1 )->value;
            
            // how many digits each?
            int dmax = std::log10( max ) + 1;
            int dmin = std::log10( min ) + 1;

            // powers of ten based on digit; ex: a 4 digit number as max value would have 10^(4 - 2) as its power
            int maxpow = ( int ) ( std::pow( 10, dmax - 2 ) + 0.5 );
            int minpow = ( int ) ( std::pow( 10, dmin - 1 ) + 0.5 );

            min -= ( min % minpow ); // rounds down to nearest n-digit number; i.e 361 would go to 300
            max += ( maxpow - ( max % maxpow ) ); // rounds up to nearest multiple of maxpow (10 ^ {dmax - 2})

            int step = ( max - min ) / 5; // calculate step for 5 points

            if ( step == 0 )
            { 
                for ( int i = 0; i <= 150; i++ )
                {
                    if ( i == 150 )
                        axis << "+", points << max;

                    axis << "-", points << " ";
                }
            }
            else 
            {
                // set widths for all 5 points
                for ( int i = min; i <= max; i += step )
                {
                    sequence.push_back( i ); // pusheds the value
                    float percentage = ( float ) i / ( float ) max;
                    int width = 150 * percentage;
                    widths.push_back( width ); // pushes the width for the value
                }

                int count =  0;
                short lw = 0; // keep track of how many digits the last value got
                for ( int i = 0; i <= *( widths.end() - 1 ); i++ )
                {
                    if ( std::find( widths.begin(), widths.end(), i ) != widths.end() )
                    {
                        axis << "+";
                        points << sequence.at( count );
                        lw = std::log10( sequence.at( count ) ); // digits - 1 from the last number, so we can skip whitespaces on points
                        count++;
                        continue;
                    }

                    axis << "-";
                    
                    if ( lw ) 
                    { 
                        lw--; 
                        continue; 
                    }

                    points << " "; // it'll only puts a whitespace if we align it with the axis, skipping the number of digits of the last value added
                }
            }

            axis << "----------------------------->"; // adding a final

            std::cout << axis.str() << std::endl << Color::tcolor( points.str(), Color::YELLOW ) << std::endl;
            std::cout << Color::tcolor( label, Color::YELLOW, Color::BOLD ) << std::endl << std::endl;
            std::cout << Color::tcolor( source, Color::WHITE, Color::BOLD ) << std::endl;
        }
};

#endif
