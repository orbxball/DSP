#include <stdio.h>
#include <stdlib.h>
#include "File.h"
#include "Prob.h"       // About Probability
#include "LM.h"         // Basic interface of Language Model
#include "Ngram.h"      // Inherit from LM.h
#include "Vocab.h"      // Vocabulary: Here is for set of Chinese Character
#include "VocabMap.h"   // Vocabulary map: two vocabulary mapping
#include "VocabMultiMap.h"

/* ManPage
 * http://www.speech.sri.com/projects/srilm/manpages/
 */

#ifndef CANDIDATE
#define CANDIDATE 1000
#endif

#ifndef LEN
#define LEN 200
#endif

#ifndef SMALL
#define SMALL -100
#endif

/***
 * Available Constant in .h
 * ----------------------------------
 * maxWordsPerLine = 50000 (File.h)
 * maxWordLength = 1024 (Vocab.h)
 * Log_Zero = -inf (Prob.h)
 * Log_Inf = inf (Prob.h)
 * Log_One = 0 (Prob.h)
 * Vocab_None: A special VocabIndex used to denote no vocabulary item and to terminate VocabIndex arrays
***/

int main (int argc, char* argv[]) {
    if (argc != 9) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "./mydisambig -text [file] -map [map] -lm [LM] -order [orderNum]\n");
        exit(1);
    }

    /*** Initiailize arguments ***/
    const char* text_filename = argv[2];
    const char* map_filename = argv[4];
    const char* lm_filename = argv[6];
    int order = atoi(argv[8]);

    /*** Initialize extern parameter ***/
    Vocab voc;
    Vocab ZhuYin, Big5;

    Ngram lm(voc, order);
    //VocabMultiMap map(ZhuYin, Big5);
    VocabMap map(ZhuYin, Big5);

    /*** Read in the map & language model ***/
    File mapFile(map_filename, "r");
    map.read(mapFile);
    mapFile.close();

    File lmFile(lm_filename, "r");
    lm.read(lmFile);
    lmFile.close();
    
    /*** Read the testdata ***/
    File textfile(text_filename, "r");
    char* buf;
    while(buf = textfile.getline()) {
        VocabString sen[maxWordsPerLine];
        // Need to parse to words, otherwise char* will not work
        unsigned int count = Vocab::parseWords(buf, &(sen[1]), maxWordsPerLine);
        sen[0] = "<s>";
        sen[count+1] = "</s>";
        count += 2;
        
        /*for (int i = 1; i < count-1; i++)
            printf(">%s<%s", sen[i], (i == count-1)?"\n":"");
        */

        /*** VocabIter Example
        VocabIter iter(ZhuYin);
        iter.init();
        VocabIndex i;
        while (i = ZhuYin.getIndex(iter.next())) 
            printf("%d %s\n", i, ZhuYin.getWord(i));
        ***/

        /*** 
         * Viterbi for the sentence "sen"
         * for a sentence execute "count" times 
        ***/

        // Parameter
        LogP Proba[LEN][CANDIDATE] = {{0.0}};
        VocabIndex VidxGraph[LEN][CANDIDATE]; // Record the (i, j) VocalIndex of Big5
        int Backtrack[LEN][CANDIDATE];
        int CandiNum[LEN];

        Prob p;
        VocabIndex v_idx; //Be mapped by ZhuYin, which is a VocalIndex of Big5
        VocabIndex empty_context[] = {Vocab_None};
        VocabIndex bi_context[] = {Vocab_None, Vocab_None};

        // Initialization for Viterbi
        VocabMapIter iter(map, ZhuYin.getIndex(sen[0]));
        iter.init();
        int size = 0;
        while (iter.next(v_idx, p)) {
            VocabIndex candi = voc.getIndex(Big5.getWord(v_idx));
            //if (candi == Vocab_None) continue;
            // if we cannot find the word in voc, we should mark it Unknown rather than discard it
            candi = (candi == Vocab_None)? voc.getIndex(Vocab_Unknown): candi;

            LogP logp = lm.wordProb(candi, empty_context); //unigram for start <s>
            Proba[0][size] = (logp == LogP_Zero)? SMALL: logp;
            VidxGraph[0][size] = v_idx;
            Backtrack[0][size] = -1; // start: -1
            size++;
        }
        CandiNum[0] = size;

        //for (int j = 0; j < CandiNum[0]; j++) printf("prob:%f\n", Proba[0][j]);
        
        // Recursion for Viterbi
        for (int i = 1; i < count; i++) {
            VocabMapIter iter(map, ZhuYin.getIndex(sen[i]));
            iter.init();
            size = 0;
            while (iter.next(v_idx, p)) {
                VocabIndex candi = voc.getIndex(Big5.getWord(v_idx));
                candi = (candi == Vocab_None)? voc.getIndex(Vocab_Unknown): candi;

                // See last column which has the highest probability
                LogP maxp = LogP_Zero;
                for (int j = 0; j < CandiNum[i-1]; j++) {
                    VocabIndex last = voc.getIndex(Big5.getWord(VidxGraph[i-1][j]));
                    last = (last == Vocab_None)? voc.getIndex(Vocab_Unknown): last;
                    bi_context[0] = last;

                    LogP logp = lm.wordProb(candi, bi_context);
                    // Check backoff!!! VERY IMPORTANT!!! 
                    LogP backoff = lm.wordProb(candi, empty_context);
                    if (logp == LogP_Zero && backoff == LogP_Zero) logp == SMALL;

                    logp += Proba[i-1][j];
                    if (logp > maxp) {
                        maxp = logp;
                        Backtrack[i][size] = j;
                    }
                }
                Proba[i][size] = maxp;
                VidxGraph[i][size] = v_idx;
                size++;
            }
            CandiNum[i] = size;
        }
/*
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < CandiNum[i]; j++)
                printf("%f ", Proba[i][j]);
            printf("\n");
        }
*/

        // Find the Max Probability path
        LogP maxp = LogP_Zero;
        int max_col = -1, j;
        for (j = 0; j < CandiNum[count-1]; j++) {
            if (Proba[count-1][j] > maxp) {
                maxp = Proba[count-1][j];
                max_col = j;
            }
        }
        
        //printf("%d, %f, %d\n", max_col, maxp, Backtrack[count-1][0]);

        VocabString AnsPath[maxWordLength];
        AnsPath[0] = "<s>";
        AnsPath[count-1] = "</s>";
        for (int i = count-1; i > 0; i--) {
            AnsPath[i] = Big5.getWord(VidxGraph[i][max_col]);
            max_col = Backtrack[i][max_col];
        }

        // Print the Answer Path
        for (int i = 0; i < count; i++)
            printf("%s%s", AnsPath[i], (i == count-1)? "\n": " ");
    }
    textfile.close();

    return 0;
}
