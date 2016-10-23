#include "hmm.h"

#define MAX_LEN 1024

int main(int argc, char const *argv[]) {
  FILE* f1, *f2;
  char ans[] = "testing_answer.txt";
  char myans[] = "result1.txt";

  f1 = fopen(ans, "r");
  f2 = fopen(myans, "r");

  char line[MAX_LEN];
  char m[MAX_LEN];
  double d;
  FILE* acc = fopen("acc.txt", "w");

  int total_num = 0;
  int right_num = 0;

  // printf("Start:\n");
  while (fscanf(f1, "%s", line) != EOF) {
    fscanf(f2, "%s %lf", m, &d);
    // printf("%s -> %s %e\n", line, m, d);

    total_num++;
    if (strcmp(line, m) == 0) right_num++;
  }
  fprintf(acc, "%f\n", (double)right_num/total_num);
  fclose(acc);
  // printf("End\n");

  fclose(f1);
  fclose(f2);

  return 0;
}