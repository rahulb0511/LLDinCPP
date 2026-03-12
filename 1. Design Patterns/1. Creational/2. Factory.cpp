#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// FACTORY DESIGN PATTERN (JAVA STYLE - C++).   Also covered Null object patterm (handling null object gracefully)
// =========================================================
//
// ✅ Client doesn't do: new UPIPayment()
// ✅ Client does: PaymentMethod* p = PaymentFactory::create("UPI");
//
// =========================================================


// =========================================================
// 1) Product Interface
// =========================================================
class PaymentMethod {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentMethod() {}
};


// =========================================================
// 2) Concrete Products
// =========================================================
class UPIPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[Factory] Paid " << amount << " via UPI\n";
    }
};

class CardPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[Factory] Paid " << amount << " via Card\n";
    }
};

class CashPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[Factory] Paid " << amount << " via Cash\n";
    }
};

class NullObj : public PaymentMethod {
public:
    void pay(int amount) override {
        cout<< "[Factory] NUll object, Please use right keyword for object creation\n";
    }
};

// =========================================================
// 3) Factory Class (creates objects)
// =========================================================
class PaymentFactory {
public:
    static PaymentMethod* create(string type) {

        if (type == "UPI") {
            return new UPIPayment();
        }
        else if (type == "CARD") {
            return new CardPayment();
        }
        else if (type == "CASH") {
            return new CashPayment();
        }

        return new NullObj(); // invalid type
    }
};


// =========================================================
// ✅ MAIN
// =========================================================
int main() {
    PaymentMethod* p1 = PaymentFactory::create("UPI");
    p1->pay(500);
    delete p1;

    PaymentMethod* p2 = PaymentFactory::create("CARD");
    p2->pay(1000);
    delete p2;

    PaymentMethod* p3 = PaymentFactory::create("CASH");
    p3->pay(200);
    delete p3;

    PaymentMethod* p4 = PaymentFactory::create("Kuch bhi");
    p4->pay(200);
    delete p4;

    return 0;
}
