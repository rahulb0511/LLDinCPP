#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/*
=========================================================
    NOTES: Virtual Function, Pure Virtual, Abstract,
           Interface in C++, Destructor (Base/Parent)
=========================================================

✅ Only ONE main
✅ All classes outside main
✅ Notes style comments included
*/


// =========================================================
// 1) VIRTUAL FUNCTION (Runtime Polymorphism)
// =========================================================
class AnimalVirtual {
public:
    virtual void speak() {
        cout << "[AnimalVirtual] Animal speaks\n";
    }

    // ✅ Destructor should be virtual when using polymorphism
    virtual ~AnimalVirtual() {
        cout << "[AnimalVirtual] Base Destructor called\n";
    }
};

class DogVirtual : public AnimalVirtual {
public:
    void speak() override {
        cout << "[DogVirtual] Dog barks\n";
    }

    ~DogVirtual() override {
        cout << "[DogVirtual] Derived Destructor called\n";
    }
};


//NOTES:- Agar virtual nhi denge to java jaisa dynamic method dispatch wala functionality ni achieve kr payenge, jaise ki
class Animala {
public:
    void speak() {
        cout << "Animal speaks\n";
    }
};

class Doga : public Animala {
public:
    void speak() {
        cout << "Dog barks\n";
    }
};

int main() {
    Animala* a = new Doga();
    a->speak();   // ❌ Animal speaks (ye print hoga) (because no virtual)
}


// =========================================================
// 2) PURE VIRTUAL FUNCTION
// =========================================================
class PaymentMethodPure {
public:
    virtual void pay(int amount) = 0;

    // ✅ Always keep virtual destructor in interfaces/abstract base
    virtual ~PaymentMethodPure() {
        cout << "[PaymentMethodPure] Base Destructor called\n";
    }
};

class UPIPaymentPure : public PaymentMethodPure {
public:
    void pay(int amount) override {
        cout << "[UPIPaymentPure] Paid " << amount << " using UPI\n";
    }

    ~UPIPaymentPure() override {
        cout << "[UPIPaymentPure] Derived Destructor called\n";
    }
};

class CardPaymentPure : public PaymentMethodPure {
public:
    void pay(int amount) override {
        cout << "[CardPaymentPure] Paid " << amount << " using Card\n";
    }

    ~CardPaymentPure() override {
        cout << "[CardPaymentPure] Derived Destructor called\n";
    }
};


// =========================================================
// 3) ABSTRACT CLASS in C++
// =========================================================
class ShapeAbstract {
protected:
    string color;

public:
    ShapeAbstract(string c) : color(c) {}

    virtual double area() = 0;

    void printColor() {
        cout << "[ShapeAbstract] Color = " << color << "\n";
    }

    virtual ~ShapeAbstract() {
        cout << "[ShapeAbstract] Base Destructor called\n";
    }
};

class CircleAbstract : public ShapeAbstract {
private:
    double r;

public:
    CircleAbstract(string c, double r) : ShapeAbstract(c), r(r) {}

    double area() override {
        return 3.14159 * r * r;
    }

    ~CircleAbstract() override {
        cout << "[CircleAbstract] Derived Destructor called\n";
    }
};


// =========================================================
// 4) INTERFACE in C++
// =========================================================
class ILoggerInterface {
public:
    virtual void info(const string& msg) = 0;
    virtual void error(const string& msg) = 0;

    // ✅ virtual destructor MUST in interface
    virtual ~ILoggerInterface() {}
};

class ConsoleLogger : public ILoggerInterface {
public:
    void info(const string& msg) override {
        cout << "[ConsoleLogger][INFO] " << msg << "\n";
    }

    void error(const string& msg) override {
        cout << "[ConsoleLogger][ERROR] " << msg << "\n";
    }

    ~ConsoleLogger() override {}
};


// =========================================================
// 5) DESTRUCTOR in C++ (Virtual kyu jruri hai?)
// =========================================================
/*
Destructor virtual kyu jruri?

✅ When we delete object using BASE CLASS POINTER:
    Base* b = new Derived();
    delete b;

To delete properly:
✅ Derived destructor should run first
✅ Then Base destructor should run

WHY?

Because derived class may own extra resources:
- memory (new / malloc)
- file handles
- sockets
- locks
- database connections

If base destructor NOT virtual:
❌ delete b calls only Base destructor
❌ Derived destructor doesn't run
❌ resource leak / incomplete cleanup

IMPORTANT C++ rule:
- If ANY function is virtual in base (polymorphic base),
  then base destructor should be virtual.
*/


// ---------------------------------------------------------
// Case A: Base destructor NOT virtual (WRONG / risky)
// ---------------------------------------------------------
class BaseNoVirtualDestructor {
public:
    // ❌ NOT virtual => problem when deleting derived via base pointer
    ~BaseNoVirtualDestructor() {
        cout << "[BaseNoVirtualDestructor] Base destructor executed\n";
    }
};

// Derived contains extra resource
class DerivedNoVirtualDestructor : public BaseNoVirtualDestructor {
private:
    int* data; // simulate resource allocated

public:
    DerivedNoVirtualDestructor() {
        data = new int[5]; // resource allocate
        cout << "[DerivedNoVirtualDestructor] Resource allocated\n";
    }

    ~DerivedNoVirtualDestructor() {
        // this SHOULD run to free memory
        delete[] data;
        cout << "[DerivedNoVirtualDestructor] Derived destructor executed (resource freed)\n";
    }
};


// ---------------------------------------------------------
// Case B: Base destructor VIRTUAL (CORRECT)
// ---------------------------------------------------------
class BaseWithVirtualDestructor {
public:
    // ✅ virtual => correct destruction chain
    virtual ~BaseWithVirtualDestructor() {
        cout << "[BaseWithVirtualDestructor] Base destructor executed\n";
    }
};

class DerivedWithVirtualDestructor : public BaseWithVirtualDestructor {
private:
    int* data;

public:
    DerivedWithVirtualDestructor() {
        data = new int[5];
        cout << "[DerivedWithVirtualDestructor] Resource allocated\n";
    }

    ~DerivedWithVirtualDestructor() override {
        delete[] data;
        cout << "[DerivedWithVirtualDestructor] Derived destructor executed (resource freed)\n";
    }
};


// =========================================================
// ✅ MAIN (Only 1) => Runs all examples
// =========================================================
int main() {
    cout << "✅ NOTES RUN START\n\n";


    // =====================================================
    // 1) Virtual function demo
    // =====================================================
    cout << "----- 1) Virtual Function Demo -----\n";
    {
        AnimalVirtual* a = new DogVirtual();
        a->speak();
        delete a; // ✅ Derived + Base both destructors run
    }
    cout << "\n";


    // =====================================================
    // 2) Pure virtual function demo
    // =====================================================
    cout << "----- 2) Pure Virtual Function Demo -----\n";
    {
        PaymentMethodPure* p1 = new UPIPaymentPure();
        PaymentMethodPure* p2 = new CardPaymentPure();
        p1->pay(500);
        p2->pay(1200);
        delete p1;
        delete p2;
    }
    cout << "\n";


    // =====================================================
    // 3) Abstract class demo
    // =====================================================
    cout << "----- 3) Abstract Class Demo -----\n";
    {
        ShapeAbstract* shape = new CircleAbstract("Blue", 2.0);
        shape->printColor();
        cout << "[CircleAbstract] Area = " << shape->area() << "\n";
        delete shape; // ✅ derived + base destructors run
    }
    cout << "\n";


    // =====================================================
    // 4) Interface demo
    // =====================================================
    cout << "----- 4) Interface Demo -----\n";
    {
        ILoggerInterface* logger = new ConsoleLogger();
        logger->info("System started");
        logger->error("Something went wrong!");
        delete logger;
    }
    cout << "\n";


    // =====================================================
    // 5) Destructor Virtual vs Non-Virtual (IMPORTANT)
    // =====================================================
    cout << "----- 5) Destructor Demo (Virtual vs Non-Virtual) -----\n";

    cout << "\n✅ Case A: Base destructor NOT virtual (WRONG)\n";
    {
        BaseNoVirtualDestructor* x = new DerivedNoVirtualDestructor();

        /*
            Here object is actually DerivedNoVirtualDestructor
            BUT pointer type is BaseNoVirtualDestructor*

            delete x; will call:
            ❌ Base destructor only
            ✅ Derived destructor will NOT run (most cases)
            => resource leak happens
        */
        delete x;
        cout << "=> Notice: Derived destructor message DID NOT appear => resource not freed ❌\n";
    }

    cout << "\n✅ Case B: Base destructor virtual (CORRECT)\n";
    {
        BaseWithVirtualDestructor* y = new DerivedWithVirtualDestructor();

        /*
            delete y; will call:
            ✅ Derived destructor first (resource freed)
            ✅ Base destructor next
            => correct cleanup ✅
        */
        delete y;
        cout << "=> Notice: Derived destructor executed => resource freed ✅\n";
    }

    cout << "\n✅ NOTES RUN END\n";
    return 0;
}
