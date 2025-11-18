#include<stdio.h>
#include "header.h"

void displayMenu() {
    printf("\n=========================================================================================\n");
    printf("                             ICC ODI Player Performance Analyzer                         \n");
    printf("=========================================================================================\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display All Players of Specific Role Across All Teams by Performance Index\n");
    printf("6. Exit\n");
    printf("=========================================================================================\n");
}

void handleAddPlayer(Team* team) {
    int teamId;
    printf("Enter Team ID to add player: ");
    getValidTeamId(&teamId);

    if (addNewPlayerToTeam(team, teamId))
    {
        printf("\nPlayer Added Successfully to Team %s!\n", team[teamId-1].name);
    }
    else 
    {
        printf("Team Not Found.\n");
    }
}

void handleTopKPlayers(Team* team) {
    int teamId;
    printf("Enter Team ID: ");
    getValidTeamId(&teamId);

    int roleId;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    getValidRoleId(&roleId);

    PlayerRole role = (PlayerRole)roleId;

    int topCount;
    printf("Enter number of players: ");
    getValidInteger(&topCount);

    if (!getTopKPlayers(team, teamId, role, topCount))
    {
        printf("Team Not Found.\n");                
    }                                
}

void handleChoice(Team* team) {
    int choice;
    printf("\nEnter your choice: ");
    getValidInteger(&choice);

    switch(choice) 
    {
        case 1: 
            handleAddPlayer(team);
            break;

        case 2:
        {
            int teamId;
            printf("Enter Team ID: ");
            getValidTeamId(&teamId);

            if (!displayTeamData(team, teamId))
            {
                printf("Team Not Found.\n");
            }
            break;
        }

        case 3:
            printf("Teams Sorted by Average Batting Strike Rate");
            if (!displaySortedTeams(team))
            {
                printf("No teams exist.");
            }
            break;
            
        case 4:
            handleTopKPlayers(team);
            break;
        
        case 5:
        {
            int roleId;
            printf("Enter Role (1-Batsman, 2-Bowler, 3-All-Rounder): ");
            getValidRoleId(&roleId);

            PlayerRole role = (PlayerRole)roleId;
            displaySortedPlayersOfRole(team, role);
            break;
        }
        
        case 6:
            freeAllTeams(team);
            printf("Memory released. Exiting program...\n");
            exit(0);
        
        default:
            printf("Invalid Choice!\n"); 
    }  
}