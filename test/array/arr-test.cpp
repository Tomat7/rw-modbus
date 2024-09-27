#include <iostream>
using namespace std;

int arrglobal[1000000];

int main() {
  int length;
  
  cin >> length;

  int * arr = new int[length];

  for (int i = 0; i < length; i++) {
    arr[i] = (i + 1) * 10;
  }

  for (int i = 0; i < length; i++) {
    cout << arr[i] << " " << endl;
  }

  delete[] arr;

  return 0;
}
