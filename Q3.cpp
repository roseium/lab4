#include <iostream>
#include <iomanip>
using namespace std;

// Function that prints the numbers as real numbers
void print_real(float number, int fieldspace, int precision)
{
    cout << fixed << setw(fieldspace) << setprecision(precision) << number << endl;
}


// Template function
template <typename T>
void update_scale(T &m1, T &m2, T m3 = 10)
{
    T temp_m1 = m1;
    T temp_m2 = m2;

    // Update m1: sum of m1 and m2, then multiply by m3
    m1 = (temp_m1 + temp_m2) * m3;

    // Update m2: difference of m1 and m2, then multiply by m3
    m2 = (temp_m1 - temp_m2) * m3;
}


int main(void)
{
    float a, b;

    // Input two numbers
    cout << "Please input two real numbers: ";
    cin >> a >> b;

    // Print initial values of a and b
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    // Call the update_scale function with a and b
    update_scale(a, b);

    // Print updated values of a and b
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    return 0;
}
