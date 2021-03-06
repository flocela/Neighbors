#include "catch.hpp"
#include "../src/Resident_Flat.h"
#include "../src/Resident_StepDown.h"
using Catch::Matchers::Contains;

// Neighbors are used in getHappiness() method.
// Only arugment that matteres is Color argument.
Resident_Flat blueNeighbor01{1, Color::blue, 0.0, 0.0, 0.0 };

Resident_StepDown redNeighbor02{2, Color::red, 0.0, 0.25, 0.1, 0.0, 0.0};

Resident_StepDown redNeighbor03{3, Color::red, 0.0, 0.25, 0.1, 0.0, 0.0};
    
Resident_StepDown greenNeighbor04{4, Color::green, 0.0, 0.0, 0.1, 0.0, 0.0};

Resident_StepDown greenNeighbor05{5, Color::green, 0.0, 0.0, 0.1, 0.0, 0.0};

Resident_StepDown greenNeighbor06{6, Color::green, 0.0, 0.0, 0.1, 0.0, 0.0};

TEST_CASE("Resident_Flat Ctor throws exception if happinessGoal is greater than 1.0.")
{   
    REQUIRE_THROWS_WITH(Resident_Flat(
            0,
            Color::green,
            0,
            1.2,
            0.0
        ), 
        Contains("HappinessGoal must be between 0.0 and 1.0 inclusive.") 
    );
}


TEST_CASE("Resident_Flat Ctor throws exception if happinessGoal is less than 0.0")
{   
    REQUIRE_THROWS_WITH(
        Resident_Flat(
            0,
            Color::green,
            0.0,
            -0.1,
            0.0
        ), 
        Contains("HappinessGoal must be between 0.0 and 1.0 inclusive.")
    );
}

TEST_CASE("Resident_Flat Ctor throws exception if happinessValue is greater than 1.0.")
{   
    REQUIRE_THROWS_WITH(
        Resident_Flat(
            0, 
            Color::green,
            0.0,
            0.0,
            1.2
        ), 
        Contains("HappinessValue must be between 0.0 and 1.0 inclusive.")
    );
}

TEST_CASE("Resident_Flat Ctor throws exception if happinessValue is less than 0.0.")
{   
    REQUIRE_THROWS_WITH(
        Resident_Flat(
            0, 
            Color::green,
            0.0,
            0.0,
            -.25
        ), 
        Contains("must be between 0.0 and 1.0 inclusive")
    );
}


TEST_CASE("getHappines() returns happinessValue")
{
    Resident_Flat blueResident {0, Color::blue, 0.0, 0.0, 0.25};
    std::set<Resident*> neighbors = { 
        &blueNeighbor01,
        &greenNeighbor04,
        &greenNeighbor05,
        &greenNeighbor06
    };
    REQUIRE(blueResident.calculateHappiness(neighbors, 4) == 0.25);
}

TEST_CASE("toStrBasic() ")
{
    Resident_Flat resident { 1,
                            Color::green,
                            1.0,
                            1.0,
                            0.25 };
    REQUIRE(resident.toStrBasic() == "Res_Flat(id, clr, mov, goal, hapV):"
        " (1, green, 1.000000, 1.000000, 0.250000)");
}

// findingHome() and findingBesHome() methods use this map
//    0   5   10   15   20
//    1   6   11   16   21
//    2   7   12   17   22
//    3   8   13   18   23
//    4   9   14   19   24

House hs0 = House{0};
House hs1 = House{1};
House hs2 = House{2};
House hs3 = House{3};
House hs4 = House{4};
House hs5 = House{5};
House hs6 = House{6};
House hs7 = House{7};
House hs8 = House{8};
House hs9 = House{9};
House hs10 = House{10};
House hs11 = House{11};
House hs12 = House{12};
House hs13 = House{13};
House hs14 = House{14};
House hs15 = House{15};
House hs16 = House{16};
House hs17 = House{17};
House hs18 = House{18};
House hs19 = House{19};
House hs20 = House{20};
House hs21 = House{21};
House hs22 = House{22};
House hs23 = House{23};
House hs24 = House{24};

TEST_CASE("Resident_Flat findHome()"
    " returns one of the open houses.")
{
    Resident_Flat greenResident { 
        1,
        Color::green,
        0.0,
        0.5, // happinessGoal
        0.6  // happinessValue
    };

    House& oldHouse = hs0;   // resident's current house
    House& grHouse8  = hs8;  // green resident lives in house 8
    House& blHouse16 = hs16; // blue resident lives in house 16
    House& house11 = hs11; // next to blue resident
    House& house13 = hs13; // next to green resident

    std::set<House*> housesAdjToHouse13;
    housesAdjToHouse13.insert(&hs7);
    housesAdjToHouse13.insert(&hs8);
    housesAdjToHouse13.insert(&hs9);
    housesAdjToHouse13.insert(&hs12);
    housesAdjToHouse13.insert(&hs14);
    housesAdjToHouse13.insert(&hs17);
    housesAdjToHouse13.insert(&hs18);
    housesAdjToHouse13.insert(&hs19);

    std::set<House*> housesAdjToHouse11;
    housesAdjToHouse11.insert(&hs5);
    housesAdjToHouse11.insert(&hs6);
    housesAdjToHouse11.insert(&hs7);
    housesAdjToHouse11.insert(&hs10);
    housesAdjToHouse11.insert(&hs12);
    housesAdjToHouse11.insert(&hs15);
    housesAdjToHouse11.insert(&hs16);
    housesAdjToHouse11.insert(&hs17);

    std::map<House*, std::set<House*>> openHousesAndTheirAdjHouses;
    openHousesAndTheirAdjHouses.insert(std::pair<House*, std::set<House*>>(
        &house11,
        housesAdjToHouse13
        )
    );
    openHousesAndTheirAdjHouses.insert(std::pair<House*, std::set<House*>>(
        &house13,
        housesAdjToHouse11)
    );

    std::map<House*, Resident*> houseToResMap;
    houseToResMap.insert(std::pair<House*, Resident*>(&grHouse8,&greenNeighbor04));
    houseToResMap.insert(std::pair<House*, Resident*>(&blHouse16, &blueNeighbor01));
    
    House* answerHouse = greenResident.findHome(
        &oldHouse,
        openHousesAndTheirAdjHouses,
        houseToResMap
    );

    REQUIRE(
        (answerHouse == &house13 ||
         answerHouse == &house11
        )
    );

}

TEST_CASE("Resident_Flat findBestHome()"
    " green resident has choice of a) two green neigbors, b) two green + 1 blue neighbors,"
    " or c) one blue plus one green neigbor, chooses a).")
{  
    Resident_Flat greenResident { 
        1,
        Color::green,
        0.0,
        0.5, // happinessGoal
        0.6  // happinessValue
    };

    House& oldHouse = hs0;  // resident's current house
    House& grHouse7 = hs7;  // green resident lives in house 7
    House& grHouse8 = hs8;  // green resident lives in house 8
    House& blHouse16 = hs16; // blue resident lives in house 16
    House& house2    = hs2;  // next to two green residents (happiness = 0.5)
    House& house11   = hs11; // next to one blue resident + one green resident (happiness = 0.3)
    House& house12   = hs12; // next to two green + one blue residents (happiness = 0.36)

    std::set<House*> housesAdjToHouse2;
    housesAdjToHouse2.insert(&hs1);
    housesAdjToHouse2.insert(&hs3);
    housesAdjToHouse2.insert(&hs6);
    housesAdjToHouse2.insert(&hs7);
    housesAdjToHouse2.insert(&hs8);

    std::set<House*> housesAdjToHouse11;
    housesAdjToHouse11.insert(&hs5);
    housesAdjToHouse11.insert(&hs6);
    housesAdjToHouse11.insert(&hs7);
    housesAdjToHouse11.insert(&hs10);
    housesAdjToHouse11.insert(&hs12);
    housesAdjToHouse11.insert(&hs15);
    housesAdjToHouse11.insert(&hs16);
    housesAdjToHouse11.insert(&hs17);

    std::set<House*> housesAdjToHouse12;
    housesAdjToHouse12.insert(&hs6);
    housesAdjToHouse12.insert(&hs7);
    housesAdjToHouse12.insert(&hs8);
    housesAdjToHouse12.insert(&hs11);
    housesAdjToHouse12.insert(&hs13);
    housesAdjToHouse12.insert(&hs16);
    housesAdjToHouse12.insert(&hs17);
    housesAdjToHouse12.insert(&hs18);

    std::map<House*, std::set<House*>> openHousesAndTheirAdjHouses;

    openHousesAndTheirAdjHouses.insert(std::pair<House*, std::set<House*>>(
        &house12,
        housesAdjToHouse12
        )
    );

    openHousesAndTheirAdjHouses.insert(std::pair<House*, std::set<House*>>(
        &house2,
        housesAdjToHouse2)
    );

    openHousesAndTheirAdjHouses.insert(std::pair<House*, std::set<House*>>(
        &house11,
        housesAdjToHouse11)
    );

    std::map<House*, Resident*> houseToResMap;
    houseToResMap.insert(std::pair<House*, Resident*>(&grHouse7,&greenNeighbor04));
    houseToResMap.insert(std::pair<House*, Resident*>(&grHouse8,&greenNeighbor05));
    houseToResMap.insert(std::pair<House*, Resident*>(&blHouse16, &blueNeighbor01));
    
    House* answerHouse = greenResident.findBestHome(
            &oldHouse,
            openHousesAndTheirAdjHouses,
            houseToResMap
        );
    
    REQUIRE( ( 
        answerHouse == &house2 ||
        answerHouse == &house11 ||
        answerHouse == &house12
    ));

}