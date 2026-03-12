#include <bits/stdc++.h>
#include <iostream>
using namespace std;


/*

Functional Requirements:-
1. Build different kind of dynamic notification -> text, timestamp e.t.c
2. send that notification to all kind of observers, like sms, email
3. 

*/


/* ============================================================
   1) Notification + Decorator
   ============================================================ */

// Interface (Java style)
class INotification {
public:
    virtual string getContent() = 0;
};

// Simple Notification (Base Component)
class SimpleNotification : public INotification {
private:
    string text;

public:
    SimpleNotification(string msg) {
        text = msg;
    }

    string getContent() override {
        return text;
    }
};

// Decorator Base
class NotificationDecorator : public INotification {
protected:
    INotification* notification;

public:
    NotificationDecorator(INotification* n) {
        notification = n;
    }
};

// Timestamp Decorator
class TimestampDecorator : public NotificationDecorator {
public:
    TimestampDecorator(INotification* n) : NotificationDecorator(n) {}

    string getContent() override {
        return "[2025-04-13 14:22:00] " + notification->getContent();
    }
};

// Signature Decorator
class SignatureDecorator : public NotificationDecorator {
private:
    string signature;

public:
    SignatureDecorator(INotification* n, string sig) : NotificationDecorator(n) {
        signature = sig;
    }

    string getContent() override {
        return notification->getContent() + "\n-- " + signature + "\n\n";
    }
};

/* ============================================================
   2) Observer Pattern
   ============================================================ */

class IObserver {
public:
    virtual void update() = 0;
};

class NotificationObservable {
private:
    vector<IObserver*> observers;
    INotification* currentNotification;

public:
    NotificationObservable() {
        currentNotification = nullptr;
    }

    void addObserver(IObserver* obs) {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) {
        vector<IObserver*> updated;
        for (auto x : observers) {
            if (x != obs) updated.push_back(x);
        }
        observers = updated;
    }

    void notifyObservers() {
        for (auto obs : observers) {
            obs->update();
        }
    }

    void setNotification(INotification* notification) {
        currentNotification = notification;
        notifyObservers();
    }

    string getNotificationContent() {
        if (currentNotification == nullptr) return "";
        return currentNotification->getContent();
    }
};

/* ============================================================
   3) NotificationService (Singleton)
   ============================================================ */

class NotificationService {
private:
    NotificationObservable* observable;

    NotificationService() {
        observable = new NotificationObservable();
    }

public:
    static NotificationService* instance;

    vector<INotification*> notifications;

    static NotificationService* getInstance() {
        if (instance == nullptr) {
            instance = new NotificationService();
        }
        return instance;
    }

    NotificationObservable* getObservable() {
        return observable;
    }

    void sendNotification(INotification* notification) {
        notifications.push_back(notification);
        observable->setNotification(notification);
    }
};

NotificationService* NotificationService::instance = nullptr;

/* ============================================================
   4) Concrete Observer 1: Logger
   ============================================================ */

class Logger : public IObserver {
private:
    NotificationObservable* observable;

public:
    Logger() {
        observable = NotificationService::getInstance()->getObservable();
        observable->addObserver(this);
    }

    void update() override {
        cout << "\n[Logger] Logging New Notification:\n";
        cout << observable->getNotificationContent();
    }
};

/* ============================================================
   5) Strategy Pattern (Notification Sending Ways)
   ============================================================ */

class INotificationStrategy {
public:
    virtual void sendNotification(string content) = 0;
};

class EmailStrategy : public INotificationStrategy {
private:
    string emailId;

public:
    EmailStrategy(string emailId) {
        this->emailId = emailId;
    }

    void sendNotification(string content) override {
        cout << "\n[Email] Sending to: " << emailId << "\n";
        cout << content;
    }
};

class SMSStrategy : public INotificationStrategy {
private:
    string mobileNumber;

public:
    SMSStrategy(string mobileNumber) {
        this->mobileNumber = mobileNumber;
    }

    void sendNotification(string content) override {
        cout << "\n[SMS] Sending to: " << mobileNumber << "\n";
        cout << content;
    }
};

class PopUpStrategy : public INotificationStrategy {
public:
    void sendNotification(string content) override {
        cout << "\n[Popup] Showing popup:\n";
        cout << content;
    }
};

/* ============================================================
   6) Concrete Observer 2: NotificationEngine
   ============================================================ */

class NotificationEngine : public IObserver {
private:
    NotificationObservable* observable;
    vector<INotificationStrategy*> strategies;

public:
    NotificationEngine() {
        observable = NotificationService::getInstance()->getObservable();
        observable->addObserver(this);
    }

    void addNotificationStrategy(INotificationStrategy* st) {
        strategies.push_back(st);
    }

    void update() override {
        string content = observable->getNotificationContent();

        cout << "\n[NotificationEngine] Dispatching notification...\n";

        for (auto st : strategies) {
            st->sendNotification(content);
        }
    }
};

/* ============================================================
   7) Main
   ============================================================ */

int main() {

    NotificationService* service = NotificationService::getInstance();

    // Observers
    Logger* logger = new Logger();

    NotificationEngine* engine = new NotificationEngine();
    engine->addNotificationStrategy(new EmailStrategy("random.person@gmail.com"));
    engine->addNotificationStrategy(new SMSStrategy("+91 9876543210"));
    engine->addNotificationStrategy(new PopUpStrategy());

    // Notification + Decorators
    INotification* notification = new SimpleNotification("Your order has been shipped!");
    notification = new TimestampDecorator(notification);
    notification = new SignatureDecorator(notification, "Customer Care");

    // Send
    service->sendNotification(notification);

    return 0;
}
