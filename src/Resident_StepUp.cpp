#include "Resident_StepUp.h"
#include <iostream>

Resident_StepUp::Resident_StepUp ( 
    int id,
    Color color,
    double allowedMovementDistance,
    double happinessGoal,
    double happinessValueAtZeroDiversity,
    double happinessValueAtOneDiversity,
    double diversityWhereRiseHappens):
    Resident(id, color, allowedMovementDistance, happinessGoal),
    _happiness_at_zero_diversity{happinessValueAtZeroDiversity},
    _happiness_at_one_diversity{happinessValueAtOneDiversity},
    _diversity_where_rise_happens{diversityWhereRiseHappens},
    _happiness_func{
        happinessValueAtZeroDiversity, 
        happinessValueAtOneDiversity, 
        diversityWhereRiseHappens
    }
{
}

std::string Resident_StepUp::toStrBasic() const
{
    return "Res_StepUp(id, clr, mov, goal, hap0, hap1, rise): (" + 
        std::to_string(getID()) +
        ", " + _the_color_infos[getColor()]._my_string +
        ", " + std::to_string(getAllowedMovementDistance()) +
        ", " + std::to_string(getHappinessGoal()) +
        ", " + std::to_string(_happiness_at_zero_diversity) +
        ", " + std::to_string(_happiness_at_one_diversity) +
        ", " + std::to_string(_diversity_where_rise_happens) +
        ")"; 
}

double Resident_StepUp::implCalculateHappiness (
    std::set<Resident*> neighbors,
    int numOfAdjacentHouses
) const
{
    int like  = 0;
    int diff  = 0;
    for (Resident* res : neighbors)
    {
        if (res->getColor() == getColor())
        {
            like++;
        }
        else
            diff++;
        
    }
    return _happiness_func.getHappiness(numOfAdjacentHouses, like, diff);
}

House* Resident_StepUp::findHome(
    House* oldHouse,
    std::map<House*, std::set<House*>> openHousesAndTheirAdjacentHouses,
    std::map<House*, Resident*> houseToResMap
) const
{
    for (auto h2N : openHousesAndTheirAdjacentHouses)
    {
        House* currHouse = h2N.first;
        std::set<House*> setOfAdjHouses = h2N.second;
        std::set<Resident*> currHouseAdjacentResidents;
        for (House* nH : setOfAdjHouses)
        {
            if (houseToResMap.count(nH) > 0)
                currHouseAdjacentResidents.insert(houseToResMap.at(nH));
        }
        int numOfNeighboringHouses = openHousesAndTheirAdjacentHouses.at(currHouse).size();
        double happiness = implCalculateHappiness(
            currHouseAdjacentResidents,
            numOfNeighboringHouses
        );
        if (happiness >= getHappinessGoal())
            return currHouse;
    }

    return oldHouse;
}

House* Resident_StepUp::findBestHome(
    House* oldHouse,
    std::map<House*, std::set<House*>> openHousesAndTheirAdjacentHouses,
    std::map<House*, Resident*> houseToResMap
) const
{
    return findHome(oldHouse, openHousesAndTheirAdjacentHouses, houseToResMap);
}