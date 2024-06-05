#include <cmath>
#include <string>

using namespace std;

const double EPS = 0.00001;

struct Stack {
    double number;
    char znak;
    Stack* next;
};
Stack* top_finale, * top_put;

void push(Stack*& t, double long number, char ch) {
    Stack* p = (Stack*)malloc(sizeof(Stack));
    p->number = number;
    p->znak = ch;
    p->next = t;
    t = p;
}

Stack pop(Stack*& t) {
    Stack temp = (*t);
    t = t->next;
    return temp;
}

bool apply(Stack*& t, char ch) {
    if (t == nullptr) {
        return false;
    }
    double num1 = (pop(t)).number;
    if (ch == 'C' || ch == 'S' || ch == 'T' || ch == 'Q' || ch == 'L' || ch == 'A') {
        if (ch == 'C') {
            push(t, cos(num1), '#');
        }
        if (ch == 'S') {
            push(t, sin(num1), '#');
        }
        if (ch == 'T') {
            push(t, tan(num1), '#');
        }
        if (ch == 'Q') {
            if (num1 >= 0) push(t, sqrt(num1), '#');
            else {
                return false;
            }
        }
        if (ch == 'L') {
            if (num1 > 0) push(t, log(num1), '#');
            else {
                return false;
            }
        }
        if (ch == 'A'){
            push(t, abs(num1), '#');
        }
    }
    else {
        if (t == nullptr) {
            return false;
        }
        double num2 = (pop(t)).number;
        if (ch == '+') push(t, num1 + num2, '#');
        if (ch == '-') push(t, num2 - num1, '#');
        if (ch == '/') {
            if (fabs(num1) < EPS) {
                return false;
            }
            else {
                push(t, num2 / num1, '#');
            }
        }
        if (ch == '*') push(t, num1 * num2, '#');
        if (ch == '^') push(t, pow(num2, num1), '#');
    }
    return true;
}

int proritate(char ch) {
    if (ch == 'C' || ch == 'S' || ch == 'T' || ch == 'Q' || ch == 'L'|| ch == 'A') return 4;
    if (ch == '^') return 3;
    if (ch == '*' || ch == '/') return 2;
    if (ch == '+' || ch == '-') return 1;
    if (ch == '(') return 0;
    return -1;
}

void replace_all(std::string & s, string ss, string ch) {
    int pos = s.find(ss);
    while (pos != string::npos) {
        s.replace(pos, ss.length(), ch);
        pos = s.find(ss);
    }
}

int Error(string st) {

    int brace = 0;
    for (int i = 1; i <= st.length(); i++) {
        // braces
        if (st[i - 1] == '(') brace += 1;
        if (st[i - 1] == ')') brace -= 1;
        // null
        if (st[i - 1] == '/' && st[i] == '0') return -2;

        if ((proritate(st[i - 1]) > 0 && proritate(st[i]) < 4) && (proritate(st[i]) > 0 && proritate(st[i]) < 4)) return -3;

        if (proritate(st[i - 1]) == 0 && st[i] == ')') return -5;
    }
    if (brace != 0) return -1;
    return 0;
}

bool calculate (string &s, double long &ans){

    top_finale = nullptr;
    top_put = nullptr;

    if (s[0] == '-') s.insert(0, "0");

    for (int i = 1; i <= s.length(); i++) {
        if (s[i - 1] == '(' && s[i] == '-') s.insert(i, "0");
        if (s[i - 1] == 'c') replace_all(s, "cos", "C");
        if (s[i - 1] == 's' && s[i] == 'i') replace_all(s, "sin", "S");
        if (s[i - 1] == 't') replace_all(s, "tg", "T");
        if (s[i - 1] == 's' && s[i] == 'q') replace_all(s, "sqrt", "Q");
        if (s[i - 1] == 'l') replace_all(s, "ln", "L");
        if (s[i - 1] == 'a') replace_all(s, "abs", "A");
    }
    if (Error(s) < 0) {
        return false;
    }

    for (int i = 0; i <= s.length(); i++) {
        double long num = 0;
        double long add = 0;
        long long zero_amount = 0;
        if ('0' <= s[i] && s[i] <= '9') {
            while ('0' <= s[i] && s[i] <= '9') {
                num = num * 10 + (s[i] - '0');
                i++;
            }
            push(top_finale, num, '#');
        }
        if (s[i] == '.'){
            i++;
            while ('0' <= s[i] && s[i] <= '9') {
                add = add * 10 + (s[i] - '0');
                zero_amount++;
                i++;
            }
            pop(top_finale);
            push(top_finale, num + (add/(pow(10, zero_amount))), '#');
        }
        if (s[i] == '(') {
            push(top_put, 0.0, s[i]);
        }
        else if (s[i] == ')') {
            while (top_put->znak != '(') {
                if (apply(top_finale, pop(top_put).znak) == false) return false;
            }
            pop(top_put);
        }
        else if (s[i] == '^' || s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' ||
                 s[i] == 'S' || s[i] == 'L' || s[i] == 'Q' || s[i] == 'T' || s[i] == 'C' || s[i] == 'A') {
            while (top_put != nullptr && proritate(s[i]) <= proritate(top_put->znak)) {
                if (apply(top_finale, pop(top_put).znak) == false) return false;
            }
            push(top_put, 0.0, s[i]);
        }
        else if (('a' <= s[i] && s[i] <= 'z')) return false;
    }

    while (top_put != nullptr) {
        if (apply(top_finale, pop(top_put).znak) == false) return false;
    }
    ans = top_finale->number;
    return true;
}
