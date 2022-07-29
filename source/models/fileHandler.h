#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

/*!
 * Class to handle all the file operations
 *
 * @author Lucas Bazante
 * @file file_handler.h
 */

#include "../utils/common.h"
#include "barChart.h"
#include "dataset.h"

class FileHandler
{
    private:
        string fname; // name of file

    public:
        //! Constructor
        /*! Constructor method.
         *  Sets the `fname`
         *
         *  @param path String with the filepath
         */
        FileHandler( string const &path ) : fname{ path }
        { /* empty */ }

        //! Check file
        /*! This method checks if the given path points to an existent file,
         *  and if we can open it.
         *
         *  @return True if file is good, false otherwise.
         */
        bool exists( void )
        {
            std::ifstream f( this->fname );
            bool good =  f.good();
            f.close();

            return good;
        }

        //! Gets header of the dataset
        /*! This method gets the header of the dataset, i.e. its title, label and source,
         *  located in the first 3 lines of the file. If there's any information missing,
         *  returns false;
         *
         *  @param ds Pointer to the dataset in which we'll store the info.
         *
         *  @return True if all header info is present, false otherwise
         */
        bool get_header( Dataset *ds )
        {
            std::ifstream f( this->fname );

            string t, l, s;

            std::getline( f, t );
            std::getline( f, l );
            std::getline( f, s );

            if ( t.empty() or l.empty() or s.empty() ) // if any are empty
                return false;

            ds->set_info( t, l, s );

            f.close();

            return true;
        }

        //! Gets all data
        /*! This method gets all the data we need to form our charts.
         *  It reads an integer n_bars, informing us of how many lines of data will follow after for each of the
         *  charts; each chart is separated by an empty line, starting with the n_bars integer.
         *  This method will return false if we read a n_bars integer and read less datalines than what it was informed.
         *
         *  @param ds Pointer to the dataset in which we'll store the charts
         *  @param max_bar Max number of bars from the running options; we'll use to compare with each n_bars got, so we can always use whichever is smaller
         *  
         *  @return True if everything is OK with the file info, false otherwise
         */
        bool get_data( Dataset *ds, unsigned int max_bar )
        {
            std::ifstream f( this->fname );
            string line;
            BarChart *chart = nullptr; // starts as a null pointer

            while ( std::getline( f, line ) )
                if (line.empty()) // skipping header info
                    break;

            int n_bars = 0;
            while ( std::getline(f, line) )
            { 
                if ( line.empty() and n_bars > 0 )
                    return false; // means we expected n_bars, but got a whitespace before, meaning less bars than expected

                if ( line.empty() and chart == nullptr ) // empty line to delimiter the charts
                    continue;

                if ( n_bars == 0 ) // if no n_bars is set yet, or if we already ran over n_bars lines
                {
                    if ( chart != nullptr )
                        ds->push_a_chart( chart ); // push a chart to the dataset

                    if ( not line.empty() )
                    {
                        n_bars = std::stoi( line ); // gets a new n_bars
                        chart = new BarChart(); // resets the chart pointer
                    
                        // now we compare n_bars with max_bar, setting the chart's n_bars as whichever is smaller
                        // we do so because if max_bar > n_bars, it'll cause an error, since there is no sufficient number of bars to work with
                        if ( max_bar > n_bars )
                            chart->set_n_bars( n_bars );
                        else
                            chart->set_n_bars( max_bar );
                    }

                    continue;
                }

                
                Bar bar; // a new bar
                int pos = 0;
                char token = ','; // the info on each line (bar) is separated by commas

                std::stringstream ss( line ); // to separate
                string str;

                while ( std::getline( ss, str, token ) ) // while we can separate by comma
                {
                    switch( pos++ )
                    {
                        case 0: // first position is the chart's timestamp
                        {
                            chart->set_timestamp( str );
                            break;
                        }
                        case 1: // second position is the label for the bar
                        {
                            bar.label = str;
                            break;
                        }
                        case 3: // third position is the value; skipping second position as we dont need it
                        {
                            bar.value = std::stoi( str );
                            break;
                        }
                        case 4: // fourth position is the bar's category
                        {
                            bar.category = str;
                            ds->push_a_category( str ); // pushes the category, but only if it doesn't exisits already
                            break;
                        }
                    }
                }
                chart->push_a_bar( bar ); // after we populate the bar, push it to the chart
                --n_bars; // we have now less one bar to read
            }

            f.close();

            return true;
        }
};

#endif
