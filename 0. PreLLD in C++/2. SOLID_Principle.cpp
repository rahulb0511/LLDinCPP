#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// SOLID PRINCIPLES (JAVA STYLE POINTER BASED - ONE FILE)
// =========================================================
//
// ✅ Everywhere object creation like Java:
// Base* obj = new Derived();
//
// ✅ Note: C++ me manual delete important hai
// =========================================================


// =========================================================
// 1) SRP - Single Responsibility Principle
// =========================================================

// ❌ SRP Violation
class InvoiceBad {
public:
    void calculateTotal() {
        cout << "[SRP Bad] Calculating total...\n";
    }
    void saveToDB() {
        cout << "[SRP Bad] Saving invoice to DB...\n";
    }
    void printInvoice() {
        cout << "[SRP Bad] Printing invoice...\n";
    }
};

// ✅ SRP Followed
class Invoice {
public:
    void calculateTotal() {
        cout << "[SRP Good] Calculating total...\n";
    }
};

class InvoiceRepository {
public:
    void saveToDB(Invoice* inv) {
        cout << "[SRP Good] Saving invoice to DB...\n";
    }
};

class InvoicePrinter {
public:
    void print(Invoice* inv) {
        cout << "[SRP Good] Printing invoice...\n";
    }
};

void srpDemo() {
    cout << "\n=== 1) SRP Demo ===\n";

    // Bad
    InvoiceBad* bad = new InvoiceBad();
    bad->calculateTotal();
    bad->saveToDB();
    bad->printInvoice();
    delete bad;

    // Good
    Invoice* inv = new Invoice();
    InvoiceRepository* repo = new InvoiceRepository();
    InvoicePrinter* printer = new InvoicePrinter();

    inv->calculateTotal();
    repo->saveToDB(inv);
    printer->print(inv);

    delete printer;
    delete repo;
    delete inv;
}



// =========================================================
// 2) OCP - Open/Closed Principle
// =========================================================

// ❌ OCP Violation
class PaymentProcessorBad {
public:
    void pay(string method, int amount) {
        if (method == "UPI") {
            cout << "[OCP Bad] Paid " << amount << " via UPI\n";
        } else if (method == "CARD") {
            cout << "[OCP Bad] Paid " << amount << " via Card\n";
        } else if (method == "CASH") {
            cout << "[OCP Bad] Paid " << amount << " via Cash\n";
        } else {
            cout << "[OCP Bad] Invalid payment method\n";
        }
    }
};

// ✅ OCP Followed
class PaymentMethod {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentMethod() {}
};

class UPIPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[OCP Good] Paid " << amount << " via UPI\n";
    }
};

class CardPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[OCP Good] Paid " << amount << " via Card\n";
    }
};

class CashPayment : public PaymentMethod {
public:
    void pay(int amount) override {
        cout << "[OCP Good] Paid " << amount << " via Cash\n";
    }
};

class PaymentProcessorGood {
public:
    void process(PaymentMethod* method, int amount) {
        method->pay(amount);
    }
};

void ocpDemo() {
    cout << "\n=== 2) OCP Demo ===\n";

    // Bad
    PaymentProcessorBad* bad = new PaymentProcessorBad();
    bad->pay("UPI", 500);
    delete bad;

    // Good
    PaymentProcessorGood* processor = new PaymentProcessorGood();

    PaymentMethod* upi = new UPIPayment();
    PaymentMethod* card = new CardPayment();
    PaymentMethod* cash = new CashPayment();

    processor->process(upi, 500);
    processor->process(card, 1000);
    processor->process(cash, 200);

    delete cash;
    delete card;
    delete upi;
    delete processor;
}



// =========================================================
// 3) LSP - Liskov Substitution Principle
// =========================================================

// ❌ LSP Violation (Rectangle-Square issue)
class RectangleBad {
protected:
    int width, height;
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int area() const { return width * height; }
    virtual ~RectangleBad() {}
};

class SquareBad : public RectangleBad {
public:
    void setWidth(int w) override { width = w; height = w; }
    void setHeight(int h) override { width = h; height = h; }
};

void printArea(RectangleBad* r) {
    r->setWidth(5);
    r->setHeight(10);
    cout << "[LSP] Expected Area = 50, Actual Area = " << r->area() << "\n";
}

void lspBadDemo() {
    cout << "\n=== 3) LSP Bad Demo ===\n";

    RectangleBad* rect = new RectangleBad();
    SquareBad* square = new SquareBad();

    printArea(rect);   // ✅ correct
    printArea(square); // ❌ breaks expectation

    delete square;
    delete rect;
}

// ✅ LSP Followed (Better abstraction)
class Shape {
public:
    virtual int area() const = 0;
    virtual ~Shape() {}
};

class RectangleGood : public Shape {
private:
    int w, h;
public:
    RectangleGood(int w, int h) : w(w), h(h) {}
    int area() const override { return w * h; }
};

class SquareGood : public Shape {
private:
    int side;
public:
    SquareGood(int s) : side(s) {}
    int area() const override { return side * side; }
};

void lspGoodDemo() {
    cout << "\n=== 3) LSP Good Demo ===\n";

    Shape* r = new RectangleGood(5, 10);
    Shape* s = new SquareGood(5);

    cout << "[LSP Good] Rectangle area = " << r->area() << "\n";
    cout << "[LSP Good] Square area = " << s->area() << "\n";

    delete s;
    delete r;
}



// =========================================================
// 4) ISP - Interface Segregation Principle
// =========================================================

// ❌ ISP Violation
class WorkerBad {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual ~WorkerBad() {}
};

class RobotBad : public WorkerBad {
public:
    void work() override { cout << "[ISP Bad] Robot working\n"; }
    void eat() override { cout << "[ISP Bad] Robot can't eat (fake)\n"; }
    void sleep() override { cout << "[ISP Bad] Robot can't sleep (fake)\n"; }
};

// ✅ ISP Followed
class Workable {
public:
    virtual void work() = 0;
    virtual ~Workable() {}
};

class Eatable {
public:
    virtual void eat() = 0;
    virtual ~Eatable() {}
};

class Sleepable {
public:
    virtual void sleep() = 0;
    virtual ~Sleepable() {}
};

class HumanWorker : public Workable, public Eatable, public Sleepable {
public:
    void work() override { cout << "[ISP Good] Human working\n"; }
    void eat() override { cout << "[ISP Good] Human eating\n"; }
    void sleep() override { cout << "[ISP Good] Human sleeping\n"; }
};

class RobotWorker : public Workable {
public:
    void work() override { cout << "[ISP Good] Robot working\n"; }
};

void ispDemo() {
    cout << "\n=== 4) ISP Demo ===\n";

    // Bad
    WorkerBad* badRobot = new RobotBad();
    badRobot->work();
    badRobot->eat();
    badRobot->sleep();
    delete badRobot;

    // Good
    Workable* robot = new RobotWorker();
    robot->work();

    HumanWorker* human = new HumanWorker();
    human->work();
    human->eat();
    human->sleep();

    delete human;
    delete robot;
}



// =========================================================
// 5) DIP - Dependency Inversion Principle
// =========================================================

// ❌ DIP Violation
class MySQLDatabaseBad {
public:
    void save(string data) {
        cout << "[DIP Bad] Saving '" << data << "' in MySQL\n";
    }
};

class UserServiceBad {
private:
    MySQLDatabaseBad* db;
public:
    UserServiceBad() {
        db = new MySQLDatabaseBad(); // ❌ tightly coupled
    }
    void registerUser(string name) {
        db->save(name);
    }
    ~UserServiceBad() {
        delete db;
    }
};

void dipBadDemo() {
    cout << "\n=== 5) DIP Bad Demo ===\n";
    UserServiceBad* service = new UserServiceBad();
    service->registerUser("Rahul");
    delete service;
}


// ✅ DIP Followed
class IDatabase {
public:
    virtual void save(const string& data) = 0;
    virtual ~IDatabase() {}
};

class MySQLDatabaseGood : public IDatabase {
public:
    void save(const string& data) override {
        cout << "[DIP Good] Saving '" << data << "' in MySQL\n";
    }
};

class MongoDatabaseGood : public IDatabase {
public:
    void save(const string& data) override {
        cout << "[DIP Good] Saving '" << data << "' in MongoDB\n";
    }
};

class UserServiceGood {
private:
    IDatabase* db; // ✅ depends on abstraction
public:
    UserServiceGood(IDatabase* db) {
        this->db = db;
    }

    void registerUser(const string& name) {
        db->save(name);
    }
};

void dipGoodDemo() {
    cout << "\n=== 5) DIP Good Demo ===\n";

    IDatabase* mysql = new MySQLDatabaseGood();
    IDatabase* mongo = new MongoDatabaseGood();

    UserServiceGood* service1 = new UserServiceGood(mysql);
    service1->registerUser("Rahul");

    UserServiceGood* service2 = new UserServiceGood(mongo);
    service2->registerUser("Aman");

    delete service1;
    delete service2;

    delete mongo;
    delete mysql;
}



// =========================================================
// ✅ MAIN
// =========================================================
int main() {
    srpDemo();
    ocpDemo();

    lspBadDemo();
    lspGoodDemo();

    ispDemo();

    dipBadDemo();
    dipGoodDemo();

    return 0;
}
