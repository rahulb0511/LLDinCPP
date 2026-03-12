#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// ==========================
// 1) Component Interface
// ==========================
class Pizza {
public:
    virtual int cost() = 0;
};

// ==========================
// 2) Base Pizza: Margherita
// ==========================
class Margherita : public Pizza {
public:
    int cost() override {
        return 200;
    }
};

// ==========================
// 3) Base Pizza: Veggie Delight
// ==========================
class VeggieDelight : public Pizza {
public:
    int cost() override {
        return 250;
    }
};

// ==========================
// 4) Decorator Base Class
//    - Ye bhi Pizza hi hai
//    - Aur iske andar ek Pizza wrapped hota hai
// ==========================
class ToppingDecorator : public Pizza {
protected:
    Pizza* pizza;  // wrapped object

public:
    ToppingDecorator(Pizza* pizza) {
        this->pizza = pizza;
    }
};

// ==========================
// 5) Concrete Decorator: Extra Cheese
// ==========================
class ExtraCheese : public ToppingDecorator {
public:
    ExtraCheese(Pizza* pizza) : ToppingDecorator(pizza) {}

    int cost() override {
        return pizza->cost() + 50;
    }
};

// ==========================
// 6) Concrete Decorator: Olive
// ==========================
class Olive : public ToppingDecorator {
public:
    Olive(Pizza* pizza) : ToppingDecorator(pizza) {}

    int cost() override {
        return pizza->cost() + 30;
    }
};

// ==========================
// MAIN
// ==========================
int main() {

    // Base Margherita
    Pizza* p1 = new Margherita();
    cout << "Margherita cost = " << p1->cost() << "\n";

    // Margherita + Extra Cheese
    Pizza* p2 = new ExtraCheese(new Margherita());
    cout << "Margherita + ExtraCheese cost = " << p2->cost() << "\n";

    // Veggie Delight + Olive
    Pizza* p3 = new Olive(new VeggieDelight());
    cout << "VeggieDelight + Olive cost = " << p3->cost() << "\n";

    // Veggie Delight + Extra Cheese + Olive
    Pizza* p4 = new Olive(new ExtraCheese(new VeggieDelight()));
    cout << "VeggieDelight + ExtraCheese + Olive cost = " << p4->cost() << "\n";

    return 0;
}
