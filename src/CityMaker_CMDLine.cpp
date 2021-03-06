#include "CityMaker_CMDLine.h"
#include "UI_CMDLine.h"

std::unique_ptr<City> CityMaker_CMDLine::makeCity (
    std::vector<CityFactory*> cityFactories,
    int maxDeltaX,
    int maxDeltaY
)
{
    std::vector<std::string> namesOfCityFactories = {};
    for (auto& factory : cityFactories)
    {
        namesOfCityFactories.push_back(factory->toString());
    }
    int cityChoice = _ui.menu("Choose a city type by typing the"
                            " corresponding number.",
                            namesOfCityFactories);
    return cityFactories[cityChoice]->createCity(_ui, maxDeltaX, maxDeltaY);
}

std::unique_ptr<City> CityMaker_CMDLine::makeBaseCity (
    std::vector<CityFactory*> cityFactories,
    int maxDeltaX,
    int maxDeltaY
)
{
    (void) maxDeltaX;
    (void) maxDeltaY;
    int cityChoice = 0;
    return cityFactories[cityChoice]->createBaseCity(_ui, maxDeltaX, maxDeltaY);
}