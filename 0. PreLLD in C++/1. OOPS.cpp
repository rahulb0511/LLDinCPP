#include <bits/stdc++.h>
#include <iostream>
using namespace std;


//HOW TO Call parent constructor from child class -> yahan super keyword ni hota, aise hi krna pdta hai
class Parent1 {
public:
    Parent1(int x) {
        cout << "Parent1 constructor: " << x << endl;
    }
};

class Parent2 {
public:
    Parent2(string name) {
        cout << "Parent2 constructor: " << name << endl;
    }
};

class Child : public Parent1, public Parent2 {
public:
    Child(int x, string name) : Parent1(x), Parent2(name) {   // ✅ multiple parent constructors call
        cout << "Child constructor\n";
    }
};

int main() {
    Child c(10, "Rahul");
}



/*
=========================================================
    C++ LLD CODING NOTES (SINGLE FILE)
   ✅ Classes outside main (independent)
   ✅ Only 1 int main() (runs all examples)
=========================================================
*/


// =========================================================
// 1) CLASS & OBJECT
// =========================================================
class CarBasic {
public:
    string brand;
    int speed;

    void drive() {
        cout << "[Class/Object] " << brand << " driving at " << speed << " km/h\n";
    }
};


// =========================================================
// 2) ENCAPSULATION
// =========================================================
class BankAccount {
private:
    long long balance;

public:
    BankAccount(long long initialBalance) {
        balance = max(0LL, initialBalance);
    }

    void deposit(long long amount) {
        if (amount <= 0) return;
        balance += amount;
    }

    bool withdraw(long long amount) {
        if (amount <= 0) return false;
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    long long getBalance() const {
        return balance;
    }
};


// =========================================================
// 3) ABSTRACTION
// =========================================================
class CoffeeMachine {
public:
    void makeCoffee() {
        heatWater();
        grindBeans();
        brew();
        cout << "[Abstraction] Coffee ready ☕\n";
    }

private:
    void heatWater() {}
    void grindBeans() {}
    void brew() {}
};


// =========================================================
// 4) INHERITANCE (IS-A)
// =========================================================
class Vehicle {
public:
    void startEngine() { cout << "[Inheritance] Engine started\n"; }
};

class Bike : public Vehicle {
public:
    void kickStart() { cout << "[Inheritance] Bike kickstart\n"; }
};


// =========================================================
// 5) COMPOSITION (HAS-A)
// =========================================================
class Engine {
public:
    void start() { cout << "[Composition] Engine started\n"; }
};

class CarWithEngine {
private:
    Engine engine;

public:
    void startCar() {
        engine.start();
        cout << "[Composition] Car is ready\n";
    }
};


// =========================================================
// 6) INTERFACE + POLYMORPHISM
// =========================================================
class PaymentMethod {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentMethod() {} // ✅ MUST virtual
};

class UPIPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[Polymorphism] Paid " << amount << " via UPI\n";
    }
};

class CardPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[Polymorphism] Paid " << amount << " via Card\n";
    }
};

// =========================================================
// 7A) COMPILE TIME POLYMORPHISM (STATIC POLYMORPHISM)
// ---------------------------------------------------------
// ✅ Meaning:
// Function call compile time pe decide ho jaata hai
//
// ✅ Achieved by:
// 1) Function Overloading
// 2) Operator Overloading
//
// ✅ Fast (no vtable overhead)
// ✅ Decision based on function signature / parameters
// =========================================================
class Math {
public:
    // Function Overloading
    int add(int a, int b) {
        return a + b;
    }

    int add(int a, int b, int c) {
        return a + b + c;
    }

    double add(double a, double b) {
        return a + b;
    }
};

void compileTimeDemo() {
    Math m;

    cout << "[Compile Time] " << m.add(2, 3) << "\n";        // 5
    cout << "[Compile Time] " << m.add(2, 3, 4) << "\n";     // 9
    cout << "[Compile Time] " << m.add(2.5, 3.5) << "\n";    // 6.0
}


// =========================================================
// 7B) RUN TIME POLYMORPHISM (DYNAMIC POLYMORPHISM)
// ---------------------------------------------------------
// ✅ Meaning:
// Function call runtime pe decide hota hai (actual object type ke basis pe)
//
// ✅ Achieved by:
// 1) Function Overriding (inheritance required)
// 2) virtual functions required
//
// ✅ Slightly slower (vtable / dynamic dispatch)
// ✅ Most important for LLD / OOP Design
// =========================================================
class Animal {
public:
    virtual void sound() {   // ✅ virtual is important
        cout << "[Run Time] Animal sound\n";
    }

    virtual ~Animal() {}     // ✅ virtual destructor
};

class Dog : public Animal {
public:
    void sound() override {
        cout << "[Run Time] Dog barks 🐶\n";
    }
};

class Cat : public Animal {
public:
    void sound() override {
        cout << "[Run Time] Cat meows 🐱\n";
    }
};

void runTimeDemo() {
    Animal* a1 = new Dog();
    Animal* a2 = new Cat();

    a1->sound();  // Dog barks
    a2->sound();  // Cat meows

    delete a1;
    delete a2;
}

// =========================================================
// ✅ MAIN (Only one main) => sirf run examples
// =========================================================
int main() {
    cout << "✅ C++ LLD Notes Running...\n\n";

    // 1) Class/Object
    CarBasic c1;
    c1.brand = "Honda";
    c1.speed = 80;
    c1.drive();
    cout << "\n";

    // 2) Encapsulation
    BankAccount acc(1000);
    acc.deposit(500);
    acc.withdraw(200);
    cout << "[Encapsulation] Balance = " << acc.getBalance() << "\n\n";

    // 3) Abstraction
    CoffeeMachine cm;
    cm.makeCoffee();
    cout << "\n";

    // 4) Inheritance
    Bike b;
    b.startEngine();
    b.kickStart();
    cout << "\n";

    // 5) Composition
    CarWithEngine ce;
    ce.startCar();
    cout << "\n";

    // 6) Polymorphism
    PaymentMethod* p1 = new UPIPayment();
    PaymentMethod* p2 = new CardPayment();
    p1->pay(500);
    p2->pay(1200);
    delete p1;
    delete p2;
    cout << "\n";

    cout << "\n✅ Done.\n";
    return 0;
}
