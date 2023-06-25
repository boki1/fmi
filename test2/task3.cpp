#include <iostream>

using namespace std;

void generateBitVectors(int n, int sumEven, int sumOdd, string curr) {
    if (n == 0) {
        if (sumEven == sumOdd)
            cout << curr << endl;
        return;
    }

    generateBitVectors(n-1, sumEven, sumOdd+1, curr+"1");
    generateBitVectors(n-1, sumEven+1, sumOdd, curr+"0");
}

void generateAllBitVectors(int n) {
    generateBitVectors(n, 0, 0, "");
}

int main() {
    generateAllBitVectors(4);
    return 0;
}