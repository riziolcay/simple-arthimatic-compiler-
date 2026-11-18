#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
using namespace std;

class Compiler {
private:
    string input;
    int pos = 0;

    void skipSpaces() {
        while (pos < input.length() && isspace(input[pos])) {
            pos++;
        }
    }

    double expression() {
        double result = term();

        while (true) {
            skipSpaces();

            if (pos < input.length() && input[pos] == '+') {
                pos++;
                result += term();
            }
            else if (pos < input.length() && input[pos] == '-') {
                pos++;
                result -= term();
            }
            else {
                break;
            }
        }
        return result;
    }

    double term() {
        double result = factor();

        while (true) {
            skipSpaces();

            if (pos < input.length() && input[pos] == '*') {
                pos++;
                result *= factor();
            }
            else if (pos < input.length() && input[pos] == '/') {
                pos++;
                double value = factor();

                if (value == 0)
                    throw runtime_error("Division by zero!");

                result /= value;
            }
            else if (pos < input.length() && input[pos] == '%') {
                pos++;
                double value = factor();

                if (value == 0)
                    throw runtime_error("Division by zero!");

                if (result != (int)result || value != (int)value)
                    throw runtime_error("Modulus requires integers!");

                result = (int)result % (int)value;
            }
            else {
                break;
            }
        }
        return result;
    }

    double factor() {
        skipSpaces();

        if (pos >= input.length())
            throw runtime_error("Invalid expression!");

        if (input[pos] == '-') {
            pos++;
            return -factor();
        }

        if (input[pos] == '+') {
            pos++;
            return factor();
        }

        if (input[pos] == '(') {
            pos++;
            double result = expression();

            skipSpaces();

            if (pos >= input.length() || input[pos] != ')')
                throw runtime_error("Missing closing parenthesis!");

            pos++;
            return result;
        }

        if (isdigit(input[pos]) || input[pos] == '.') {
            size_t start = pos;

            while (pos < input.length() &&
                   (isdigit(input[pos]) || input[pos] == '.')) {
                pos++;
            }

            return stod(input.substr(start, pos - start));
        }

        throw runtime_error("Invalid character or number!");
    }

public:
    void compile(string code) {
        input = code;
        pos = 0;

        double result = expression();

        skipSpaces();

        if (pos != input.length())
            throw runtime_error("Unexpected character!");

        cout << "Compilation Successful!" << endl;
        cout << "Result = " << result << endl;
    }
};

int main() {
    Compiler compiler;
    string code;

    cout << "===== SIMPLE ARITHMETIC COMPILER =====" << endl;
    cout << "Supported: +  -  *  /  %  ( )" << endl;
    cout << "Enter 'exit' to close the compiler." << endl;

    while (true) {
        cout << "\nEnter arithmetic expression: ";
        getline(cin, code);

        if (code == "exit")
            break;

        try {
            compiler.compile(code);
        }
        catch (exception &e) {
            cout << "Compilation Error: " << e.what() << endl;
        }
    }
    return 0;
}