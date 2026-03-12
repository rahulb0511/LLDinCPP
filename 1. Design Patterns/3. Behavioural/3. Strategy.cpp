#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// Strategy Interface
class PaymentStrategy {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentStrategy() {}
};

// Concrete Strategy 1
class UPIPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "[Strategy] Paid " << amount << " via UPI\n";
    }
};

// Concrete Strategy 2
class CardPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "[Strategy] Paid " << amount << " via Card\n";
    }
};

// Context Class
class PaymentContext {
private:
    PaymentStrategy* strategy;

public:
    PaymentContext(PaymentStrategy* strategy) {
        this->strategy = strategy;
    }

    void makePayment(int amount) {
        strategy->pay(amount);
    }
};

int main() {
    // User chooses UPI at runtime
    PaymentStrategy* upi = new UPIPayment();
    PaymentContext* payment1 = new PaymentContext(upi);
    payment1->makePayment(500);

    // User chooses Card at runtime
    PaymentStrategy* card = new CardPayment();
    PaymentContext* payment2 = new PaymentContext(card);
    payment2->makePayment(1000);

    delete payment1;
    delete upi;
    delete payment2;
    delete card;

    return 0;
}
