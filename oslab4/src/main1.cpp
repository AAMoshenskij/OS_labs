#include<iostream>
#include"../include/lib.h"

using namespace std;

int main()
{
	int k;
	cout << "Enter 1 or 2 to select a function or -1 to exit" << endl;

	while(cin >> k) {

		if (k == 1) {
			int A, B;
			cout << "Enter section [A,B]" << endl;
			cin >> A >> B;
			cout<< "Count of prime numbers:" << PrimeCount(A, B) << endl;
		}

		if (k == 2) {
			int K;
			cout << "Enter size of row" << endl;
			cin >> K;
			cout << "Pi:" << Pi(K) << endl;
		}

		if (k == -1) {
			cout << "Exit" << endl;
			return 0;
		}

		cout << "Enter 1 or 2 to select a function or -1 to exit" << endl;
		
	}
}
