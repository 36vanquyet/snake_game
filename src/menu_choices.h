#ifndef MENU_CHOICES_H
#define MENU_CHOICES_H

#include <string>
#include <fstream>
#include "manager_db.h"

typedef enum MenuState {
    MENU_START,
    MENU_PLAY,
    MENU_SCORE,
    MENU_GUIDE,
    MENU_EXIT,
    MENU_END
} MenuState_t;

typedef enum MenuGameState {
    START_GAME = 1,
    START_WITH_LEVEL = 2,
    BACK_TO_MENU_MAIN = 3
} MenuGameState_t;

typedef enum GameLevel {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3,
} GameLevel_t;

class MenuChoice : public ManagerDBJson {
    public:
        MenuChoice(std::string path_game_db);
        ~MenuChoice();
        void MenuProcess(void);
        bool GetGameState(void);
        int GetCurrentLevel(void);

    private:
        MenuState_t state_;
        int level_;
        bool running_;
        void PrintChoiceInvalid(void);
        void UserChoiceToState(int choice);
        void PrintMenuGame(void);
        void PrintMenuScore(void);
        void PrintMenuGuide(void);
        void PrintMenuPlay(void);
        void SetLevelGame(void);
};

#endif /* MENU_CHOICES_H */