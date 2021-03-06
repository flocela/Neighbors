#include "MainExamples.h"

#include <string>
#include "City_Grid.h"
#include "Resident_StepDown.h"
#include "Simulator_Basic_A.h"
#include "SimulationComponents.h"

template<typename T>
std::set<T*> getSetOfPointers (std::vector<std::unique_ptr<T>>& ts)
{
    std::set<T*> pointers = {};
    for (auto& t : ts)
    {
        pointers.insert(t.get());
    }
    return pointers;
}

SimulationComponents MainExamples::userChoosesExample ()
{
    SimulationComponents components;
    int chosenExample = ui.menu(
        "Which example do you want to choose? ",
        std::vector<std::string>{ 
            "simple with a small city", 
            "simple with a large city",
            "finicky residents with a small city",
            "finicky residents with a large city"}
        );
    switch (chosenExample)
    {
        // 30 x 30 city with 400 houses. 
        // 200 StepDown (Blue)
        //     Unhappy if 80% or more neighbors are different from themselves.
        // 50  StepDown Residents (Red)
        //     Unhappy if 25% or more neighbors are different from themselves.
        case 1:
        {
            components.city = std::make_unique<City_Grid>(151);
            for (int ii=0; ii<200; ++ii)
            {
                components.residents.push_back(std::make_unique<Resident_StepDown>(
                    ii,
                    Color::blue,
                    5,
                    1,
                    1,
                    0,
                    0.8
                ));
            }
            for (int jj=200; jj<250; ++jj)
            {
                components.residents.push_back(std::make_unique<Resident_StepDown>(
                    200,
                    Color::red,
                    5,
                    1,
                    1,
                    0,
                    0.25
                ));
            }
            components.simulator = std::make_unique<Simulator_Basic_A>(
                components.city.get(),
                getSetOfPointers(components.residents)
            );
            components.numOfRuns = 1;
            
            break;
        }
    }
    return components;
}