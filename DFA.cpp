#include "DFA.h"


byte* Eval_DFA(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	)
{
	int state=0;
	for (int i=0;i<input_size;i++)
	{
		state = dfa[inputs[i]][state];
		printf ("next state for input %d is %d\n",inputs[i],state);
	}
	return messages[state];
}

