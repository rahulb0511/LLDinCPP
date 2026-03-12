#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/*
Functional requirements:-
1. Should support multiple providers (paytm, razorpay, e.t.c).
2. We can easily add new gateway in future
3. There should be standard payment flow with requirement validation
4. Have error handling and retries mechanism
*/


/* ============================================================
   PaymentRequest  (Data Class)
   ============================================================ */

class PaymentRequest {
public:
    string sender;
    string receiver;
    double amount;
    string currency;

    PaymentRequest(string sender, string receiver, double amount, string currency) {
        this->sender = sender;
        this->receiver = receiver;
        this->amount = amount;
        this->currency = currency;
    }
};

/* ============================================================
   BankingSystem (Strategy Interface)
   ============================================================ */

class BankingSystem {
public:
    virtual bool processPayment(double amount) = 0;
};

/* ---------------- Paytm Banking System ---------------- */

class PaytmBankingSystem : public BankingSystem {
public:
    bool processPayment(double amount) override {
        // 80% success simulation
        int r = rand() % 100;
        return r < 80;
    }
};

/* ---------------- Razorpay Banking System ---------------- */

class RazorpayBankingSystem : public BankingSystem {
public:
    bool processPayment(double amount) override {
        cout << "[BankingSystem-Razorpay] Processing payment of " << amount << "...\n";

        // 90% success simulation
        int r = rand() % 100;
        return r < 90;
    }
};

/* ============================================================
   PaymentGateway (Template Method Pattern)
   - defines fixed flow
   - child classes implement steps
   ============================================================ */

class PaymentGateway {
public:
    BankingSystem* bankingSystem;

    PaymentGateway() {
        bankingSystem = nullptr;
    }

    // Template method (same flow for every gateway)
    virtual bool processPayment(PaymentRequest* request) {
        if (!validatePayment(request)) {
            cout << "[PaymentGateway] Validation failed for " << request->sender << ".\n";
            return false;
        }

        if (!initiatePayment(request)) {
            cout << "[PaymentGateway] Initiation failed for " << request->sender << ".\n";
            return false;
        }

        if (!confirmPayment(request)) {
            cout << "[PaymentGateway] Confirmation failed for " << request->sender << ".\n";
            return false;
        }

        return true;
    }

    // steps (child class must implement)
    virtual bool validatePayment(PaymentRequest* request) = 0;
    virtual bool initiatePayment(PaymentRequest* request) = 0;
    virtual bool confirmPayment(PaymentRequest* request) = 0;
};

/* ============================================================
   PaytmGateway (Concrete Gateway)
   ============================================================ */

class PaytmGateway : public PaymentGateway {
public:
    PaytmGateway() {
        bankingSystem = new PaytmBankingSystem();
    }

    bool validatePayment(PaymentRequest* request) override {
        cout << "[Paytm] Validating payment for " << request->sender << ".\n";

        if (request->amount <= 0) return false;
        if (request->currency != "INR") return false;

        return true;
    }

    bool initiatePayment(PaymentRequest* request) override {
        cout << "[Paytm] Initiating payment of " << request->amount
             << " " << request->currency << " for " << request->sender << ".\n";

        return bankingSystem->processPayment(request->amount);
    }

    bool confirmPayment(PaymentRequest* request) override {
        cout << "[Paytm] Confirming payment for " << request->sender << ".\n";
        return true;
    }
};

/* ============================================================
   RazorpayGateway (Concrete Gateway)
   ============================================================ */

class RazorpayGateway : public PaymentGateway {
public:
    RazorpayGateway() {
        bankingSystem = new RazorpayBankingSystem();
    }

    bool validatePayment(PaymentRequest* request) override {
        cout << "[Razorpay] Validating payment for " << request->sender << ".\n";

        if (request->amount <= 0) return false;

        // currency check nahi hai in original code
        return true;
    }

    bool initiatePayment(PaymentRequest* request) override {
        cout << "[Razorpay] Initiating payment of " << request->amount
             << " " << request->currency << " for " << request->sender << ".\n";

        return bankingSystem->processPayment(request->amount);
    }

    bool confirmPayment(PaymentRequest* request) override {
        cout << "[Razorpay] Confirming payment for " << request->sender << ".\n";
        return true;
    }
};

/* ============================================================
   PaymentGatewayProxy (Proxy Pattern)
   - wraps a real gateway
   - adds retry logic
   ============================================================ */

class PaymentGatewayProxy : public PaymentGateway {
public:
    PaymentGateway* realGateway;
    int retries;

    PaymentGatewayProxy(PaymentGateway* gateway, int maxRetries) {
        realGateway = gateway;
        retries = maxRetries;
    }

    bool processPayment(PaymentRequest* request) override {
        bool result = false;

        for (int attempt = 1; attempt <= retries; attempt++) {
            if (attempt > 1) {
                cout << "[Proxy] Retrying payment (attempt " << attempt
                     << ") for " << request->sender << ".\n";
            }

            result = realGateway->processPayment(request);
            if (result) break;
        }

        if (!result) {
            cout << "[Proxy] Payment failed after " << retries
                 << " attempts for " << request->sender << ".\n";
        }

        return result;
    }

    // Proxy calls direct methods on real gateway
    bool validatePayment(PaymentRequest* request) override {
        return realGateway->validatePayment(request);
    }

    bool initiatePayment(PaymentRequest* request) override {
        return realGateway->initiatePayment(request);
    }

    bool confirmPayment(PaymentRequest* request) override {
        return realGateway->confirmPayment(request);
    }
};

/* ============================================================
   Gateway Factory (Singleton)
   ============================================================ */

enum class GatewayType {
    PAYTM,
    RAZORPAY
};

class GatewayFactory {
private:
    GatewayFactory() {}

public:
    static GatewayFactory* instance;

    static GatewayFactory* getInstance() {
        if (instance == nullptr) {
            instance = new GatewayFactory();
        }
        return instance;
    }

    PaymentGateway* getGateway(GatewayType type) {
        if (type == GatewayType::PAYTM) {
            PaymentGateway* real = new PaytmGateway();
            return new PaymentGatewayProxy(real, 3);
        } else {
            PaymentGateway* real = new RazorpayGateway();
            return new PaymentGatewayProxy(real, 1);
        }
    }
};

GatewayFactory* GatewayFactory::instance = nullptr;

/* ============================================================
   PaymentService (Singleton)
   - holds selected gateway
   ============================================================ */

class PaymentService {
private:
    PaymentService() {
        gateway = nullptr;
    }

public:
    static PaymentService* instance;
    PaymentGateway* gateway;

    static PaymentService* getInstance() {
        if (instance == nullptr) {
            instance = new PaymentService();
        }
        return instance;
    }

    void setGateway(PaymentGateway* g) {
        gateway = g;
    }

    bool processPayment(PaymentRequest* request) {
        if (gateway == nullptr) {
            cout << "[PaymentService] No payment gateway selected.\n";
            return false;
        }

        return gateway->processPayment(request);
    }
};

PaymentService* PaymentService::instance = nullptr;

/* ============================================================
   PaymentController (Singleton)
   - entry point for client
   ============================================================ */

class PaymentController {
private:
    PaymentController() {}

public:
    static PaymentController* instance;

    static PaymentController* getInstance() {
        if (instance == nullptr) {
            instance = new PaymentController();
        }
        return instance;
    }

    bool handlePayment(GatewayType type, PaymentRequest* req) {
        PaymentGateway* gateway = GatewayFactory::getInstance()->getGateway(type);
        PaymentService::getInstance()->setGateway(gateway);

        return PaymentService::getInstance()->processPayment(req);
    }
};

PaymentController* PaymentController::instance = nullptr;

/* ============================================================
   Main
   ============================================================ */

int main() {
    srand(time(nullptr));

    PaymentRequest* req1 = new PaymentRequest("Aditya", "Shubham", 1000.0, "INR");

    cout << "Processing via Paytm\n";
    cout << "------------------------------\n";
    bool res1 = PaymentController::getInstance()->handlePayment(GatewayType::PAYTM, req1);
    cout << "Result: " << (res1 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------------\n\n";

    PaymentRequest* req2 = new PaymentRequest("Shubham", "Aditya", 500.0, "USD");

    cout << "Processing via Razorpay\n";
    cout << "------------------------------\n";
    bool res2 = PaymentController::getInstance()->handlePayment(GatewayType::RAZORPAY, req2);
    cout << "Result: " << (res2 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------------\n";

    return 0;
}
