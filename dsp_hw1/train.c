#include "hmm.h"

int main(int argc, char const *argv[]) {

  // Check arguments
  if (argc != 5) {
    fprintf(stderr, "Usage:\n ./train [iteration times] [initial model name] [training data] [output model name]\n");
    return 0;
  }

  FILE* f;
  HMM hmm;
  loadHMM(&hmm, argv[2]);

  int iteration = atoi(argv[1]);
  while(iteration--){
    // Initialization for accumulate gamma & observ-gamma & epsilon
    double accumulate_initial_gamma[MAX_STATE] = {0};
    double accumulate_gamma[MAX_STATE] = {0};
    double accumulate_observation_gamma[MAX_STATE][MAX_STATE] = {{0}};
    double accumulate_epsilon[MAX_STATE][MAX_STATE] = {{0}};

    char sequence[MAX_SEQ];
    double sample_num = 0;

    //For each iteration
    f = fopen(argv[3], "r");
    while(fscanf(f, "%s", sequence) != EOF) {
      sample_num++;
      int observ_len = strlen(sequence);


      // alpha
      double alpha[MAX_SEQ][MAX_STATE] = {{0}};
      for (int t = 0; t < observ_len; t++) {
        for (int j = 0; j < hmm.state_num; j++) {
          if (t == 0) {
            alpha[t][j] = hmm.initial[j] * hmm.observation[sequence[t]-'A'][j];
          }
          else {
            for (int i = 0; i < hmm.state_num; i++)
              alpha[t][j] += alpha[t-1][i] * hmm.transition[i][j];
            alpha[t][j] *= hmm.observation[sequence[t]-'A'][j];
          }
        }
      }


      // beta
      double beta[MAX_SEQ][MAX_STATE] = {{0}};
      for (int t = observ_len-1; t >= 0; t--) {
        for (int i = 0; i < hmm.state_num; i++) {
          if (t == observ_len-1) {
            beta[t][i] = 1;
          }
          else {
            for (int j = 0; j < hmm.state_num; j++)
              beta[t][i] += hmm.transition[i][j] * hmm.observation[sequence[t+1]-'A'][j] * beta[t+1][j];
          }
        }
      }


      // gamma
      double gamma[MAX_SEQ][MAX_STATE] = {{0}};
      for (int t = 0; t < observ_len; t++) {
        double denominator = 0;
        for (int i = 0; i < hmm.state_num; i++) {
          gamma[t][i] = alpha[t][i] * beta[t][i];
          denominator += gamma[t][i];
        }

        // Divided by the denominator to standardlize the gamma[t][i]
        for (int i = 0; i < hmm.state_num; i++)
          gamma[t][i] /= denominator;
      }


      // epsilon
      double epsilon[MAX_SEQ][MAX_STATE][MAX_STATE] = {{{0}}};
      for (int t = 0; t < observ_len-1; t++) {
        double denominator = 0;
        for (int i = 0; i < hmm.state_num; i++) {
          for (int j = 0; j < hmm.state_num; j++) {
            epsilon[t][i][j] = alpha[t][i] * hmm.transition[i][j] * hmm.observation[sequence[t+1]-'A'][j] * beta[t+1][j];
            denominator += epsilon[t][i][j];
          }
        }

        // Divided by the denominator to standarlize the epsilon[t][i][j]
        for (int i = 0; i < hmm.state_num; i++) {
          for (int j = 0; j < hmm.state_num; j++) {
            epsilon[t][i][j] /= denominator;
          }
        }
      }


      // Accumulate gamma, observ-gamma and epsilon
      for (int i = 0; i < hmm.state_num; i++) {
        accumulate_initial_gamma[i] += gamma[0][i];
        for (int t = 0; t < observ_len-1; t++) {
          accumulate_gamma[i] += gamma[t][i];
          accumulate_observation_gamma[sequence[t]-'A'][i] += gamma[t][i];
        }

        for (int j = 0; j < hmm.state_num; j++) {
          for (int t = 0; t < observ_len-1; t++)
            accumulate_epsilon[i][j] += epsilon[t][i][j];
        }
      }
    }
    fclose(f);


    // Re-estimate HMM parameter
    // (By using the accumulate gamma & observ-gamma & epsilon)

    // pi
    for (int i = 0; i < hmm.state_num; i++)
      hmm.initial[i] = accumulate_initial_gamma[i] / sample_num;

    // A
    for (int i = 0; i < hmm.state_num; i++) {
      for (int j = 0; j < hmm.state_num; j++)
        hmm.transition[i][j] = accumulate_epsilon[i][j] / accumulate_gamma[i];
    }

    // B
    for (int k = 0; k < hmm.observ_num; k++) {
      for (int j = 0; j < hmm.state_num; j++) {
        hmm.observation[k][j] = accumulate_observation_gamma[k][j] / accumulate_gamma[j];
      }
    }
  }

  // Check if sum of rows of trainsion & sum of columns of observation is 1
  /*dumpHMM(stderr, &hmm);
  double row, col, initial;
  for (int i = 0; i < hmm.state_num; i++) {
    row = 0, col = 0, initial = 0;
    for (int j = 0; j < hmm.state_num; j++) {
      row += hmm.transition[i][j];
      col += hmm.observation[j][i];
      initial += hmm.initial[j];
    }
    printf("#%d row: %e\n", i+1, row);
    printf("#%d col: %e\n", i+1, col);
  }
  printf("initial:%e\n\n", initial);*/

  f = fopen(argv[4], "w");
  dumpHMM(f, &hmm);
  fclose(f);

  return 0;
}
