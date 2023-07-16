#include <iostream>

using namespace std;
int main()
{
	int** mat = new int*[32];
	for (int i=0;i<32;i++)
		mat[i] = new int[20];
	
	cout << "size of mat is: "<<sizeof(mat) << endl;
	
	return 0;
}
