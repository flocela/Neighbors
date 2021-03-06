/*
 * Copyright (c) 2018, 2019 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <memory>

#include "CityFactory.h"
#include "CityFactory_Grid.h"
#include "CityMaker_CMDLine.h"
#include "ResidentsFactory_Flat.h"
#include "ResidentsFactory.h"
#include "ResidentsMaker_CMDLine.h"
#include "Printer_Graphic.h"
#include "Printer_CMDLine.h"
#include "Resident_Flat.h"
#include "Simulator.h"
#include "Simulator_Basic_A.h"
#include "Color.h"
#include "City_Grid.h"
#include "Resident_StepDown.h"
#include "Resident_Flat.h"
#include "MainBaseQuestion.h"
#include "SimulationComponents.h"
#include "MainExamples.h"
#include "RandSeedGetter.h"

using std::vector;
using std::unique_ptr;
using std::string;

#define FONT_PATH   "assets/pacifico/Pacifico.ttf"

/* Function Declarations */
template<typename T>
vector<T*> getPointers (vector<unique_ptr<T>>& ts)
{
    vector<T*> pointers = {};
    for (auto& t : ts)
    {
        pointers.push_back(t.get());
    }
    return pointers;
}

template<typename T>
std::set<T*> getSetOfPointers (vector<unique_ptr<T>>& ts)
{
    std::set<T*> pointers = {};
    for (auto& t : ts)
    {
        pointers.insert(t.get());
    }
    return pointers;
}

vector<unique_ptr<CityFactory>> initCityFactories ();
vector<unique_ptr<ResidentsFactory>> initResidentFactories ();
void initForSimpleExample (int example);
void initForUserDefinedRun ();

/* Value Declarations */
unique_ptr<City> city;
vector<unique_ptr<Resident>> residents;
unique_ptr<Simulator> simulator;
int numOfRuns = 5;
int randomSeed = 5;

// SCREEN_WIDTH and SCREEN_HEIGHT is set to 1200 pixels. The city graph
// will take up 1/2 the screen, 600 pixels. Each house must be no smaller
// than 4 pixels, or it will be difficulat to see.
// So the largest number of houses in the x and y directions are 150.
// Calculated as 600/4 = 150.
const int SCREEN_WIDTH   = 1200;
const int SCREEN_HEIGHT  = 1200;
const int MAX_HOUSES_X   = 150;
const int MAX_HOUSES_Y   = 150;

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    vector<unique_ptr<CityFactory>> cityFactories = initCityFactories();
    vector<unique_ptr<ResidentsFactory>> residentFactories = initResidentFactories();

    vector<CityFactory*> cityFactoryPointers = getPointers(cityFactories);
    vector<ResidentsFactory*> resFactoryPointers = getPointers(residentFactories); 

    MainBaseQuestion mainQuestion;
    bool usesExamples = mainQuestion.askUserToUsePremadeExamples();
    SimulationComponents components;
    int randomSeed = 1;
    if (usesExamples)
    {
        srand(randomSeed);
        MainExamples mainExamples;
        components = mainExamples.userChoosesExample();
        city = std::move(components.city);
        residents = std::move(components.residents);
        simulator = std::move(components.simulator);
        numOfRuns = components.numOfRuns;
    }
    else
    {
        RandSeedGetter randSeedGetter;
        randomSeed = randSeedGetter.makeSeedForRand();
        srand(randomSeed);
        CityMaker_CMDLine cityMaker{};
        /*unique_ptr<City> city = cityMaker.makeCity(
            cityFactoryPointers,
            MAX_DELTA_X,
            MAX_DELTA_Y);*/
        city = cityMaker.makeBaseCity(
            cityFactoryPointers, 
            MAX_HOUSES_X, 
            MAX_HOUSES_Y
        );

        ResidentsMaker_CMDLine residentsMaker{};
        //vector<unique_ptr<Resident>> residents = 
        //    residentsMaker.makeResidents(residentFactoryPointers, city->getSize(), _the_color_Infos);
        residents = 
            residentsMaker.makeBaseResidents(resFactoryPointers, city->getSize(), _base_colors);
    
        std::set<Resident*> residentPtrs = {};
        for (auto& resident: residents)
        {
            residentPtrs.insert(resident.get());
        }
        
        simulator = std::make_unique<Simulator_Basic_A>(city.get(), residentPtrs);

    }
    
    
    
/*
    std::map<int, Resident*> houseToResidentMap = {};
    for (auto& resident : residents)
    {
        houseToResidentMap[resident->getID()] = resident.get();
    }*/
    
    std::set<Color> colors;
    for (auto& resident : residents)
    {
        colors.insert(resident->getColor());
    }

    Printer_Graphic graphicPrinter{4, 1200, 1200, city.get(), colors};
    std::map<House*, Resident*> houseToResidentMap;
    for (int ii=0; ii< numOfRuns; ii++)
    {   
        houseToResidentMap = simulator->simulate();
        graphicPrinter.print(houseToResidentMap, ii, "Title");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    Printer_CMDLine cmdLinePrinter{numOfRuns, city.get()};
    //cmdLinePrinter.print(houseToResidentMap, numOfRuns, "Title");
    graphicPrinter.keepScreen();
    return 0; 
}

vector<unique_ptr<CityFactory>> initCityFactories ()
{
    vector<unique_ptr<CityFactory>> cityFactories = {};
    cityFactories.emplace_back(std::make_unique<CityFactory_Grid>());
    return cityFactories;
}

vector<unique_ptr<ResidentsFactory>> initResidentFactories()
{
    vector<unique_ptr<ResidentsFactory>> residentFactories = {};
    residentFactories.emplace_back(std::make_unique<ResidentsFactory_Flat>());
    return residentFactories;
}
