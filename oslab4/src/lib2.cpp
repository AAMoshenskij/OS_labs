#include<iostream>

using namespace std;

extern "C" int PrimeCount(int A, int B) {
	int mas[B], cnt = 0;
	for(int i = A; i <= B; i++) {
		if(B <= 1) {
			return cnt;
		}
		if(i <= 1) {
			continue;
		}
		mas[i] = i;
	}
	mas[1] = 0;
	for(int i = 2; i <= B; i++) {
        if(mas[i] != 0) {
            for(int j = i * 2; j <= B; j += i){
                mas[j] = 0;
            }
        }
    }
    for(int i = A; i <= B; i++) {
    	if(i <= 1) {
    		continue;
    	}
    	if (mas[i] != 0) {
    		cnt++;
    	}
    }
    return cnt;
}

extern "C" float Pi(int K) {
    float result = 1.0;
    for (int i = 1; i <= K; i++) {
        result *= (4.0*i*i) / (4.0*i*i - 1);
    }
    return result * 2;
}