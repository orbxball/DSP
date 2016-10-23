#include "hmm.h"

#define HMMNUM 5

int main(int argc, char const *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "Usage:\n ./test [model list] [testing data] [result]\n");
    // result should have the best model and the likelihood
    return 0;
  }

  // Initialization
  FILE* f;
  f = fopen(argv[2], "r");
  char sequence[MAX_SEQ];

  HMM hmms[HMMNUM];
  load_models(argv[1], hmms, HMMNUM);
  // dump_models(hmms, 5);

  // Iterate througn testing data
  while(fscanf(f, "%s", sequence) != EOF) {
    int observ_len = strlen(sequence);
    double prob[HMMNUM] = {0};  // probability array stored the hiest probability in hmms[n]

    // Iterate through model list to see which is the best
    for (int n = 0; n < HMMNUM; n++) {

      // delta
      double delta[MAX_SEQ][MAX_STATE] = {{0}};
      for (int t = 0; t < observ_len; t++) {
        for (int j = 0; j < hmms[n].state_num; j++) {
          if (t == 0) {
            delta[t][j] = hmms[n].initial[j] * hmms[n].observation[sequence[t]-'A'][j];
          }
          else {
            for (int i = 0; i < hmms[n].state_num; i++) {
              double tmp = delta[t-1][i] * hmms[n].transition[i][j];
              if (tmp > delta[t][j]) delta[t][j] = tmp;
            }
            delta[t][j] *= hmms[n].observation[sequence[t]-'A'][j];
          }
        }
      }

      // find the highest probability in delta and write into prob array
      for (int i = 0; i < hmms[n].state_num; i++) {
        double tmp = delta[observ_len-1][i];
        if (tmp > prob[n]) prob[n] = tmp;
      }

    }

    // find the index of the highest probability of hmm
    int index = 0;
    for (int i = 0; i < HMMNUM; i++) {
      if (prob[i] > prob[index]) index = i;
    }

    // write into the result.txt
    FILE* result = fopen(argv[3], "a");
    fprintf(result, "model_0%d.txt %e\n", index+1, prob[index]);
    fclose(result);
  }

  fclose(f);

  return 0;
}
