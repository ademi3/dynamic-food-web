
/*------------------------------------------------
Program 2: Dynamic Food Web Creation
Course: CS211, Fall 2024, UIC
Author: Alexandru Demidetchi
------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices  
    int numPrey;
} Org;

// this function dynamically adds a new organism to the food web
// parameters: ppWeb is a pointer to a dynamically allocated array of Orgs, pNumOrg is passed-by-pointer and keeps track of the size of the web, newOrgName is the character array for the name of the new organism
void addOrgToWeb(Org** ppWeb, int* pNumOrg, char* newOrgName) {
    
   // if the web is empty, space is allocated on the heap for a single Org and the number of organisms is updated to 1
   if (*pNumOrg == 0) { // checking if the web is empty

    *ppWeb = NULL;
    *ppWeb = (Org*)malloc(sizeof(Org));
    *pNumOrg = 1;
   }
    // if the web is not empty, a new array is dynamically allocated to hold the new Org along with the old ones
   else {
    Org *newWeb = (Org*)malloc((*pNumOrg + 1) * sizeof(Org));

    // copying the old Orgs to the new array
    for (int i = 0; i < *pNumOrg; ++i) { 
        newWeb[i] = (*ppWeb)[i];
    }
    free(*ppWeb); // freeing the memory associated with the original web since we only need the new web now
    *ppWeb = newWeb; // reassigning the array pointer to the new web
    (*pNumOrg)++; // updating the count of the total number of Orgs in the web
   }

    strcpy((*ppWeb)[*pNumOrg - 1].name, newOrgName); // the name of the newly added Org is appended to the end of the pWeb array

    (*ppWeb)[*pNumOrg - 1].prey = NULL; // the prey subitem of the new Org is set to NULL
    (*ppWeb)[*pNumOrg - 1].numPrey = 0; // the numPrey subitem is initialized to 0

}


// this function dynamically adds a predator-prey relation to the web
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array, predInd is the predator index at which a prey[] subitem will be added 
// parameters: preyInd is a prey index that will be added to the prey[] subitem at predInd
// returns a bool which indicates if the new relation was added successfully or not (true = successful, false = unsuccessful)
bool addRelationToWeb(Org* pWeb, int numOrg, int predInd, int preyInd) {

    // checking if both the predator and prey index values are valid; if not, false is returned
    if (predInd < 0 || preyInd < 0 || predInd >= numOrg || preyInd >= numOrg) {

        printf("Invalid predator and/or prey index. No relation added to the food web.");
        return false;
    }
    // checking if the predator/prey relation is already part of the web; if so, false is returned
    for (int i = 0; i < pWeb[predInd].numPrey; i++) {
        if (pWeb[predInd].prey[i] == preyInd) {
            return false;
        }
    }

    // if the predator array is empty, space is allocated on the heap for a single prey subitem and the subitem is initialized
    if (pWeb[predInd].numPrey == 0) {
        pWeb[predInd].prey = (int*)malloc(sizeof(int));
        pWeb[predInd].prey[0] = preyInd; // adding the prey to the start of the predator's prey array
        pWeb[predInd].numPrey = 1;
    }
    // if the predator array is not empty, a new prey array is dynamically allocated to hold the new prey, along with the old ones
    else {
        int *newPreyArray = (int*)malloc((pWeb[predInd].numPrey + 1) * sizeof(int));

        // copying the old prey elements to the new array
        for (int i = 0; i < pWeb[predInd].numPrey; i++) {
            newPreyArray[i] = pWeb[predInd].prey[i];
        }
        newPreyArray[pWeb[predInd].numPrey] = preyInd; // appending the index of the new prey element to the end of the newPreyArray
        
        free(pWeb[predInd].prey); // freeing the memory associated with the original prey array since we only need the updated prey array now
        pWeb[predInd].prey = newPreyArray; // reassigning the array pointer to the updated array
        pWeb[predInd].numPrey++;

    }
    return true;

}


// this function removes an organism from the web at a specific index
// parameters: ppWeb is a pointer to a dynamically allocated array of Orgs, pNumOrg is passed-by-pointer and keeps track of the size of the web 
// parameters: index represents the position in the web for an organism to be removed
// returns a bool which indicates if the organism was successfully removed from the web (true = successful, false = unsuccessful)
bool removeOrgFromWeb(Org** ppWeb, int* pNumOrg, int index) {

    // verifying that the index to be removed is valid
    if (index < 0 || index >= *pNumOrg) {
        printf("Invalid extinction index. No organism removed from the food web.");
        return false;
    }

    free((*ppWeb)[index].prey); // freeing the allocated memory for the Org's prey array before the Org is removed from the web
    Org *updatedWeb = (Org*)malloc((*pNumOrg - 1) * sizeof(Org)); // allocating space for the new array with the updated number of organisms

    // this loop copies all of the Orgs except the one that needs to be removed
    for (int org = 0; org < *pNumOrg - 1; ++org) {
        // if the current index is greater than or equal to the index at which the Org needs to be removed, the elements are copied into the array, so that they
        // skip the index that needs to be removed and append the rest
        if (org >= index) {
            updatedWeb[org] = (*ppWeb)[org + 1];
        }
        // when the current index is less than index, the old array elements are copied to the new array directly
        else {
            updatedWeb[org] = (*ppWeb)[org];
        }
    }

    free(*ppWeb); // freeing the memory associated with the original array since only the updated array is needed now
    *ppWeb = updatedWeb; // adjusting the array pointer to point to the updated array
    (*pNumOrg)--; // decrementing the count of the total number of Orgs in the array
    
    // handling the case where the food web only has one organism and it goes extinct
    if (*pNumOrg == 0) {
        *ppWeb = NULL;
    }

    // looping through each organism in the web
    for (int org = 0; org < *pNumOrg; ++org) {

        // looping through the current Org's prey array
        for (int currPrey = 0; currPrey < (*ppWeb)[org].numPrey; ++currPrey) {
            if ((*ppWeb)[org].prey[currPrey] == index) { // checking if the current Org has the removed Org as one of its prey
                int *updatedPrey = (int*)malloc(((*ppWeb)[org].numPrey - 1) * sizeof(int)); // allocating space for the new prey array with the updated number of prey

                int updatedArrayInd = 0;
                // once again looping through the current Org's prey array
                for (int preyInd = 0; preyInd < (*ppWeb)[org].numPrey; ++preyInd) {

                    // if the current prey element does not equal the index that needs to be removed, then the prey element is copied directly into the new array
                    if ((*ppWeb)[org].prey[preyInd] != index) {
                        updatedPrey[updatedArrayInd++] = ((*ppWeb)[org].prey[preyInd]); // incrementing updatedArrayInd ensures the next valid prey element will be added in the correct position
                    } 
                }
                free((*ppWeb)[org].prey); // freeing the memory associated with the original prey array

                // handling the case where a prey array becomes empty due to the extinction of its only prey
                if (updatedArrayInd == 0) {
                    (*ppWeb)[org].prey = NULL;
                    (*ppWeb)[org].numPrey = 0;
                }
                else { 
                    (*ppWeb)[org].prey = updatedPrey; // updating the prey pointer to point to the updated array
                    (*ppWeb)[org].numPrey--; // decrementing the prey count of the array
                }
            }
        }
        // looping through each prey element for the current Org
        for (int preyInd = 0; preyInd < (*ppWeb)[org].numPrey; ++preyInd) {
            // checking if the current prey index is greater than the index of the Org that has been removed
            if ((*ppWeb)[org].prey[preyInd] > index) {
                (*ppWeb)[org].prey[preyInd]--; // since the organism has been removed, the current prey's index has to be shifted down to remain valid with the other indices
            }
        }
    }
    return true;
}


// this function frees all the allocated memory associated with the pWeb array in order to prevent potential leaks within the program
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void freeWeb(Org* pWeb, int numOrg) {
    
    // looping through each Org in the web
    for (int i = 0; i < numOrg; ++i) {
        
        // checking if the prey array subitem for the current Org is empty; if not, the allocated memory associated with the prey array is freed
        if (pWeb[i].prey != NULL) { 
            free(pWeb[i].prey);
        }
    }
    free(pWeb); // freeing the memory allocated for the actual pWeb array
}


// this function prints all of the organisms within the food web along with the list of their prey
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printWeb(Org* pWeb, int numOrg) {

    // looping through each predator in the web
    for (int predInd = 0; predInd < numOrg; ++predInd) {

        // if the predator does not have any prey, then only the name of the predator is printed
        if (pWeb[predInd].numPrey == 0) {
            printf("  (%d) %s\n", predInd, pWeb[predInd].name);
        }
        // if the predator has at least one prey, the print statement will contain the names of the prey
        if (pWeb[predInd].numPrey > 0) {
            printf("  (%d) %s eats", predInd, pWeb[predInd].name);

            // looping through the prey list for a given predator
            for (int preyInd = 0; preyInd < pWeb[predInd].numPrey; preyInd++) {
                printf(" %s", pWeb[pWeb[predInd].prey[preyInd]].name); // printing the name of the prey

                // condition to prevent the last prey element from printing a comma
                if (preyInd < pWeb[predInd].numPrey - 1) {
                    printf(",");
                }
            }
            printf("\n");
        }
    }
}


// this function prints the apex predator(s) from within the web; an apex predator is an organism that is not the prey of any other organism in the food web
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printApexPredators(Org *pWeb, int numOrg) {

    // looping through each predator in the food web
    for (int predInd = 0; predInd < numOrg; ++predInd) {
        bool apexPred = true; // bool that indicates whether a predator is an apex predator or not

        // looping to compare the current predator against all the predators
        for (int predIndToCompare = 0; predIndToCompare < numOrg; ++predIndToCompare) {

            if (predInd == predIndToCompare) { // skipping the case where the two predators are the same
                continue;
            }
            // looping through the prey elements of the predator that is being compared
            for (int preyInd = 0; preyInd < pWeb[predIndToCompare].numPrey; ++preyInd) {
                // checking if the current predator is one of the prey elements of the predator that is being compared
                if (pWeb[predIndToCompare].prey[preyInd] == predInd) {
                    apexPred = false; // the predator is not an apex predator
                    break;
                }
            }
        }
        // name of the current predator is printed if it is an apex predator
        if (apexPred) {
            printf("  %s\n", pWeb[predInd].name);
        }
    }
}


// this function prints the producer(s) from within the web; a producer is an organism that does not eat any other organism in the food web (a producer is typically a plant)
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printProducers(Org *pWeb, int numOrg) {

    // looping through each (potential) predator in the food web
    for (int predInd = 0; predInd < numOrg; ++predInd) {

        // if the current organism doesn't have any prey, then the organism is a producer and its name is printed
        if (pWeb[predInd].numPrey == 0) {
            printf("  %s\n", pWeb[predInd].name);
        }
    }
}


// this function prints the most flexible eater(s) from within the web; a flexible eater is an organism that eats many different organisms in the food web
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printMostFlexibleEaters(Org *pWeb, int numOrg) {

    int numMostPrey = 0; // variable to holds the max number of prey found
    char *mostFlexibleEaters[40]; // array that will hold the eater(s) that are the most flexible
    int numFlexibleEaters = 0; // variable to keep track of the size of the array
    
    // looping through each predator in the food web
    for (int predInd = 0; predInd < numOrg; ++predInd) {
        
        // checking if the current predator has more prey than the current max number of prey 
        if (pWeb[predInd].numPrey > numMostPrey) {
            numMostPrey = pWeb[predInd].numPrey; // updating the max num
            numFlexibleEaters = 0; // resetting numFlexibleEaters since a new most flexible eater has been found and the previous entries to the array are not needed
            mostFlexibleEaters[numFlexibleEaters++] = pWeb[predInd].name; // adding the name of the new most flexible eater to the array
        }
        // checking if there is a predator whose prey count is equal to the max
        else if (pWeb[predInd].numPrey == numMostPrey) {
            mostFlexibleEaters[numFlexibleEaters++] = pWeb[predInd].name; // adding that predator to the array
        }
    }
    // the names of the most flexible eaters are printed
    for (int i = 0; i < numFlexibleEaters; ++i) {
        printf("  %s\n", mostFlexibleEaters[i]);
    }
}


// this function prints the most tastiest prey from within the web; a prey is characterized as "tasty" if it is eaten by many different organisms in the food web
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printTastiestFood(Org *pWeb, int numOrg) {
    
    int numMostEatenFood = 0; // variable to hold the food which is eaten most by predators
    char *mostTastiestFoods[40]; // array that will hold the most tastiest food(s)
    int numTastyFoods = 0; // variable to keep track of the of the size of the array
    int tastyFoodCounts[numOrg]; // array to keep track of how many times each organism is found to be a prey (food)

    // loop to initialize all elements of the tastyFoodCounts array to 0
    for (int org = 0; org < numOrg; ++org) {
        tastyFoodCounts[org] = 0;
    }

    // looping through each predator in the food web
    for (int predInd = 0; predInd < numOrg; ++predInd) {
        // looping through each prey of the current predator
        for (int preyInd = 0; preyInd < pWeb[predInd].numPrey; ++preyInd) {
            int foodInd = pWeb[predInd].prey[preyInd]; // foodInd represents the index of an organism that has been found to be a prey
            tastyFoodCounts[foodInd]++; // that prey's count in the tastyFoodCounts array increases by 1 
        }  
    }
    // looping through each organism once more
    for (int foodInd = 0; foodInd < numOrg; ++foodInd) {
        // checking if the current organism is more tasty than the most tastiest organism found so far
        if (tastyFoodCounts[foodInd] > numMostEatenFood) {
            numMostEatenFood = tastyFoodCounts[foodInd]; // updating the max num
            numTastyFoods = 0; // numTastyFoods is reset to 0 since a new most tastiest food has been found
            mostTastiestFoods[numTastyFoods++] = pWeb[foodInd].name; // adding the name of the new most tastiest food to the array
        }
        // checking if there is an organism whose equally as tasty as the most tastiest organism
        else if (tastyFoodCounts[foodInd] == numMostEatenFood) {
            mostTastiestFoods[numTastyFoods++] = pWeb[foodInd].name; // adding that organism to the array
        }
    }
    // printing the most tastiest organisms 
    for (int i = 0; i < numTastyFoods; ++i) {
        printf("  %s\n", mostTastiestFoods[i]);
    }
}


// this function prints the height of each organism in the food web; the height of an organism is obtained by finding the longest path from any of the producers up to that organism
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printFoodWebHeights(Org *pWeb, int numOrg) {

    int orgHeights[numOrg]; // array to keep track of the heights of each organism
    bool predHeightUpdate; // bool that updates whenever a height is increased
    
    // initializing all heights to 0
    for (int i = 0; i < numOrg; ++i) {
        orgHeights[i] = 0;
    }

    // loop that captures all of the height updates
    while (true) {

        predHeightUpdate = false;
        // looping through every predator
        for (int predInd = 0; predInd < numOrg; ++predInd) {
            int maxPreyHeight = 0; // variable that keeps track of the max height of a given predator's prey

            // looping through the current predator's prey
            for (int preyInd = 0; preyInd < pWeb[predInd].numPrey; ++preyInd) {

                // checking if the height of the current prey is larger than the max height found so far
                if (orgHeights[pWeb[predInd].prey[preyInd]] > maxPreyHeight) {
                    maxPreyHeight = orgHeights[pWeb[predInd].prey[preyInd]]; // updating the max height variable
                }
            }

            // checking if the current predator has at least one prey
            if (pWeb[predInd].numPrey > 0) {

                // checking if the current predator's height is equal to the max height of its prey + 1
                if (orgHeights[predInd] != maxPreyHeight + 1) {
                    orgHeights[predInd] = maxPreyHeight + 1; // if not, its height is updated
                    predHeightUpdate = true; // bool is updated since a predator's height has changed
                }
            }
            // checking if the current predator doesn't have any prey (producer)
            else if (orgHeights[predInd] != 0) {
                orgHeights[predInd] = 0; // if it doesn't its height is set to 0
                predHeightUpdate = true; 
            }
        }
        // the loop is broken out of if no changes were made to any of the Orgs' heights during the run
        if (!predHeightUpdate) {
            break;
        }
    }
    // printing the Orgs' heights
    for (int i = 0; i < numOrg; ++i) {
        printf("  %s: %d\n", pWeb[i].name, orgHeights[i]);
    }
}


// this function organizes each organism in the web into one of the following categories: producer, herbivore, omnivore, carnivore
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array
void printVoreTypes(Org *pWeb, int numOrg) {

    printf("  Producers:\n");
    // looping through each organism
    for (int org = 0; org < numOrg; ++org) {
        // if the organism doesn't have any prey, then it's a producer
        if (pWeb[org].numPrey == 0) {
            printf("    %s\n", pWeb[org].name);
        }
    }

    printf("  Herbivores:\n");
    // looping through each organism
    for (int org = 0; org < numOrg; ++org) {
        // checking if the organism has at least one prey 
        if (pWeb[org].numPrey > 0) {
            bool herbivoreType = true;

            // looping through the current Org's prey
            for (int preyInd = 0; preyInd < pWeb[org].numPrey; ++preyInd) {
                // if the prey itself has zero prey, then the current Org consumes producers
                if (pWeb[pWeb[org].prey[preyInd]].numPrey == 0) {
                    herbivoreType = true; // herbivores eat producers, so the bool is set to true
                }
                else { // if the prey has at least one prey element, then the current Org cannot be a herbivore
                    herbivoreType = false;
                    break;
                }
            }
            // printing the names of the herbivores
            if (herbivoreType) {
                printf("    %s\n", pWeb[org].name);
            }
        }
    }
    
    printf("  Omnivores:\n");
    // looping through each organism
    for (int org = 0; org < numOrg; ++org) {

        // checking if the organism has at least one prey
        if (pWeb[org].numPrey > 0) {
            bool consumesProducers = false;
            bool consumesAnimals = false;
            bool omnivoreType = false;

            // looping through the current Org's prey
            for (int preyInd = 0; preyInd < pWeb[org].numPrey; ++preyInd) {
                
                // if the prey itself has zero prey, then the current Org consumes producers
                if (pWeb[pWeb[org].prey[preyInd]].numPrey == 0) {
                    consumesProducers = true;
                }
                else { // if there are no producers in the prey list, then the current Org consumes animals
                    consumesAnimals = true;
                }
            }
            // if an Org consumes both producers and animals, it is an omnivore
            if (consumesProducers && consumesAnimals) {
                omnivoreType = true;
            }
            if (omnivoreType) {
                printf("    %s\n", pWeb[org].name);
            }
        }
    }

    printf("  Carnivores:\n");
    // looping through each organism
    for (int org = 0; org < numOrg; ++org) {
        // checking if the organism has at least one prey
        if (pWeb[org].numPrey > 0) {
            bool carnivoreType = true;

            // looping through the current Org's prey
            for (int preyInd = 0; preyInd < pWeb[org].numPrey; ++preyInd) {
                
                // checking if the Org has any producers in its prey list
                if (pWeb[pWeb[org].prey[preyInd]].numPrey == 0) {
                    carnivoreType = false; // if it does, then it is not a carnivore
                    break;
                }
            }
            if (carnivoreType) {
                printf("    %s\n", pWeb[org].name);
            }
        }
    }
}


// this function calls all of the decomposed functions that have been written; the decomposed functions provide different pieces of information regarding the characteristics of the food web
// parameters: pWeb is a dynamically allocated array of Orgs, numOrg is the size of the array, modified is a bool that indicates whether the original food web is being printed or an updated one
void displayAll(Org* pWeb, int numOrg, bool modified) {

    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(pWeb,numOrg); 
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
    printApexPredators(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    printProducers(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");
    printMostFlexibleEaters(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    printTastiestFood(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    printFoodWebHeights(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    printVoreTypes(pWeb, numOrg);
    printf("\n");

}


// this function proccesses command-line arguments and updates the program mode parameters
// parameters: argc is the number of arguments that were entered on the command-line, argv is an array of the command-line arguments that were entered
// parameters: pBasicMode is bool that is passed-by-reference and represents the mode in which the initial web is only built and no modifications are allowed to be made to it
// parameters: pDebugMode is a bool that is passed-by-reference and represents the mode in which a full web is built after each step of modification
// parameters: pQuietMode is a bool that is passed-by-reference and represents the mode in which the prompt messages are suppressed
// this function returns false if an invalid or duplicate command-line argument was entered, and otherwise returns true
bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {

    // initializing counts for each valid command-line argument
    int bCount = 0;
    int dCount = 0;
    int qCount = 0;
    
    // loop checks for an invalid command-line argument and updates the count of the valid command-line arguments
    for (int i = 1; i < argc; ++i) { 

        // checking if the first two characters of the current command-line argument are valid
        if ((argv[i][0] != '-') || ((argv[i][1] != 'b') && (argv[i][1] != 'd') && (argv[i][1] != 'q'))) { 
            return false;
        }
        // checking if the basicMode command has been entered
        if (((argv[i][0] == '-') && argv[i][1] == 'b')) {
            bCount++;

        }
        // checking if the debugMode command has been entered
        else if (((argv[i][0] == '-') && argv[i][1] == 'd')) {
            dCount++;
        }
        // checking if the quietMode command has been entered
        else if (((argv[i][0] == '-') && argv[i][1] == 'q')) {
            qCount++;
        }
    }

    if ((bCount > 1) || (dCount > 1) || (qCount > 1)) { // checking if a valid command has been entered more than once
        return false;
    }
    // checking if the basicMode command has been entered exactly once
    if (bCount == 1) {
        *pBasicMode = true; // pointer changes the value of basicMode variable in main() to true
    }
    // checking if the debugMode command has been entered exactly once
    if (dCount == 1) {
        *pDebugMode = true; // pointer changes the value of debugMode variable in main() to true
    }
    // checking if the quietMode command has been entered exacrly once
    if (qCount == 1) {
        *pQuietMode = true; // pointer changes the value of quietMode variable in main() to true
    }
    return true;
}

// this is a helper function to print the state of a program mode (either on or off)
// parameter: mode represents one of the three different program modes (basic mode, debug mode, quiet mode)
void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}

int main(int argc, char* argv[]) {  
    
    bool basicMode = false;
    bool debugMode = false;
    bool quietMode = false;

    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1;
    }

    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode);
    printf("  debug mode = ");
    printONorOFF(debugMode);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("\n");

    int numOrgs = 0;
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* pWeb = NULL; // pWeb is initialized as a pointer to an Org struct and is set to NULL
            // pWeb will later act as a pointer to the first element of a dynamically allocated array of Org structs, representing the food web

    printf("Building the initial food web...\n");
    
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = "";
    scanf("%s",tempName); // reading the first organism name
    if (!quietMode) printf("\n");

    // this loop runs until the user types DONE
    while (strcmp(tempName,"DONE") != 0) { 
        addOrgToWeb(&pWeb,&numOrgs,tempName); // an organism is added to the food web
        if (debugMode) {
            printf("DEBUG MODE - added an organism:\n");
            printWeb(pWeb,numOrgs); // printing the current state of the food web in debugMode
            printf("\n");
        }
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName); // user is prompted for the next organism
        if (!quietMode) printf("\n");
    }
    if (!quietMode) printf("\n");

    // prompting the user to enter a predator/prey relation (unless in quietMode)
    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
    int predInd, preyInd;
    scanf("%d %d",&predInd, &preyInd); // reading the first predator/prey relation
    if (!quietMode) printf("\n");

    // loop runs until the user enters a pair that is out of the appropriate index range, or until the predator and the prey are the same organism
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(pWeb,numOrgs,predInd,preyInd); // a relation is added to the web 
        if (debugMode) {
            printf("DEBUG MODE - added a relation:\n");
            printWeb(pWeb,numOrgs); // printing the current state of the food web in debugMode
            printf("\n");
        }
        
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
        scanf("%d %d",&predInd, &preyInd);  // user is prompted for the next predator/prey relation
        if (!quietMode) printf("\n");
    }
    printf("\n");

    printf("--------------------------------\n\n");
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");
    
    displayAll(pWeb,numOrgs,false); // this function prints all the information regarding the food web that is at our disposal

    if (!basicMode) {
        printf("--------------------------------\n\n");
        printf("Modifying the food web...\n\n");
        char opt = '?'; // variable to store the menu selection entered by the user

        // continue reprompting the user for their selection unless the user enters "q"
        while (opt != 'q') {
            // display all the food web options (unless in quietMode)
            if (!quietMode) {
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");
            }
            scanf(" %c", &opt); // reading the selection from the user
            if (!quietMode) printf("\n\n");

            if (opt == 'o') {
                char newName[20];
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName); // reading the name of the new organism
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&pWeb,&numOrgs,newName); // adding the new organism to the food web
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'x') {
                int extInd;
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd); // reading the index for the extinct organism (organism to be removed from the web)
                if (!quietMode) printf("\n");

                // checking that the index entered is valid
                if (extInd >= 0 && extInd < numOrgs) { 
                    printf("Species Extinction: %s\n", pWeb[extInd].name); // printing the name of the organism that is deemed extinct
                    removeOrgFromWeb(&pWeb,&numOrgs,extInd); // removing the organism from the food web
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");
                
                if (debugMode) {
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'r') {
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd); // reading the indices for the predator and prey relation
                if (!quietMode) printf("\n");

                if (addRelationToWeb(pWeb,numOrgs,predInd,preyInd)) { // adding the relation to the web
                    printf("New Food Source: %s eats %s\n", pWeb[predInd].name, pWeb[preyInd].name); // printing the predator/prey relation that has been added
                };
                printf("\n");
                if (debugMode) {
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'p') {
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(pWeb,numOrgs);
                printf("\n");
                
            } else if (opt == 'd') {
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(pWeb,numOrgs,true); // all of the information regarding the food web is displayed

            }
            printf("--------------------------------\n\n");
        }
    }
    freeWeb(pWeb,numOrgs); // freeing the dynamic heap-allocated memory for the web before the program ends
    return 0;
}

