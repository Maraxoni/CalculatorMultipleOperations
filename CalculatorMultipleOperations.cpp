#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

float calculate(float x, float y, char op) {
    float result;

    asm volatile (
        "movss %1, %%xmm0 \n"   // Przenoszenie wartości x do xmm0
        "movss %2, %%xmm1 \n"   // Przenoszenie wartości y do xmm1

        "cmp $'+', %3 \n"       // If "+" jump to addition
        "je addition \n"
        "cmp $'-', %3 \n"       // If "-" jump to subtraction
        "je subtraction \n"
        "cmp $'*', %3 \n"       // If "*" jump to multiplication
        "je multiplication \n"
        "cmp $'/', %3 \n"       // If "/" jump to division
        "je division \n"
        "jmp end \n"            // Jump to end

        "addition: \n"
        "addss %%xmm1, %%xmm0 \n" // Add xmm1 to xmm0
        "jmp end \n"            // Jump to end

        "subtraction: \n"
        "subss %%xmm1, %%xmm0 \n" // Subtract xmm1 from xmm0
        "jmp end \n"            // Jump to end

        "multiplication: \n"
        "mulss %%xmm1, %%xmm0 \n" // Multiply xmm0 by xmm1
        "jmp end \n"            // Jump to end

        "division: \n"
        "divss %%xmm1, %%xmm0 \n" // Divide xmm0 by xmm1
        "jmp end \n"            // Jump to end

        "end: \n"
        "movss %%xmm0, %0 \n"   // Moving output form xmm0 to result
        : "=m" (result)         // Output - result
        : "m" (x), "m" (y), "r" (op) // Input - x, y, op
        : "xmm0", "xmm1"        // SSE registers xmm0 i xmm1 used in SSE instructions
        );

    return result;
}

int main() {
    string input;
    cout << "Enter operations (+,-,*,/) separated by spaces: ";
    getline(cin, input);

    istringstream iss(input);
    vector<float> numbers;
    vector<char> operations;

    float num;
    char op;
    while (iss >> num >> op) {
        numbers.push_back(num);
        operations.push_back(op);
    }
    // Push the last number into the numbers vector
    numbers.push_back(num);

    if (numbers.size() != operations.size() + 1) {
        cout << "Invalid input!" << endl;
        return 1;
    }

    // Sprawdzanie kolejności działań: '*','/' najpierw, '+' i '-' później
    for (char precedence : {'*', '/', '+', '-'}) {
        for (size_t i = 0; i < operations.size(); ++i) {
            if (operations[i] == precedence) {
                float result = calculate(numbers[i], numbers[i + 1], operations[i]);
                numbers[i] = result;
                numbers.erase(numbers.begin() + i + 1);
                operations.erase(operations.begin() + i);
                --i; // Adjust index after erasing
            }
        }
    }

    cout << "Result: " << numbers[0] << "\n";

    return 0;
}