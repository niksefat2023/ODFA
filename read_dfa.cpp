#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

int read_dfa(char *, //DFA file(input)
             int*, //starting state(output)
             int*, //number of states(output)
             int*, //number of accepting states(output)
             int ***, //a matrix with size 2*accept_states which speicify the signature-id per accepting state(output)
             int ***); //transition matrix(output)

int main()
{
    int states_num,accepting_num,s_start;
    int** accepting_states_ids;
    int** transitions;
    
    read_dfa ("sig10.dfa",&s_start, &states_num, &accepting_num, &accepting_states_ids, &transitions);
    
    printf("# states: %d\n",states_num);
    printf("# accepting states: %d\n",accepting_num);    
    printf("start is: %d\n",s_start);
    printf("transition test.. Next State for:\nState 3,Alphabet 92: %d\nState 3,Alphabet 67:%d\nState 0,Alphabet 2:%d \n", transitions[3][92], transitions[3][67], transitions[0][2]);

    system("PAUSE");
    return EXIT_SUCCESS;
}

int read_dfa(char *nfile, int* s_start, int* states_num, int* accepting_num,int*** accepting_states_ids,int*** transitions)
{  
  ifstream nfa_file;
  int i, tmps, j;

  unsigned int num_states, num_accepting_states, num_accept_ids, num_transitions;
  int tmp_st, tmp_acc_id, tmp_loc_s;

  nfa_file.open(nfile);
  if (!nfa_file) {
    cerr << "File could not be openned" << endl;
    exit(1);
  }

  /* read the number of states and num of (acc_state_no, accept_id) pairs */
  /* sali: now we assume that num of (acc_state_no, accept_id) is equal to number of accepting states. (One id per each accepting state) */
  nfa_file >> num_states >> num_accept_ids;
  
  /* allocate space and read accepting states and their signature-IDs */
  *accepting_states_ids = (int **)malloc(num_accept_ids * sizeof(int*));  
  for (i = 0; i < num_accept_ids; i++) {
    (*accepting_states_ids)[i]=(int*) malloc(2*sizeof(int));
    nfa_file >> tmp_st >> tmp_acc_id;

    (*accepting_states_ids)[i][0]= tmp_st;
    (*accepting_states_ids)[i][1]= tmp_acc_id;
  }
  
  /* read number of accepting states and the accepting state array */
  /* sali: not important for dfas */
  nfa_file >> num_accepting_states;
  for (i = 0; i < num_accepting_states; i++) {
    nfa_file >> tmps;
  } 
    
  /* read start state and the number of transitions */

    nfa_file >> *s_start >> num_transitions;
  
  //debug
//  cout << "Number of transitions " << num_transitions << endl;

  /* allocate space for transition table */
  *transitions = (int **)malloc(num_states * sizeof(int *));
  if (*transitions == NULL)
    perror("out of memory"), exit(1);
 
 //sali: read transition
  unsigned int tmp1,tmp2,tmp3;
  for (i = 0; i < num_states; i++) {
    (*transitions)[i] =  (int *)malloc(256 * sizeof(int));
    if ((*transitions)[i] == NULL)
      perror("out of memory"), exit(1);
   
    for (j=0;j<256;j++)
    {
        nfa_file >> tmp1 >> tmp2 >> tmp3;
        (*transitions)[i][tmp2] = tmp3;
    }
  } 
 
  *states_num = num_states;
  *accepting_num = num_accepting_states;
  nfa_file.close();  
}

