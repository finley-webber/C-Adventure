#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* -----------------------------------------------------------------------------------------------------------------------------------
Challenge (I didn't strictly adhere to this but kept in the general guidelines):

You're an explorer who has entered a mysterious cave searching for a hidden treasure. The cave has multiple rooms and passages.
 Some rooms might have clues, some have challenges, and others might be empty or have traps.
Requirements:

Create a structure (struct) for each room. Each room should have:
    A name
    Options for where the player can go next.
    Any special items or challenges in the room.
    Whether the room has been visited before or not.

Implement a health system. The player starts with 100 health, and some rooms or challenges can decrease the player's health.

Implement an inventory system using an array. The player can pick up or use items.

Implement random challenges in certain rooms, like riddles or number guessing games. 

Successfully navigating these challenges might reward the player with items or health.

The game should have at least one ending scenario, such as finding the treasure or being trapped forever.

Bonus Features:

Create a map system where players can see a representation of rooms they've already visited.

Implement more complex challenges or puzzles.

Add a save and load game feature, allowing the player to save their progress to a file and load it back.

Implement an enemy or creature in some rooms that the player must avoid or deal with.
*/

char* k = "[+]";
char* i = "[*]";
char* e = "[-]";
char* border = "---------------------------------------------------------------------------------------------------------------------------------------------";

int health = 100;
int player_location_x = 0;
int player_location_y = 0;
int gameFinished = 0;
int isPlayerAttacking = 0;

int movement[8][2] = {
    {-1,-1},
    {0,-1},
    {1,-1},
    {1, 0},
    {1,1},
    {0,1},
    {-1,1},
    {-1,0},
};

char *firstWord[] = {
    "Chamber ",
    "Hall ",
    "Room ",
    "Sanctuary ",
    "Dungeon ",
    "Vault ",
    "Tower ",
    "Grotto ",
    "Cellar ",
    "Lair "
};

char *middleWord[] = {
    "of ",
    "for ",
    "with ",
    "beyond ",
    "under ",
    "against ",
    "beside ",
    "without ",
    "within ",
    "towards "
};

char *lastWord[] = {
    "Death",
    "Secrets",
    "Dreams",
    "Whispers",
    "Shadows",
    "Mysteries",
    "Eternity",
    "Ghosts",
    "Echoes",
    "Legends"
};

char *enemyTypes[] = {
    "Goblins",
    "Skeletons",
    "Tauren"
};

char *objType[3][5] = {
    {"Shield", "Broken Shield", "Lesser Shield", "Standard Shield", "Great Shield" },
    {"Healing item", "Stimulant", "Bandage", "Potion", "Potion of Life"},
    {"Weapon", "Withered Sword", "Unsharpened Sword", "Sword", "Enchanted Sword"},
};

/*
Objects that the player can pick up and use for various purposes
*/

struct obj {
    char type[30];
    char name[60];
    int damage;
    int defense;
};

struct obj inventory[8] = {};

/*
Enemies that the player can find scattered throughout each room
*/

struct enemy {
    char *type;
    int health;
    int damage;
    int defense;
};

/*
The rooms that the player navigates throughout the map
*/

struct room {
    char name[30];
    int enemies;
    struct enemy *enemyType;
    struct obj *object;
    int roomChecked;
    int roomVisited;
    int location_x;
    int location_y;
};

/*
The map of the cave
*/

/*

*/

struct map {
    struct room ***rooms;
    int size;
};

void showInventory () {
    printf("%s\n", border);
    for (int i = 0; i < 8; i++) {
        printf("Item %i: %s\n", i + 1, inventory[i].name);
        if (strcmp(inventory[i].name, "Empty") == 0) {
        } else {
            printf("\t- Type: %s\n", inventory[i].type);
            printf("\t- Damage: %i\n", inventory[i].damage);
            printf("\t- Defense: %i\n", inventory[i].defense);
        }
    }
}

struct obj heal(struct obj inventory[8]) {
    printf("Choose your healing item:\n");
    showInventory();

    int ch;
    int valid = 0;
    char choice;

    do {
        while((ch = getchar()) != '\n' && ch != EOF);
        choice = getchar();
        choice = choice - '0';
        choice--;
        if (strcmp(inventory[choice].name, "Empty") == 0 || choice < 0 || choice > 7) {
            printf("Chosen: %s\n", inventory[choice].name);
            printf("Not a valid choice.\n");
        } else {
            valid = 1;
        }
    } while(!valid);

    for (int i = 1; i < 5; i++) {
        if (strcmp(objType[1][i], inventory[choice].name) == 0) {
            health = health + (10 * i);
            if (health > 100) {
                health = 100;
            }
            printf("Health is now at %i\n", health);
            strcpy(inventory[choice].name, "Empty");
            strcpy(inventory[choice].type, "Empty");
            break;
        }
    }

    return *inventory;
}

void endGame(){
    gameFinished = 1;
    printf("GAME OVER!\n");
}

int damageCalc(int inflicted, int defense) {
    int damageTaken;

    if (defense >= inflicted) {
        damageTaken = 0;
    } else {
        damageTaken = (int) inflicted * (1 - (double)defense/100);
    }

    return damageTaken;
}

struct room attack(struct room currRoom, struct map map, int enemyHealthAtStart) {
    if (isPlayerAttacking == 1) {

        printf("You plan your attack against the enemy \nWhat will you strike with?\n");
        showInventory();
        printf("%s\n", border);
        int ch;
        char choice;
        int valid = 0;
        do {
            while ((ch = getchar()) != '\n' && ch != EOF);
            choice = getchar();
            choice = choice - '0';
            choice--;
            if (choice < 0 || choice > 7) {
                printf("Chosen: %s\n", inventory[choice].name);
                printf("Not a valid choice.\n");
            } else {
                valid = 1;
            }
        } while (!valid);
        int damageTaken;
        if (strcmp(inventory[choice].name, "Empty") == 0) {
            damageTaken = damageCalc(0, currRoom.enemyType->defense);
        } else {
            damageTaken = damageCalc(inventory[choice].damage, currRoom.enemyType->defense);
        }
        currRoom.enemyType->health = currRoom.enemyType->health - damageTaken;
        if (currRoom.enemyType->health <= 0) {
            currRoom.enemyType->health = enemyHealthAtStart;
            printf("You attack it with your %s, dealing %i damage, and killing it in the process.\n", inventory[choice].name, damageTaken);
            currRoom.enemies--;
        } else {
            currRoom.enemyType->health -= damageTaken;
            printf("You attack it with your %s, dealing %i damage, and leaving it with %i hitpoints left.\n", inventory[choice].name, damageTaken, currRoom.enemyType->health);
        }
        isPlayerAttacking = 0;
    } else {
        int eDamage = currRoom.enemyType->damage;
        printf("It attacks you, what do you defend yourself with?\n");
        showInventory();
        /*
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        char choice = getchar();
        choice = choice - '0';
        if (strcmp(inventory[choice].name, "Empty") == 0 || choice < 0 || choice > 7) {
            printf("Not a valid choice.\n");
            currRoom = attack(currRoom, map, enemyHealthAtStart);
        }
        */
        int ch;
        char choice;
        int valid = 0;
        do {
            while ((ch = getchar()) != '\n' && ch != EOF);
            choice = getchar();
            choice = choice - '0';
            choice--;

            if (choice < 0 || choice > 7) {
                printf("Chosen: %i\n", choice);
                printf("Not a valid choice.\n");
            } else {
                valid = 1;
            }
        } while (!valid);
        int damageTaken;
        if (strcmp(inventory[choice].name, "Empty") == 0) {
            damageTaken = damageCalc(eDamage, 0);
        } else{
            damageTaken = damageCalc(eDamage, inventory[choice].defense);
        }
        health = health - damageTaken;
        printf("As it attacks you, you block it's attack with your %s, \nthe attack did a hefty %i damage to your health, leaving you with %i hitpoints left,\n", inventory[choice].name, damageTaken, health);
        if (health <= 0) {
            endGame();
        }
        isPlayerAttacking = 1;
    }

    return currRoom;
}

struct room attackEnemies(struct room currRoom, struct map map) {
    int enemyHealthAtStart = currRoom.enemyType->health;
    printf("You march over to one of the %s\n", currRoom.enemyType->type);
    
    while(currRoom.enemies > 0 && gameFinished != 1) {
        currRoom = attack(currRoom, map, enemyHealthAtStart);
    };

    if (gameFinished != 1) {
        currRoom.enemyType->type = "DEAD";
        currRoom.enemyType->health = 0;
        currRoom.enemyType->damage = 0;
        currRoom.enemyType->defense = 0;
    }

    return currRoom;
}

struct room discardObject(struct room currRoom, struct map map) {

    if (strcmp(currRoom.object->name, "Looted!") != 0) {
        printf("This room already has an object in it. \nAny object discarded by you will be destroyed!\n");
        printf("Do you wish to proceed? (y/n)\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        char choice = getchar();
        if (choice == 'y') {
        } else {
            return currRoom;
        }
    }

    int counter = 1;
    int availableItems[8] = {11, 11, 11, 11, 11, 11, 11, 11};

    printf("Select which object you would like to get rid of: \n");
    for (int i = 0; i < 8; i++) {
        if (strcmp(inventory[i].name, "Looted!") == 0 || strcmp(inventory[i].name, "Empty") == 0) {
        } else {
            printf("%i): %s\n", counter, inventory[i].name);
            availableItems[counter - 1] = i;
            counter++;
        }
    }

    int ch;
    char choice;
    int valid = 0;

    do {
        while ((ch = getchar()) != '\n' && ch != EOF);
        choice = getchar();
        choice = choice - '0';
        choice--;
        if (choice > counter || choice < 0 || availableItems[choice] == 11) {
            printf("Not a valid input. Try again\n");
        } else {
            valid = 1;
        }
    } while (!valid);

    int item = availableItems[choice];
    printf("Discarded Object: %s\n",inventory[item].name);

    if (strcmp(currRoom.object->name, "Looted!") != 0) {
    } else {
        strcpy(currRoom.object->name, inventory[item].name);
        strcpy(currRoom.object->type, inventory[item].type);
        currRoom.object->damage = inventory[item].damage;
        currRoom.object->defense = inventory[item].defense;
    }
    strcpy(inventory[item].name, "Empty");
    strcpy(inventory[item].type, "Empty");
    inventory[item].damage = 0;
    inventory[item].defense = 0;

    return currRoom;
}

struct obj setupInventory(struct obj inv[8]) {
    for (int i = 0; i < 8;i++) {
        strcpy(inv[i].name, "Empty");
        strcpy(inv[i].type, "Empty");
        inv->damage = 0;
        inv->defense = 0;
    }
    return *inv;
};

struct obj createObject() {
    struct obj* newObject = (struct obj*) malloc(sizeof(struct obj));
    int oType = rand() % 3;
    int oName = (rand() % 4) + 1;
    
    strcpy(newObject->type, objType[oType][0]);
    strcpy(newObject->name, objType[oType][oName]);

    newObject->damage = pow(oName, 2) * ((oType - 1) * 10);
    if (newObject->damage < 0) {
        newObject->damage = 0;
    }
    newObject->defense = ((-1 * oType) * 1000) + 5 * pow(oName, 2);
    if (newObject->defense < 0) {
        newObject->defense = 0;
    }
    return *newObject;
}

struct room setupRoom(struct map* map, int x_loc, int y_loc) {
    struct room* newRoom = (struct room*) malloc(sizeof(struct room));

    int fw = rand() % 8;
    int mw = rand() % 8;
    int lw = rand() % 8;

    strcpy(newRoom->name, "The ");
    strcat(newRoom->name, firstWord[fw]);
    strcat(newRoom->name, middleWord[mw]);
    strcat(newRoom->name, lastWord[lw]);

    struct enemy *newEnemy = (struct enemy*) malloc((sizeof(struct enemy)));
    int eName = rand() % 3;
    newEnemy->type = enemyTypes[eName];
    newEnemy->health = pow((eName + 1), 2) * 15;
    newEnemy->damage = pow((eName + 1), 2) * 10;
    newEnemy->defense = pow((eName + 1), 2) * 5;

    newRoom->enemyType = newEnemy;

    if (strcmp(newRoom->enemyType->type, "Tauren") == 0) {
        newRoom->enemies = 1;
    } else {
        newRoom->enemies = rand() % 5;
    }

    newRoom->object = malloc(sizeof(struct obj));
    *(newRoom->object) = createObject();

    newRoom->roomChecked = 0;
    newRoom->roomVisited = 1;

    // IMPORTANT: Without this, the map won't have a room to put information in
    map->rooms[x_loc][y_loc] = newRoom;

    printf("%s\n", border);
    printf("As you enter this room of the cave, \nyou observe an engraving upon the wall that reads:\n%s\n", map->rooms[x_loc][y_loc]->name);
    printf("%s\n", border);

    map->rooms[x_loc][y_loc]->location_x = x_loc;
    map->rooms[x_loc][y_loc]->location_y = y_loc;

    /*
        CASE STUDY:
        I was wondering why `newRoom->object->name` was being altered when I changed `map->rooms[x_loc][y_loc]->object->name` variable and it's because (or at least
        I believe it's because) that since I returned the object as a pointer (from `createObject()`), then both the `newRoom` variable and the `map->rooms...`
        variable share the same memory address, meaning that if I change something in one variable, it will also change it for the other variable.
    */
    //strcpy(map->rooms[x_loc][y_loc]->object->name, "");
    //strcat(map->rooms[x_loc][y_loc]->object->name, "Unknown");

    return *newRoom;
}

int sizeCheck (void) {

    int num = 0;

    printf("How big do you want the map?\nRecommended: 5+\nLimit: 31\n");
    scanf("%i", &num);
    if (num > 31 || num <= 1) {
        printf("-----------------\n");
        printf("Not a valid size\n");
        printf("-----------------\n");
        sizeCheck();
    }

    return num;
}

void checkMap (struct room currRoom, struct map map) {
    for (int i = -1; i < 2; i++){
        for (int n = -1; n < 2; n++) {
            if (player_location_x + i < 0 || player_location_y + n < 0 || player_location_x + i >= map.size || player_location_y + n >= map.size) {
            } else {
                printf("%s\n", border);
                if (map.rooms[player_location_x + i][player_location_y + n]->roomVisited == 1) {
                    printf("Room Name: %s ||| ", map.rooms[player_location_x + i][player_location_y + n]->name);
                } else {
                    printf("Room Name: Unknown ||| ");
                }
                printf("Location: %ix%i ||| ", player_location_x + i, player_location_y + n);
                if (map.rooms[player_location_x + i][player_location_y + n]->roomChecked == 0) {
                    printf("Object: Unknown ||| ");
                    printf("Enemy Type: Unknown ||| ");
                    printf("Amount of enemies: Unknown\n");
                } else {
                    printf("Object: %s ||| ", map.rooms[player_location_x + i][player_location_y + n]->object->name);
                    if (map.rooms[player_location_x + i][player_location_y + n]->enemies != 0) {
                        printf("Enemy Type: %s ||| ", map.rooms[player_location_x + i][player_location_y + n]->enemyType->type);
                    }
                    printf("Amount of Enemies: %i\n", map.rooms[player_location_x + i][player_location_y + n]->enemies);
                }
                printf("%s\n", border);
            }
        }
    }
    
}


struct room pickUpObject(struct room currRoom, struct map map) {
    for (int i = 0; i < 8; i++) {
        if (strcmp(inventory[i].name, "Empty") == 0 && strcmp(currRoom.object->name, "Looted!") != 0) {
            strcpy(inventory[i].name, currRoom.object->name);
            strcpy(inventory[i].type, currRoom.object->type);
            inventory[i].damage = currRoom.object->damage;
            inventory[i].defense = currRoom.object->defense;

            strcpy(currRoom.object->name, "Looted!");
            strcpy(currRoom.object->type, "Looted!");
            currRoom.object->damage = 0;
            currRoom.object->defense = 0;
        }
    }

    return currRoom;

}

struct room checkRoom(struct room currRoom, struct map** map) {

    (**map).rooms[player_location_x][player_location_y]->enemyType = currRoom.enemyType;
    (**map).rooms[player_location_x][player_location_y]->object = currRoom.object;
    (**map).rooms[player_location_x][player_location_y]->enemies = currRoom.enemies;
    (**map).rooms[player_location_x][player_location_y]->roomChecked = 1;

    printf("%s\n", border);
    if (currRoom.enemies > 1) {
        printf("You look around and you see %i figures standing around. \nThey appear to be %s\n", currRoom.enemies, currRoom.enemyType->type);
    } else if (currRoom.enemies == 1 && strcmp(currRoom.enemyType->type, "Tauren") == 0) {
        printf("You look around and you see a figure standing around. \nIt's a Tauren!\n");
    } else if (currRoom.enemies == 1) {
        printf("You look around and you see a figure standing around. \nThey appear to be one of the %s\n", currRoom.enemyType->type);
    }
    printf("%s\n", border);
    printf("After looking around the room,\nyou notice a %s on the floor.\nYou pick it up and examine it and you believe it's a\n%s.\n", currRoom.object->type, currRoom.object->name);
    printf("%s\n", border);

    currRoom.roomChecked = 1;

    return currRoom;
}

int doorCheck(int x, int y, struct map map) {
    if (x < 0 || y < 0 || x >= map.size || y >= map.size) {
        printf("No door leads there\n");
        return 1;
    } else {
        return 0;
    }
}

struct room changeRoom(struct room currRoom, struct map map) {
    int ch;
    int valid = 0;
    int potential_x;
    int potential_y;
    do {
        printf("%s\n", border);
        printf("Where do you want to go?\n");
        printf("0): North West \n1): North \n2): North East \n3): East \n4): South East \n5): South \n6): South West \n7): West \n");
        while ((ch = getchar()) != '\n' && ch != EOF);
        char choice = getchar();
        choice = choice - '0';

        potential_x = player_location_x + movement[choice][0];
        potential_y = player_location_y + movement[choice][1];

        int check = doorCheck(potential_x, potential_y, map);

        if (check == 0) {
            valid = 1;
        }
    } while (!valid);

    player_location_x = potential_x;
    player_location_y = potential_y;

    if (strcmp(map.rooms[potential_x][potential_y]->name, "Undiscovered") == 0) {
        currRoom = setupRoom(&map, player_location_x,player_location_y);
    } else {
        currRoom = *map.rooms[player_location_x][player_location_y];
        printf("%s\n", border);
        printf("As you walk towards the door, you notice an engraving above it. It reads: %s\n", currRoom.name);
    }

    return currRoom;
};

struct room options(struct room currRoom, struct map* map, struct obj inv[8]) {

    printf("Here are your options:\n");
    printf("%s\n", border);

    printf("a): Look around room\n");

    if (currRoom.roomChecked == 1 && strcmp(currRoom.object->name, "Looted!") != 0) {
        printf("b): Pick up %s\n", currRoom.object->name);
    }
    printf("c): Exit Room\n");

    if (currRoom.roomChecked == 1 && strcmp(currRoom.enemyType->type, "DEAD") != 0) {
        printf("d): Attack the %s\n", currRoom.enemyType->type);
    }

    printf("e): Check the map\n");

    printf("f): Check inventory\n");

    printf("g): Discard Object\n");

    printf("h): Heal\n");

    printf("i): Exit Dungeon\n");

    printf("%s\n",border);

    printf("Your choice: ");

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    char choice = getchar();

    choice = tolower(choice);

    if (choice == 'a'){
        currRoom = checkRoom(currRoom, &map);
    } else if (choice == 'b' && strcmp(currRoom.object->name, "Looted!") != 0) {
        currRoom = pickUpObject(currRoom, *map);
    } else if (choice == 'e'){
        checkMap(currRoom, *map);
    } else if (choice == 'c') {
        currRoom = changeRoom(currRoom, *map);
    } else if (choice == 'f') {
        showInventory();
    } else if (choice == 'g') {
        discardObject(currRoom, *map);
    } else if (choice == 'd' && strcmp(currRoom.enemyType->type, "DEAD") != 0) {
        currRoom = attackEnemies(currRoom, *map);
    } else if (choice == 'h') {
        *inv = heal(inv);
    } else if (choice == 'i') {
        endGame();
    }

    printf("%s\n", border);

    return currRoom;
}

/*

*/

struct map makeMap(void) {

    struct map map;

    map.size = sizeCheck();

    map.rooms = (struct room***) malloc(sizeof(struct room**) * map.size);
    if(map.rooms == NULL) {
        printf("%s Failed to create array\n", e);
    }

    for (int n = 0; n < map.size; n++) {
        map.rooms[n] = (struct room**) malloc(sizeof(struct room*) * map.size);
    }

    struct obj undiscoveredObject;
    strcpy(undiscoveredObject.name, "Unknown");
    strcpy(undiscoveredObject.type, "Unknown");


    for (int i = 0; i < map.size; i++) {
        for (int u = 0; u < map.size; u++) {
            map.rooms[i][u] = (struct room*) malloc(sizeof(struct room) * 500);
            strcpy(map.rooms[i][u]->name, "Undiscovered");
            map.rooms[i][u]->object = &undiscoveredObject;
            map.rooms[i][u]->roomChecked = 0;
        }
    }
    return map;
}

int main (void) {
    srand(time(NULL));

    *inventory = setupInventory(inventory);
    
    struct map map = makeMap();
    printf("%s\n", border);
    printf("As dawn's first light kissed the horizon, \nyou find yourself at the entrance of the enigmatic Whispering Caves, \na place the villagers speak of only in hushed tones. \nLegends say that deep within its twisting labyrinth lies a treasure beyond imagination: \nthe lost Relic of Elysium. \nBut many who sought its power never returned. \nWhispers of treacherous traps, cryptic riddles, and rooms imbued with ancient magic abound.\nWith a mixture of excitement and trepidation, you light your lantern. \nThe entrance beckons, darkness yawning wide. \nThe cold air tastes of damp stone and age-old secrets. \nYour heart races as you step into the abyss, with nothing but your wits and the items you carry.\nWill you emerge as the legendary explorer who finally uncovers the Relic of Elysium? \nOr will the caves claim you, as they have claimed so many before?\nThe choice is yours, adventurer. \nLet the journey begin.\n");
    printf("%s\n", border);

    struct room currentRoom = setupRoom(&map, player_location_x, player_location_y);

    while (gameFinished != 1){
        currentRoom = options(currentRoom, &map, inventory);
    }
    for (int i = 0; i < map.size; i++) {
        for (int u = 0; u < map.size;u++) {
                free(map.rooms[i][u]->enemyType);

                free(map.rooms[i][u]);
        }
        free(map.rooms[i]);
    }


    return 0;
}

