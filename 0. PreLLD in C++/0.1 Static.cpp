#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// 8) STATIC KEYWORD + CONST KEYWORD (C++)
// =========================================================


// =========================================================
// 8A) STATIC MEMBER VARIABLE (Class-level variable)
// ---------------------------------------------------------
// ✅ Meaning:
// - static variable "object ka nahi", "class ka" hota hai
// - sab objects same static variable share karte hain
//
// ✅ Memory:
// - ek hi copy banti hai (whole program me)
//
// ✅ Access:
// - ClassName::var se access karte hain
// - object.var se bhi access ho jaata hai (but avoid)
// =========================================================
#include <iostream>
class Counter {
public:
    static int count;  // ✅ declaration (class ke andar)

    Counter() {
        count++; // har object banne pe increment
    }
};

// ✅ definition + initialization (class ke bahar)
int Counter::count = 0;


// =========================================================
// ✅ Usage Example
// =========================================================
void staticMemberVariableDemo() {
    Counter a, b, c;

    cout << "[Static Variable] count = " << Counter::count << "\n";  
    // Output: 3
}


// =========================================================
// 8B) STATIC MEMBER FUNCTION (Class-level function)
// ---------------------------------------------------------
// ✅ Meaning:
// - static function class ka hota hai, object ka nahi
//
// ✅ Can access:
// ✅ Only static members directly
//
// ❌ Cannot access:
// ❌ non-static members (because no "this" pointer)
//
// ✅ Call:
// - ClassName::func()
// =========================================================
class Utility {
public:
    static int add(int a, int b) {
        return a + b;
    }

    // ❌ Not allowed:
    // void printX() { cout << x; }  // x non-static ho to issue
};

void staticMemberFunctionDemo() {
    cout << "[Static Method] " << Utility::add(2, 3) << "\n";
}


// =========================================================
// 8C) STATIC VARIABLE INITIALIZATION RULES
// ---------------------------------------------------------
// ✅ 1) Normal static member variable
// -> class ke bahar initialize karna padta hai
//
// class A { static int x; };
// int A::x = 10;  ✅
//
// ❌ Agar bahar define nahi kiya => LINKER ERROR aayega
// =========================================================


// =========================================================
// 8D) STATIC CONST (Integral types) can be initialized INSIDE class
// ---------------------------------------------------------
// ✅ Old rule (pre-C++17 + common interview point):
// static const int can be initialized inside class
// because it's compile-time constant
// =========================================================
class ConfigOld {
public:
    static const int MAX_USERS = 100;  // ✅ allowed
};


// =========================================================
// ✅ But if you need memory address (ODR-use), still definition needed (old compilers)
// (Not always needed in modern compilers)
//
// const int* p = &ConfigOld::MAX_USERS; // might require definition
// =========================================================


// =========================================================
// 8E) INLINE STATIC (C++17+) ✅ Best & cleanest
// ---------------------------------------------------------
// ✅ Meaning:
// - static member variable ko class ke andar hi define + initialize kar sakte ho
// - no need to write outside definition
// =========================================================
class ConfigModern {
public:
    inline static int timeout = 30;        // ✅ allowed (C++17+)
    inline static long long salary = 0;    // ✅ allowed
};


// =========================================================
// 8F) STATIC LOCAL VARIABLE (function ke andar)
// ---------------------------------------------------------
// ✅ Meaning:
// - function call hone pe baar baar create nahi hota
// - only 1 time initialize hota hai
// - value preserve hoti hai across calls
// =========================================================
int getHitCount() {
    static int hits = 0; // ✅ initialize only once
    hits++;
    return hits;
}

void staticLocalDemo() {
    cout << getHitCount() << "\n"; // 1
    cout << getHitCount() << "\n"; // 2
    cout << getHitCount() << "\n"; // 3
}


// =========================================================
// 8G) IMPORTANT: Can we initialize static directly inside class?
// ---------------------------------------------------------
// ✅ Answer:
// ❌ Normal static member variable -> NO (before C++17)
// ✅ static const integral -> YES
// ✅ inline static -> YES (C++17+)
// =========================================================


// =========================================================
// =========================================================
// 9) CONST KEYWORD (C++)
// =========================================================


// =========================================================
// 9A) CONST VARIABLE (Normal variable)
// ---------------------------------------------------------
// ✅ Meaning: value change nahi kar sakte
// =========================================================
void constVariableDemo() {
    const int x = 10;
    // x = 20; // ❌ ERROR
}


// =========================================================
// 9B) CONST POINTER vs POINTER TO CONST
// ---------------------------------------------------------
// ✅ Pointer to const (data const)
// - pointer move ho sakta hai
// - data change nahi kar sakte
// =========================================================
void pointerToConstDemo() {
    int a = 10, b = 20;

    const int* p = &a;   // ✅ data const
    // *p = 100;         // ❌ not allowed
    p = &b;              // ✅ allowed (pointer can change)
}


// ✅ Const pointer (pointer const)
// - pointer move nahi ho sakta
// - data change kar sakte ho
// =========================================================
void constPointerDemo() {
    int a = 10, b = 20;

    int* const p = &a;   // ✅ pointer const
    *p = 100;            // ✅ allowed (data change)
    // p = &b;            // ❌ not allowed
}


// ✅ Const pointer to const
// - pointer bhi fix
// - data bhi fix
// =========================================================
void constPointerToConstDemo() {
    int a = 10;
    const int* const p = &a;

    // *p = 50; // ❌
    // p = ...  // ❌
}


// =========================================================
// 9C) CONST MEMBER FUNCTION
// ---------------------------------------------------------
// ✅ Meaning:
// - function object ko modify nahi karega
// - "this" becomes: const ClassName* this
//
// ✅ Mostly used in getters
// =========================================================
class BankAccountConst {
private:
    long long balance;
    const int x;
    int y;

public:
    BankAccountConst(long long b, int axe, int wai) : x(axe), y(wai) {
        this->balance = b;
    }

    long long getBalance() const {  // ✅ const method
        return balance;
    }

    void deposit(long long amt) {   // non-const method
        balance += amt;
    }
};

void constMethodDemo() {
    const BankAccountConst acc(1000, 2, 3);

    cout << acc.getBalance() << "\n"; // ✅ allowed

    // acc.deposit(100); // ❌ ERROR (const object can't call non-const methods)
}


// =========================================================
// 9D) CONST OBJECT
// ---------------------------------------------------------
// ✅ Meaning:
// - object read-only ban jaata hai
// - sirf const member functions call kar sakte ho
// =========================================================


// =========================================================
// 9E) CONST REFERENCE (Most important in interviews)
// ---------------------------------------------------------
// ✅ Why use?
// - copy avoid hoti hai (fast)
// - safety: modify nahi kar sakte
// =========================================================
void printVector(const vector<int>& v) {  // ✅ best practice
    for (int x : v) cout << x << " ";
    cout << "\n";

    // v.push_back(10); // ❌ not allowed
}


// =========================================================
// 9F) CONST IN PARAMETERS (Best practice)
// ---------------------------------------------------------
// ✅ Use const for:
// - strings, vectors, objects pass by reference
// =========================================================


// =========================================================
// 9G) CONST IN RETURN TYPE
// ---------------------------------------------------------
// Generally return const is not very useful for primitives.
// But returning const reference can be used in getters.
//
// Example:
class Person {
private:
    string name;
public:
    Person(string n) : name(n) {}

    const string& getName() const { // ✅ returns const ref
        return name;
    }
};


// =========================================================
// ✅ Quick Interview Summary
// ---------------------------------------------------------
// STATIC:
// ✅ static variable = class shared variable
// ✅ static method = class function, no "this"
// ✅ normal static -> define outside
// ✅ static const int -> can init inside
// ✅ inline static (C++17+) -> can init inside, best
//
// CONST:
// ✅ const variable -> change nahi kar sakte
// ✅ const method -> object state change nahi karega
// ✅ const object -> only const methods allowed
// ✅ const reference -> no copy + safe
// =========================================================
 
int main () {
    
}