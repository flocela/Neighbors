#include "ResidentsFactory_Flat.h"
#include <limits>
#include "Resident_Flat.h"

std::string ResidentsFactory_Flat::toString ()
{
    return "Flat Residents Factory";
}

std::vector<std::unique_ptr<Resident>> ResidentsFactory_Flat::createResidents (
    UI& ui, 
    int firstID,
    int count,
    Color color
)
{   
    Question_Double qHappinessGoal{
        1,
        0.0,
        1.0,
        _happinessGoalOrigPrompt,
        _happinessGoalTypePrompt,
        _happinessGoalRangePrompt
    };

    double happinessGoal = askUserForDouble(
        ui,
        qHappinessGoal,
        _happinessGoalFailure
    );

    Question_YN qHappyAtGoal {
        2,
        _happy_at_goal_orig_prompt,
        _happy_at_goal_invalid_prompt
    };

    bool happyAtGoal = askUserForBool(
        ui,
        qHappyAtGoal,
        _happy_at_goal_failure

    );
    
    return createResidents(ui, firstID, count, happinessGoal, happyAtGoal, color);
}

// This is used by ResidentsMaker_CMDLine because the game uses one happinessGoal for
// all residents. That is found out for the whole game, and then passed in here.
std::vector<std::unique_ptr<Resident>> ResidentsFactory_Flat::createResidents (
    UI& ui,
    int firstID, 
    int count, 
    double happinessGoal,
    bool happyAtGoal,
    Color color
)
{   
    Question_Double qMovement{
        2,
        0.0,
        std::numeric_limits<double>::max(),
        _movmentOrigPrompt,
        _movementTypePrompt,
        _movemenRangePrompt
    };

    Question_Double qHappinessValue{
        3,
        0.0,
        1.0,
        _happinessValueOrigPrompt,
        _happinessValueTypePrompt,
        _happinessValueRangePrompt
    };
    
    double movement = askUserForDouble(
        ui,
        qMovement,
        _movementRangeFailure
    );

    double happinessValue = askUserForDouble(
        ui,
        qHappinessValue,
        _happinessValueFailure
    );

    std::vector<std::unique_ptr<Resident>> residents = {};
    for ( int ii=0; ii<count; ++ii)
    {
        residents.push_back(
            std::make_unique<Resident_Flat>(
                firstID+ii,
                color,
                movement,
                happinessGoal,
                happyAtGoal,
                happinessValue
        ));
    }
    return residents;
}

std::vector<std::unique_ptr<Resident>> ResidentsFactory_Flat::createBaseResidents (
    UI& ui,
    int firstID, 
    int count, 
    double happinessGoal,
    bool happyAtGoal,
    Color color
)
{   
    (void) ui;
    double movement = 4;
    double happinessValue = 0.5;

    std::vector<std::unique_ptr<Resident>> residents = {};
    for ( int ii=0; ii<count; ++ii)
    {
        residents.push_back(std::make_unique<Resident_Flat>(
            firstID+ii,
            color,
            movement,
            happinessGoal,
            happyAtGoal,
            happinessValue
        ));
    }
    return residents;
}

int ResidentsFactory_Flat::askUserForInt (
    UI& ui, 
    Question_Int question, 
    std::string failureString
)
{
    ui.getAnswer(question);
    if (question.hasValidAnswer())
    {
        return std::stoi(question.getAnswer());
    }
    else
    {
        throw failureString;
    }
}

double ResidentsFactory_Flat::askUserForDouble (
    UI& ui, 
    Question_Double question, 
    std::string failureString
)
{
    ui.getAnswer(question);
    if (question.hasValidAnswer())
    {
        return std::stod(question.getAnswer());
    }
    else
    {
        throw failureString;
    }
}

bool ResidentsFactory_Flat::askUserForBool (
    UI& ui,
    Question_YN question,
    std::string failureString
)
{
    ui.getAnswer(question);
    if (question.hasValidAnswer())
    {
        return question.getAnswer() == "N" ? false : true;
    }
    else
    {
        throw failureString;
    }
}
