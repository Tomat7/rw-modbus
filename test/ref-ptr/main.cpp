#include <iostream>

using namespace std;

void func1(int B)
{
    B = 12;
}

void func2(int& B)
{
    B = 15;
}

void func3(int *ptr2)
{
    *ptr2 = 18;
}


int main()
{
    int A;
    int *ptr = &A;
    int &ref = A;
    
//    ptr = &A;
    cout << endl;

    A = 5;
    cout << "--- Set A=5 ---" << endl;
    cout << "A= " << A << endl;
    cout << "ref= " << ref << endl;
    cout << "&ref= " << &ref << endl;
    cout << "ptr= " << ptr << endl;
    cout << "*ptr= " << *ptr << endl;
    cout << "================" << *ptr << endl;

    *ptr = 7;
    cout << "--- Set *ptr = 7 ---" << endl;
    cout << "A= " << A << endl;
    cout << "ref= " << ref << endl;
    cout << "&ref= " << &ref << endl;
    cout << "ptr= " << ptr << endl;
    cout << "*ptr= " << *ptr << endl;
    cout << "================" << *ptr << endl;

    ref = 9;
    cout << "--- Set ref = 9 ---" << endl;
    cout << "A= " << A << endl;
    cout << "ref= " << ref << endl;
    cout << "&ref= " << &ref << endl;
    cout << "ptr= " << ptr << endl;
    cout << "*ptr= " << *ptr << endl;
    cout << "================" << *ptr << endl;

    func1(A);
    cout << "--- Set (B) = 12 ---" << endl;
    cout << "A= " << A << endl;
    cout << "ref= " << ref << endl;
    cout << "&ref= " << &ref << endl;
    cout << "ptr= " << ptr << endl;
    cout << "*ptr= " << *ptr << endl;
    cout << "================" << *ptr << endl;

    func2(A);
    cout << "--- Set (&B) = 15 ---" << endl;
    cout << "A= " << A << endl;
    cout << "ref= " << ref << endl;
    cout << "&ref= " << &ref << endl;
    cout << "ptr= " << ptr << endl;
    cout << "*ptr= " << *ptr << endl;
    cout << "================" << *ptr << endl;

    func3(&A);
    cout << "--- Set (*ptr2) = 18 ---" << endl;
    cout << "A= " << A << endl;
    cout << "ref= " << ref << endl;
    cout << "&ref= " << &ref << endl;
    cout << "ptr= " << ptr << endl;
    cout << "*ptr= " << *ptr << endl;
    cout << "================" << *ptr << endl;

//    func3(&A);


    return 0;
}
