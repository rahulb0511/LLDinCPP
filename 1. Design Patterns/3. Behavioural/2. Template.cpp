#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// TEMPLATE METHOD DESIGN PATTERN (JAVA STYLE - C++)
// =========================================================
//
// ✅ Base class defines TEMPLATE METHOD (fixed workflow)
// ✅ Derived classes override some steps
//
// Example:
// Beverage (template method): makeBeverage()
// Steps:
// 1) boilWater()        (common)
// 2) brew()             (custom)
// 3) pourInCup()        (common)
// 4) addCondiments()    (custom)
// =========================================================


// =========================================================
// 1) ABSTRACT BASE CLASS (Template Method)
// =========================================================
class Beverage {
public:
    // ✅ Template Method (final algorithm)
    void makeBeverage() {
        boilWater();
        brew();              // customizable step
        pourInCup();
        addCondiments();     // customizable step
        cout << "[Template] Beverage Ready ✅\n\n";
    }

    virtual ~Beverage() {}

protected:
    void boilWater() {
        cout << "[Common] Boiling Water...\n";
    }

    void pourInCup() {
        cout << "[Common] Pouring into cup...\n";
    }

    // ✅ steps that child must implement
    virtual void brew() = 0;
    virtual void addCondiments() = 0;
};


// =========================================================
// 2) CONCRETE CLASS: Tea
// =========================================================
class Tea : public Beverage {
protected:
    void brew() override {
        cout << "[Tea] Steeping tea leaves...\n";
    }

    void addCondiments() override {
        cout << "[Tea] Adding lemon...\n";
    }
};


// =========================================================
// 3) CONCRETE CLASS: Coffee
// =========================================================
class Coffee : public Beverage {
protected:
    void brew() override {
        cout << "[Coffee] Brewing coffee powder...\n";
    }

    void addCondiments() override {
        cout << "[Coffee] Adding sugar and milk...\n";
    }
};


// =========================================================
// ✅ MAIN
// =========================================================
int main() {

    // ✅ Java style
    Beverage* tea = new Tea();
    tea->makeBeverage();

    Beverage* coffee = new Coffee();
    coffee->makeBeverage();

    delete tea;
    delete coffee;

    return 0;
}
