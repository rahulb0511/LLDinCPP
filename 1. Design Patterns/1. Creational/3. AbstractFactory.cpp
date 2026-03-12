#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// ABSTRACT FACTORY DESIGN PATTERN (JAVA STYLE - C++)
// =========================================================
//
// ✅ Abstract Factory creates "FAMILY OF RELATED OBJECTS"
// without specifying their concrete classes.
//
// Example Family:
// - PaymentMethod
// - ReceiptGenerator
//
// UPI Family:
// - UPIPayment
// - UPIReceipt
//
// Card Family:
// - CardPayment
// - CardReceipt
//
// Cash Family:
// - CashPayment
// - CashReceipt
//
// =========================================================


// =========================================================
// 1) ABSTRACT PRODUCTS (Interfaces)
// =========================================================

// Payment Product
class PaymentMethod {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentMethod() {}
};

// Receipt Product
class ReceiptGenerator {
public:
    virtual void generateReceipt(int amount) = 0;
    virtual ~ReceiptGenerator() {}
};


// =========================================================
// 2) CONCRETE PRODUCTS (UPI Family)
// =========================================================
class UPIPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[UPI] Paid " << amount << " via UPI\n";
    }
};

class UPIReceipt : public ReceiptGenerator {
public:
    void generateReceipt(int amount) override {
        cout << "[UPI Receipt] Receipt generated for amount: " << amount << "\n";
    }
};


// =========================================================
// 3) CONCRETE PRODUCTS (CARD Family)
// =========================================================
class CardPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[CARD] Paid " << amount << " via Card\n";
    }
};

class CardReceipt : public ReceiptGenerator {
public:
    void generateReceipt(int amount) override {
        cout << "[Card Receipt] Receipt generated for amount: " << amount << "\n";
    }
};


// =========================================================
// 4) CONCRETE PRODUCTS (CASH Family)
// =========================================================
class CashPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[CASH] Paid " << amount << " via Cash\n";
    }
};

class CashReceipt : public ReceiptGenerator {
public:
    void generateReceipt(int amount) override {
        cout << "[Cash Receipt] Receipt generated for amount: " << amount << "\n";
    }
};


// =========================================================
// 5) ABSTRACT FACTORY (Interface)
// =========================================================
class PaymentAbstractFactory {
public:
    virtual PaymentMethod* createPaymentMethod() = 0;
    virtual ReceiptGenerator* createReceiptGenerator() = 0;
    virtual ~PaymentAbstractFactory() {}
};


// =========================================================
// 6) CONCRETE FACTORIES
// =========================================================

// UPI Factory
class UPIFactory : public PaymentAbstractFactory {
public:
    PaymentMethod* createPaymentMethod() override {
        return new UPIPayment();
    }
    ReceiptGenerator* createReceiptGenerator() override {
        return new UPIReceipt();
    }
};

// Card Factory
class CardFactory : public PaymentAbstractFactory {
public:
    PaymentMethod* createPaymentMethod() override {
        return new CardPayment();
    }
    ReceiptGenerator* createReceiptGenerator() override {
        return new CardReceipt();
    }
};

// Cash Factory
class CashFactory : public PaymentAbstractFactory {
public:
    PaymentMethod* createPaymentMethod() override {
        return new CashPayment();
    }
    ReceiptGenerator* createReceiptGenerator() override {
        return new CashReceipt();
    }
};


// =========================================================
// 7) CLIENT CODE (uses factory)
// =========================================================
class PaymentService {
private:
    PaymentAbstractFactory* factory;

public:
    PaymentService(PaymentAbstractFactory* factory) {
        this->factory = factory;
    }

    void checkout(int amount) {
        PaymentMethod* payment = factory->createPaymentMethod();
        ReceiptGenerator* receipt = factory->createReceiptGenerator();

        payment->pay(amount);
        receipt->generateReceipt(amount);

        delete receipt;
        delete payment;
    }
};


// =========================================================
// ✅ MAIN
// =========================================================
int main() {
    // ✅ Runtime decide which family you want

    PaymentAbstractFactory* upiFactory = new UPIFactory();
    PaymentService* service1 = new PaymentService(upiFactory);
    service1->checkout(500);

    PaymentAbstractFactory* cardFactory = new CardFactory();
    PaymentService* service2 = new PaymentService(cardFactory);
    service2->checkout(1000);

    PaymentAbstractFactory* cashFactory = new CashFactory();
    PaymentService* service3 = new PaymentService(cashFactory);
    service3->checkout(200);

    // ✅ Cleanup
    delete service3;
    delete cashFactory;

    delete service2;
    delete cardFactory;

    delete service1;
    delete upiFactory;

    return 0;
}
