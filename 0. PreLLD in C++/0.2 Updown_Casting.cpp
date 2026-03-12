#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// 10) UPCASTING / DOWNCASTING (OOP + Polymorphism)
// =========================================================
//
// ✅ Upcasting:
// Derived -> Base (safe)
// Mostly implicit
//
// ✅ Downcasting:
// Base -> Derived (unsafe)
// Needs cast
// Prefer dynamic_cast
// =========================================================

// Base class
class Animal {
public:
    virtual void speak() { cout << "[Animal] speaks\n"; }
    virtual ~Animal() { cout << "[Animal] destructor\n"; }
};

class Dog : public Animal {
public:
    void speak() override { cout << "[Dog] barks\n"; }
    void onlyDog() { cout << "[Dog] onlyDog() called\n"; }
    ~Dog() { cout << "[Dog] destructor\n"; }
};

class Cat : public Animal {
public:
    void speak() override { cout << "[Cat] meows\n"; }
    void onlyCat() { cout << "[Cat] onlyCat() called\n"; }
    ~Cat() { cout << "[Cat] destructor\n"; }
};

void upcastingDemo() {
    cout << "\n=== Upcasting Demo ===\n";

    Dog d;
    Animal* a = &d;  // ✅ Upcasting (Derived -> Base)
    a->speak();      // ✅ runtime polymorphism (Dog barks)
}

void downcastingDemo() {
    cout << "\n=== Downcasting Demo ===\n";

    Animal* a = new Dog();  // Upcast
    a->speak();

    // ✅ Downcast using dynamic_cast
    Dog* d = dynamic_cast<Dog*>(a);
    if (d != nullptr) {
        d->onlyDog();
    }

    delete a; // ✅ virtual destructor so Dog destructor bhi call hoga
}



// =========================================================
// 11) CASTS IN C++
// =========================================================
//
// ✅ static_cast   -> compile-time cast (fast) (no runtime check)
// ✅ dynamic_cast  -> runtime checked cast (safe for polymorphism)
// ✅ const_cast    -> remove/add constness
// ✅ reinterpret_cast -> raw unsafe bit-level cast
// =========================================================


// =========================================================
// 11A) static_cast
// ---------------------------------------------------------
// ✅ use cases:
// - normal conversions (int->double)
// - upcasting safe
// - downcasting possible but UNSAFE (no runtime check)
// =========================================================
void staticCastDemo() {
    cout << "\n=== static_cast Demo ===\n";

    double pi = 3.14;
    int x = static_cast<int>(pi);
    cout << "[static_cast] double->int: " << x << "\n";

    Animal* a = new Dog();

    // ✅ Upcast (safe)
    Animal* basePtr = static_cast<Animal*>(a);

    // ❌ Downcast allowed but UNSAFE (no checking)
    Dog* dogPtr = static_cast<Dog*>(basePtr); // might crash if not actually Dog
    dogPtr->onlyDog();

    delete a;
}


// =========================================================
// 11B) dynamic_cast
// ---------------------------------------------------------
// ✅ use cases:
// - safe downcasting in inheritance
// - requires at least 1 virtual function in base class
//
// ✅ If cast fails:
// - pointer => returns nullptr
// - reference => throws bad_cast
// =========================================================
void dynamicCastDemo() {
    cout << "\n=== dynamic_cast Demo ===\n";

    Animal* a1 = new Dog();
    Animal* a2 = new Cat();

    // ✅ correct downcast
    if (Dog* d = dynamic_cast<Dog*>(a1)) {
        d->onlyDog();
    }

    // ✅ wrong downcast (Cat to Dog) => nullptr
    if (Dog* d = dynamic_cast<Dog*>(a2)) {
        d->onlyDog();
    } else {
        cout << "[dynamic_cast] a2 is NOT a Dog\n";
    }

    delete a1;
    delete a2;

    // ✅ reference form
    cout << "\n--- dynamic_cast reference demo ---\n";
    try {
        Animal* a3 = new Cat();
        Dog& dref = dynamic_cast<Dog&>(*a3); // ❌ throws bad_cast
        dref.onlyDog();
        delete a3;
    } catch (const bad_cast& e) {
        cout << "[dynamic_cast] bad_cast caught: " << e.what() << "\n";
    }
}


// =========================================================
// 11C) const_cast
// ---------------------------------------------------------
// ✅ use case:
// - remove constness (mostly used in legacy APIs)
//
// ⚠️ WARNING:
// If the original object is actually const, modifying it => UNDEFINED BEHAVIOR
// =========================================================
void constCastDemo() {
    cout << "\n=== const_cast Demo ===\n";

    int a = 10;
    const int* p = &a;

    int* modPtr = const_cast<int*>(p); // remove const
    *modPtr = 99;

    cout << "[const_cast] a = " << a << "\n";

    // ⚠️ VERY DANGEROUS EXAMPLE:
    const int b = 50;
    const int* pb = &b;

    int* bad = const_cast<int*>(pb);
    // *bad = 100; // ❌ UB (don't do this)
    cout << "[const_cast] b still = " << b << " (modifying it is UB)\n";
}


// =========================================================
// 11D) reinterpret_cast
// ---------------------------------------------------------
// ✅ Meaning:
// - raw conversion (bit-level)
// - mostly used in low-level systems programming
//
// ⚠️ EXTREMELY UNSAFE in normal OOP usage
// =========================================================
void reinterpretCastDemo() {
    cout << "\n=== reinterpret_cast Demo ===\n";

    long long x = 123456789;
    void* vp = &x;

    long long* px = reinterpret_cast<long long*>(vp); // raw cast
    cout << "[reinterpret_cast] value = " << *px << "\n";
}



// =========================================================
// 12) OBJECT SLICING
// =========================================================
//
// ✅ Meaning:
// When derived object is copied into base object by VALUE,
// then derived part gets "cut/sliced" off.
//
// ✅ Happens when:
// Base b = Derived();
//
// ✅ Fix:
// Use references/pointers instead of object by value
// =========================================================
class Base {
public:
    virtual void show() { cout << "[Base] show\n"; }
    virtual ~Base() {}
};

class Derived : public Base {
public:
    int extra = 100;
    void show() override { cout << "[Derived] show, extra=" << extra << "\n"; }
};

void objectSlicingDemo() {
    cout << "\n=== Object Slicing Demo ===\n";

    Derived d;
    d.extra = 500;

    Base b = d;  // ❌ SLICING (Derived -> Base by value)
    b.show();    // calls Base::show OR may call Base part only (sliced)

    // ✅ correct way:
    Base& ref = d;
    ref.show();  // Derived show (polymorphism)

    Base* ptr = &d;
    ptr->show(); // Derived show
}

/*. SKIP FOR NOW:- NOT IMPORTANT FOR LLD PRACTISE

// =========================================================
// 13) FRIEND FUNCTION / FRIEND CLASS
// =========================================================
//
// ✅ friend function:
// - can access private/protected members of class
//
// ✅ friend class:
// - whole class can access private/protected of another class
//
// ⚠️ Use carefully:
// - breaks encapsulation (but sometimes needed)
// =========================================================

// ------------------------
// Friend Function Example
// ------------------------
class Box {
private:
    int secret = 42;

public:
    // friend function declaration
    friend void revealSecret(Box b);
};

void revealSecret(Box b) {
    cout << "[Friend Function] secret = " << b.secret << "\n"; // ✅ allowed
}


// ------------------------
// Friend Class Example
// ------------------------
class Account {
private:
    long long balance = 1000;

    void privateFunc() {
        cout << "[Account] privateFunc called\n";
    }

public:
    Account(long long b) : balance(b) {}

    friend class Auditor; // ✅ friend class
};

class Auditor {
public:
    void audit(Account& acc) {
        cout << "[Friend Class] balance = " << acc.balance << "\n"; // ✅ allowed
        acc.privateFunc(); // ✅ allowed
    }
};

void friendDemo() {
    cout << "\n=== Friend Demo ===\n";

    Box b;
    revealSecret(b);

    Account acc(9999);
    Auditor aud;
    aud.audit(acc);
}



// =========================================================
// 14) RULE OF 3 / RULE OF 5 / RULE OF 0
// =========================================================
//
// ✅ When a class manages resources (heap memory, file, socket etc.)
// then you must handle copy/move properly
//
// ---------------------------------------------------------
// ✅ Rule of 3 (C++03):
// If you define ANY of these:
// 1) Destructor
// 2) Copy Constructor
// 3) Copy Assignment Operator
// Then define ALL 3.
//
// ---------------------------------------------------------
// ✅ Rule of 5 (C++11+):
// Rule of 3 + move support
// 4) Move Constructor
// 5) Move Assignment Operator
//
// ---------------------------------------------------------
// ✅ Rule of 0:
// Best practice: avoid manual resource management.
// Use smart pointers / STL containers.
// Then you don't need any of these custom functions.
// =========================================================


// =========================================================
// 14A) Rule of 3 Example (Manual heap memory)
// =========================================================
class MyStringRule3 {
private:
    char* data;

public:
    // Constructor
    MyStringRule3(const char* s = "") {
        int n = strlen(s);
        data = new char[n + 1];
        strcpy(data, s);
    }

    // ✅ Destructor
    ~MyStringRule3() {
        delete[] data;
    }

    // ✅ Copy Constructor (Deep Copy)
    MyStringRule3(const MyStringRule3& other) {
        int n = strlen(other.data);
        data = new char[n + 1];
        strcpy(data, other.data);
    }

    // ✅ Copy Assignment Operator (Deep Copy)
    MyStringRule3& operator=(const MyStringRule3& other) {
        if (this == &other) return *this;

        delete[] data; // free old
        int n = strlen(other.data);
        data = new char[n + 1];
        strcpy(data, other.data);
        return *this;
    }

    void print() const {
        cout << "[Rule of 3] " << data << "\n";
    }
};

void ruleOf3Demo() {
    cout << "\n=== Rule of 3 Demo ===\n";

    MyStringRule3 a("Rahul");
    MyStringRule3 b = a;      // copy constructor
    MyStringRule3 c("Hello");
    c = a;                    // copy assignment

    a.print();
    b.print();
    c.print();
}



// =========================================================
// 14B) Rule of 5 Example (Move supported) (C++11+)
// =========================================================
class MyStringRule5 {
private:
    char* data;

public:
    // Constructor
    MyStringRule5(const char* s = "") {
        int n = strlen(s);
        data = new char[n + 1];
        strcpy(data, s);
    }

    // Destructor
    ~MyStringRule5() {
        delete[] data;
    }

    // Copy Constructor
    MyStringRule5(const MyStringRule5& other) {
        int n = strlen(other.data);
        data = new char[n + 1];
        strcpy(data, other.data);
    }

    // Copy Assignment
    MyStringRule5& operator=(const MyStringRule5& other) {
        if (this == &other) return *this;

        delete[] data;
        int n = strlen(other.data);
        data = new char[n + 1];
        strcpy(data, other.data);
        return *this;
    }

    // ✅ Move Constructor
    MyStringRule5(MyStringRule5&& other) noexcept {
        data = other.data;
        other.data = nullptr;
    }

    // ✅ Move Assignment
    MyStringRule5& operator=(MyStringRule5&& other) noexcept {
        if (this == &other) return *this;

        delete[] data;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    void print() const {
        cout << "[Rule of 5] " << (data ? data : "NULL") << "\n";
    }
};

void ruleOf5Demo() {
    cout << "\n=== Rule of 5 Demo ===\n";

    MyStringRule5 a("MoveMe");

    // move constructor
    MyStringRule5 b = std::move(a);

    b.print();
    a.print(); // becomes NULL
}


// =========================================================
// 14C) Rule of 0 Example (BEST PRACTICE ✅)
// =========================================================
//
// ✅ No custom destructor/copy/move needed because
// std::string handles memory itself
// =========================================================
class MyStringRule0 {
private:
    string data; // ✅ RAII

public:
    MyStringRule0(string s) : data(std::move(s)) {}

    void print() const {
        cout << "[Rule of 0] " << data << "\n";
    }
};

void ruleOf0Demo() {
    cout << "\n=== Rule of 0 Demo ===\n";
    MyStringRule0 s("No manual memory management");
    s.print();
}

*/

// =========================================================
// MAIN
// =========================================================
int main() {
    upcastingDemo();
    downcastingDemo();

    staticCastDemo();
    dynamicCastDemo();
    constCastDemo();
    reinterpretCastDemo();

    objectSlicingDemo();

    // friendDemo();

    // ruleOf3Demo();
    // ruleOf5Demo();
    // ruleOf0Demo();

    return 0;
}
