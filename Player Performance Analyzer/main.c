#include "header.h"

int main() {
    Team* team = initialiseTeams();
    if (team == NULL)
    {
        printf("Initialisation of teams Failed!");
        return 0;
    }

    if (!initialisePlayers(team))
    {
        printf("Initialisation of players Failed!");
        return 0;
    }

    while(true)
    {
        displayMenu();
        handleChoice(team);
    }
}