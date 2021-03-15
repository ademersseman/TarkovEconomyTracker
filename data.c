#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct item {
   int price[24];
   char name[50];
} item;

int x = 0;
void printNames(item arr[]) {
  for (int i = 0; i < 2158; i++) {
    printf("%s", arr[i].name);
  }
}

void itemcpy(item *a,  item *b) {
  memcpy(a, b, sizeof(char) * 50 + sizeof(int) * 24);
}

void merge(item arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  item L[n1], R[n2];

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++) {
    itemcpy(&L[i], &arr[l + i]);
  }
  for (j = 0; j < n2; j++) {
    itemcpy(&R[j], &arr[m + 1 + j]);
  }

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (strcmp(L[i].name, R[j].name) <= 0/*L[i] <= R[j]*/) {
      itemcpy(&arr[k], &L[i]);
      i++;
    }
    else {
      itemcpy(&arr[k], &R[j]);
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
  are any */
  while (i < n1) {
    itemcpy(&arr[k], &L[i]);
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
  are any */
  while (j < n2) {
    itemcpy(&arr[k], &R[j]);
    j++;
    k++;
  }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(item arr[], int l, int r)
{
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    merge(arr, l, m, r);
  }
}

int binarySearch(item arr[], char userInput[]) {
  item *guess;
  int l = 0, r = 2158, m;
  printf("\n");
  while(r > l) {
    m = l + (r - l)/2;
    guess = &arr[m];
    printf("\n\nthe best guess is: %s", guess->name);
    printf("The strcmp output is: %d\n", strncmp(guess->name, userInput, 19));
    for (int i = 0; i < 50; i++) {
      if (guess->name[i] == '\0') {
        printf("has a null char at: %d", i);
        break;
      }
    }
    for (int i = 0; i < 50; i++) {
      if (userInput[i] == '\0') {
        printf("\nand has a null char at: %d", i);
        break;
      }
    }

    if (strncmp(guess->name, userInput, 19) == 0) {
      printf("\nFOUND!");
      return m;
    } else if (strncmp(guess->name, userInput, 19) >= 0) {
      r = m;
    } else if (strncmp(guess->name, userInput, 19) <= 0) {
      l = m;
    }
  }
  free(guess);
  return m;
}


void printDatabase(item arr[]) {
  for (int i = 0; i < 2158; i++) {
    printf("\n%s", arr[i].name);
  }
}

int main() {
  item items[2158];

//opens file
  FILE *fp;
  fp = fopen("itemnames.txt", "r");
//initialize the names of every item
  int index = 0;
  while (fgets(items[index].name, 150, fp) != NULL) {
    int count = 0;
    for (int i = 0; i < strlen(items[index].name); i++) {
      if (items[index].name[i] != ' ') {
        items[index].name[count++] = items[index].name[i];
      }
    }
    items[index].name[count - 1] = '\0';
    index++;
  }
  fp = fopen("programoutput.txt", "w");
  fclose(fp);
/*
  mergeSort(items, 0, 2157);

  printf("\nresult:%d\n%s", binarySearch(items, "Secure Flash drive"), items[binarySearch(items, "Secure Flash drive")].name);
  printf("Press ENTER key to Continue\n");
  getchar();
*/

  int seconds = 3600;//used to count the seconds
  int currentHour = 0;//keeps track of the current hour
  int searching = 0; //0 if not searching 1 if searching
  while(1) {
    if (currentHour == 24) {
      currentHour = 0;
    }

    if (seconds == 3600 && searching == 0) {//refresh and load prices
      seconds = 0;
      fp = fopen("hourlyprice.txt", "r");

      for (int i = 0; i < 2158; i++) {
        fscanf(fp, "%d", &items[i].price[currentHour]);
      }
      fp = fopen("currenthour.txt", "w");
      fprintf(fp, "%d", currentHour);
      currentHour++;
      printf("\nSuccessfully logged the price this hour!");
      fclose(fp);
    }


    fp = fopen("userinput.txt", "r");

    char userInput[50];
    if (fscanf(fp,"%s",&userInput) != EOF) {
        printf("\nThe item in userInput.txt is:%s", userInput);

        fclose(fp);
        searching++;

        mergeSort(items, 0, 2157);

        item *current = &items[binarySearch(items, userInput)];

        printf("\nThe item I found is: %s", current->name);
        fp = fopen("programoutput.txt", "w");
        for (int i = 0; i < 24; i++) {
          printf("\nloading current output: %d", current->price[i]);
          fprintf(fp, "%d\n", current->price[i]);
        }
        fclose(fp);
        free(current);

        fp = fopen("userinput.txt", "w");//deletes user input

        //must create a new token every day probably through scheduled python event
    }

    fclose(fp);

    sleep(1);

    seconds++;
  }
}
