#include <iostream>
#include <string>
#include "menu_choices.h"
#include "system_common.h"

/**
 * @brief Constructs a new MenuChoice object.
 *
 * This constructor initializes a new MenuChoice object with the specified path to the game database. It sets the initial state of the menu to MENU_START, the running status to false, and the level to 1.
 *
 * @param path_game_db The path to the game database JSON file.
 */
MenuChoice::MenuChoice(std::string path_game_db):ManagerDBJson(path_game_db)
{
    state_ = MENU_START;
    running_ = false;
    level_ = 1;
}

/**
 * @brief Destructor for the MenuChoice class.
 *
 * This destructor is called when an instance of the MenuChoice class is destroyed. It does not perform any specific cleanup tasks, as the resources used by the class are managed by the game engine.
 *
 * @see MenuChoice
 */
MenuChoice::~MenuChoice()
{
}

/**
 * @brief Updates the state based on the user's menu choice.
 *
 * This function takes an integer input representing the user's choice from the menu
 * and updates the internal state accordingly. The valid choices and their corresponding
 * states are as follows:
 * - 1: MENU_PLAY
 * - 2: MENU_SCORE
 * - 3: MENU_GUIDE
 * - 4: MENU_EXIT
 *
 * If the user's choice does not match any of the valid options, the state is set to
 * MENU_START and an invalid choice message is printed.
 *
 * @param choice The user's menu choice as an integer.
 */
void MenuChoice::UserChoiceToState(int choice)
{
    switch (choice)
    {
        case 1:
            state_ = MENU_PLAY;
            break;

        case 2:
            state_ = MENU_SCORE;
            break;

        case 3:
            state_ = MENU_GUIDE;
            break;

        case 4:
            state_ = MENU_EXIT;
            break;

        default:
            state_ = MENU_START;
            PrintChoiceInvalid();
            break;
    }
}

/**
 * @brief Prints the main menu of the game.
 *
 * This function prints the main menu of the game, allowing the user to choose between playing a new game, viewing the score,
 * reading the guide, or exiting the game. The function uses a switch statement to handle different states of the menu.
 *
 * @param void
 * @return void
 */
void MenuChoice::PrintMenuGame(void)
{
    std::cout << "*** Menu game ***" << std::endl;
    std::cout << "1. Play new game" << std::endl;
    std::cout << "2. View Score" << std::endl;
    std::cout << "3. Guide play game" << std::endl;
    std::cout << "4. Exit game" << std::endl;
    std::cout << "Enter your choice: ";
}

/**
 * @brief Processes the main menu of the game.
 *
 * This function processes the main menu of the game, allowing the user to choose between playing a new game, viewing the score,
 * reading the guide, or exiting the game. The function uses a switch statement to handle different states of the menu.
 *
 * @param void
 * @return void
 */
void MenuChoice::MenuProcess(void)
{
    int choice = 0;

    while (state_ != MENU_END)
    {
        switch (state_)
        {
            case MENU_START:
                PrintMenuGame();
                std::cin >> choice;
                SysCmm::ClearTerminal();
                UserChoiceToState(choice);
                break;

            case MENU_PLAY:
                PrintMenuPlay();
                break;

            case MENU_SCORE:
                PrintMenuScore();
                break;

            case MENU_GUIDE:
                PrintMenuGuide();
                break;

            case MENU_EXIT:
                std::cout << "Game exit!" << std::endl;
                state_ = MENU_END;
                break;

            default:
                break;
        }
    }
}

/**
 * @brief Prints the options for playing the game.
 *
 * This function prints the options for playing the game, including starting a new game, playing with a selected difficulty, and returning to the main menu.
 *
 * @param void
 * @return void
 */
void MenuChoice::PrintMenuPlay(void)
{
    int choice = 0;
    std::cout << "*** Menu play game ***" << std::endl;
    std::cout << "1. Play game" << std::endl;
    std::cout << "2. Play the game with the selected difficulty" << std::endl;
    std::cout << "3. Back to menu" << std::endl;
    std::cout << "Enter your choice: ";

    std::cin >> choice;
    SysCmm::ClearTerminal();

    switch (choice)
    {
        case START_GAME:
            std::cout << "Game start now---------------->" << std::endl;
            running_ = true;
            state_ = MENU_END;
            break;
        
        case START_WITH_LEVEL:
            std::cout << "Start the game with the selected difficulty ---------------->" << std::endl;
            std::cout << "There are 3 difficulty levels: Easy, Medium, Hard." << std::endl;
            std::cout << "Enter a number from 1 to 3 corresponding to the 3 levels:";
            std::cin >> level_;
            SetLevelGame();
            std::cout << "Game start ---------------->" << std::endl;
            running_ = true;
            state_ = MENU_END;
            break;

        case BACK_TO_MENU_MAIN:
            state_ = MENU_START;
            break;

        default:
            PrintChoiceInvalid();
            break;
    }
}

/**
 * @brief Sets the current level of the game.
 *
 * This function sets the current level of the game. The user is prompted to choose a level from Easy, Medium, or Hard.
 *
 * @param void
 * @return void
 */
void MenuChoice::SetLevelGame(void)
{
    bool status = true;
    while (status) {
        switch (level_)
        {
            case EASY:
                std::cout << "You choice level: Easy" << std::endl;
                status = false;
                break;
            case MEDIUM:
                std::cout << "You choice level: Medium" << std::endl;
                status = false;
                break;
            case HARD:
                std::cout << "You choice level: Hard" << std::endl;
                status = false;
                break;
            default:
                std::cout << "You enter level invalid. Please enter again:";
                std::cin >> level_;
                break;
        }
    }
}

/**
 * @brief Prints an invalid choice message.
 *
 * This function outputs an error message indicating that the user's menu choice
 * is invalid and prompts the user to enter a valid choice.
 */
void MenuChoice::PrintChoiceInvalid(void)
{
    std::cout << "Your choice invalid. Please enter again!" << std::endl;
}

/**
 * @brief Get the current game state.
 *
 * This function returns the current game state, indicating whether the game is running or not.
 *
 * @return A boolean value representing the game state. True if the game is running, false otherwise.
 */
bool MenuChoice::GetGameState(void)
{
    return running_;
}

/**
 * @brief Displays the score menu and handles user choices for viewing scores.
 *
 * This function presents the user with a menu to view scores. The user can choose to:
 * - View all scores
 * - View the maximum score
 * - Return to the main menu
 */
void MenuChoice::PrintMenuScore(void)
{
    int choice = 0;
    json js_all;
    int index = 0;
    int score_hard = 0;
    int score_easy = 0;
    int score_medium = 0;
    
    std::cout << "*** Menu score ***" << std::endl;
    std::cout << "1. View all score" << std::endl;
    std::cout << "2. View maximum score" << std::endl;
    std::cout << "3. Back to menu" << std::endl;
    std::cout << "Enter your choice: ";

    std::cin >> choice;
    SysCmm::ClearTerminal();
    switch (choice)
    {
        case 1:
            js_all = ReadAllJsonFile();
            std::cout << "*** Score ***" << std::endl;
            for (auto js = js_all.begin(); js != js_all.end(); js++, index++)
            {
                std::cout << index << ". " << js.key() << std::endl;
                std::cout << "- Easy:" << std::endl;
                std::cout << "\tScore:" << js.value()["Easy"]["Score"].dump() << std::endl;
                std::cout << "\tSize:" << js.value()["Easy"]["Size"].dump() << std::endl;
                std::cout << "- Medium:" << std::endl;
                std::cout << "\tScore:" << js.value()["Medium"]["Score"].dump() << std::endl;
                std::cout << "\tSize:" << js.value()["Medium"]["Size"].dump() << std::endl;
                std::cout << "- Hard:" << std::endl;
                std::cout << "\tScore:" << js.value()["Hard"]["Score"].dump() << std::endl;
                std::cout << "\tSize:" << js.value()["Hard"]["Size"].dump() << std::endl;
            }
            std::cout << "******" << std::endl;
            std::cout << "Enter any key to back menu game ..." << std::endl;
            SysCmm::WaitForAnyKey();
            SysCmm::ClearTerminal();
            state_ = MENU_START;
            break;

        case 2:
            js_all = ReadAllJsonFile();

            for (auto js = js_all.begin(); js!= js_all.end(); js++)
            {
                for (const auto &score : js.value()["Hard"]["Score"])
                {
                    if (score_hard < score)
                    {
                        score_hard = score;
                    }
                }
                for (const auto &score : js.value()["Medium"]["Score"])
                {
                    if (score_medium < score)
                    {
                        score_medium = score;
                    }
                }
                for (const auto &score : js.value()["Easy"]["Score"])
                {
                    if (score_easy < score)
                    {
                        score_easy = score;
                    }
                }
            }
            std::cout << "*** Maximum Score ***" <<  std::endl;
            std::cout << "- Easy: " << score_easy << std::endl;
            std::cout << "- Medium: " << score_medium << std::endl;
            std::cout << "- Hard: " << score_hard << std::endl;
            std::cout << "******" << std::endl;
            std::cout << "Enter any key to back menu game ..." << std::endl;
            SysCmm::WaitForAnyKey();
            SysCmm::ClearTerminal();
            state_ = MENU_START;
            break;

        case 3:
            state_ = MENU_START;
            break;

        default:
            PrintChoiceInvalid();
            break;
    }
}

/**
 * @brief Prints the instructions for the game.
 *
 * This function prints the instructions for the game, including controls, objective, avoidance, scoring, and challenge.
 *
 */
void MenuChoice::PrintMenuGuide(void)
{
    std::cout << "*** Instructions ***" << std::endl;
    std::cout << "1. Controls: Use the arrow keys (up, down, left, right) to control the direction of the snake." << std::endl;
    std::cout << "2. Objective: Eat the food pellets to grow longer. Each pellet makes the snake one segment longer." << std::endl;
    std::cout << "3. Avoidance: Don't let the snake run into the walls or itself, or the game ends." << std::endl;
    std::cout << "4. Scoring: The longer the snake gets, the higher your score." << std::endl;
    std::cout << "5. Challenge: As the snake grows, it becomes harder to navigate without colliding with obstacles." << std::endl;
    std::cout << "******" << std::endl;
    std::cout << "Enter any key to back menu game ..." << std::endl;
    SysCmm::WaitForAnyKey();
    SysCmm::ClearTerminal();
    state_ = MENU_START;
}

/**
 * @brief Get the current level of the game.
 *
 * This function returns the current level of the game.
 *
 * @return The current level of the game.
 */
int MenuChoice::GetCurrentLevel(void)
{
    return level_;
}