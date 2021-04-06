#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct item {//each item in the game has a name and 24 prices
   int price[24];
   char name[50];
} item;
FILE *fp;//used to open the various files

//copies content from one item to another
void itemcpy(item *a,  item *b) {
  memcpy(a, b, sizeof(char) * 50 + sizeof(int) * 24);
}

//performs merge sort
void merge(item items[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  //create temp arrays
  item L[n1], R[n2];

  //Copy data to temp arrays L[] and R[]
  for (i = 0; i < n1; i++) {
    itemcpy(&L[i], &items[l + i]);
  }
  for (j = 0; j < n2; j++) {
    itemcpy(&R[j], &items[m + 1 + j]);
  }

  //Merge the temp arrays back into arr[l..r]
  i = 0; // index for left array
  j = 0; // index for right array
  k = l; // index of merged array
  while (i < n1 && j < n2) {
    if (strcmp(L[i].name, R[j].name) <= 0) {
      itemcpy(&items[k], &L[i]);
      i++;
    }
    else {
      itemcpy(&items[k], &R[j]);
      j++;
    }
    k++;
  }

  // Copy elements of left array
  while (i < n1) {
    itemcpy(&items[k], &L[i]);
    i++;
    k++;
  }

  // Copy elements of right array
  while (j < n2) {
    itemcpy(&items[k], &R[j]);
    j++;
    k++;
  }
}

void mergeSort(item items[], int l, int r)
{
  if (l < r) {
    int m = l + (r - l) / 2;//middle of the new array

    // Sort first and second halves
    mergeSort(items, l, m);
    mergeSort(items, m + 1, r);

    merge(items, l, m, r);
  }
}

int binarySearch(item items[], char userInput[]) {
  item *guess;// current guess
  int l = 0, r = 2158, m;// defines bounds for search l for min, r for max, m for middle
  printf("\n");
  while(r > l) {// if r < l then the search was unsuccessful: exited proper bounds
    m = l + (r - l)/2;//sets middle or 'search' index
    guess = &items[m];// sets guess to search index

    if (strncmp(guess->name, userInput, 19) == 0) {// the item is found
      printf("\nFOUND!");
      return m;
    } else if (strncmp(guess->name, userInput, 19) >= 0) {// search is too high
      r = m;
    } else if (strncmp(guess->name, userInput, 19) <= 0) {// search is too low
      l = m;
    }
  }
  free(guess);
  return m;
}

//print all the prices of this variable to programoutput.txt
void printPriceToFile(item *current) {
  for (int i = 0; i < 24; i++) {
    fprintf(fp, "%d\n", current->price[i]);
  }
}

//saves all prices from hourlyprice.txt to items[]
void savePriceToItems(item items[], int currentHour) {
  for (int i = 0; i < 2158; i++) {
    fscanf(fp, "%d", &items[i].price[currentHour]);
  }
}

//saves all names from itemnames.txt to items[]
void saveNamesToItems(item items[]) {
  int index = 0;
  while (fgets(items[index].name, 150, fp) != NULL) {
    items[index].name[strlen(items[index].name) - 1] = '\0';
    index++;
  }
}

//prints the names in order for testing sort
void printDatabase(item items[]) {
  for (int i = 0; i < 2158; i++) {
    printf("\n%s", items[i].name);
  }
}

int main() {
  item items[2158];//items[] of all 2158 items in the game

  fp = fopen("itemnames.txt", "r");
  saveNamesToItems(items);//initialize the names of every item
  fp = fopen("programoutput.txt", "w");//wipes progress of previous runs
  fp = fopen("userInput.txt", "w");//wipes progress of previous runs
  fclose(fp);

  int seconds = 3600;//used to count the seconds
  int currentHour = 0;//keeps track of the current hour
  int searching = 0; //0 if not searching 1 if searching
  while(1) {
    if (currentHour == 24) {//maintains 24-hour cycle
      currentHour = 0;
    }

    if (seconds == 3600 && searching == 0) {//runs every hour to upload prices to the items array
      seconds = 0;
      fp = fopen("hourlyprice.txt", "r");
      savePriceToItems(items, currentHour);

      fp = fopen("currenthour.txt", "w");
      fprintf(fp, "%d", currentHour);//saves the current hour to currentHour.txt
      currentHour++;

      fclose(fp);
      printf("\nSuccessfully logged the price this hour!");
    }


    fp = fopen("userinput.txt", "r");

    char userInput[50];//stores the contents of userInput.txt

    if (fscanf(fp,"%s",&userInput) != EOF) {//if the file is not empty

        fclose(fp);
        searching++;

        mergeSort(items, 0, 2157);
        printDatabase(items);

        item *current = &items[binarySearch(items, userInput)];//current item the user selected
        printf("\nThe item I found is: %s", current->name);

        fp = fopen("programoutput.txt", "w");
        printPriceToFile(current);//prints prices to programoutput.txt
        free(current);

        fclose(fp);

        fp = fopen("userinput.txt", "w");//deletes user input
    }

    fclose(fp);

    sleep(1);

    seconds++;
  }
}
