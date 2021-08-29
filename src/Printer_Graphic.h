#ifndef PRINTER_Graphic_H
#define PRINTER_Graphic_H

#include "GraphicCityPrinter.h"
#include "Printer.h"
#include "renderer.h"
#include <thread>
#include <memory>
#include "House.h"
#include <vector>
#include "Color.h"
#include "GraphicRunCounterPrinter.h"

class Printer_Graphic : public Printer
{   
    public:
        Printer_Graphic() = delete;
        Printer_Graphic (
            int maxNumOfRuns,
            const std::size_t screen_width,
            const std::size_t screen_height,
            City* cityPtr,
            std::set<Color> colors
        );
        Printer_Graphic (const Printer_Graphic& obj) = default;
        Printer_Graphic (Printer_Graphic&& obj) noexcept = default;
        Printer_Graphic& operator= (const Printer_Graphic& obj) = default;
        Printer_Graphic& operator= (Printer_Graphic&& obj) noexcept = default;
        ~Printer_Graphic () = default;

        void print(
            std::map<House*, Resident*> residentPerHouse,
            int run,
            std::string title
        ) override;

        void keepScreen();
    
    private:
        int _num_of_runs = 0;
        int _max_num_of_runs;
        std::size_t _screen_width;
        std::size_t _screen_height;
        std::unique_ptr<Renderer> _renderer;
        std::set<Color> _colors;
        std::unique_ptr<GraphicCityPrinter> _city_printer;
        std::unique_ptr<GraphicRunCounterPrinter> _run_counter_printer;
        std::map<Coordinate, House*> _coord_to_house_map = {};

        // COMMON TO ALL CHARTS //
        const int _axis_font_size = 20;


        // MAP OF HOUSES IN CITY //

        // Each house is inside of a square cell. The cell 
        // will hold a house (represented by a colored square) and have
        // clear border around the house.
        int _cell_size;  // pixels for square cell
        int _house_size; // pixels for colored square house

        // House coordinates from the city, not in pixels.
        int _min_x_coord = INT32_MAX; // top left corner of grid 
		int _min_y_coord = INT32_MAX; // top left corner of grid
		int _max_x_coord = INT32_MIN; // bottom right corner of grid
		int _max_y_coord = INT32_MIN; // bottom right corner of grid

		// x-axis is vertically offset from origin by x-axis-offset
		const int _city_x_axis_offset = 10;

        // y-axis is vertically offset from origin by y-axis-offset
		const int _city_y_axis_offset = 10;

		// x-axis overruns its data by x_axis_overrun
		const int _city_x_axis_overrun = 0;

        // y-axis overruns its data by y_axis_overrun
		const int _city_y_axis_overrun = 0;

        // location where x-y axes meet in pixels.
        const int city_cross_hairs_x_px = 240;
        const int city_cross_hairs_y_px = 80;
        

        // NUMBER OF RUNS COUNTER // 
        
        // x and y offsets for title displaying number of runs so far.
        const int _x_num_of_runs_offset = 80;
        const int _y_num_of_runs_offset = 80;

        // Moves non-changing information from the city into class attributes
        // (which by the way will also not be changing). For example City
        // houses' coordinates will not change so initializes _coord_to_house_map. 
        // The city's most west, east, north, and south coordinates
        // will not change. So the _min_x_coord and the like are initialized. 
        // We have _screen_width and min and max coordinates so the _cell_size
        // is initialized.
        void initCityCoordinateInfo (City* cityPtr);

        // initialize _graphic_city_printer with base information
        // like where to put grid (graphOrigin), x and y axis offsets,
        // cell_size, rgba for residents' colors and the like. Basically 
        // set up the graph where the residents will be plotted.
        void initCityPrinter (Coordinate graphOrigin);

        void initRunCounterPrinter (int maxNumOfRuns);

};

#endif