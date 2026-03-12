#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// ============================================================
// 1) Observer Interface
//    - jitne bhi observers honge (Email, Mobile), sab ye follow karenge
// ============================================================
class NotificationAlertObserver {
public:
    // jab stock update hoga -> observable sabko notify karega
    virtual void update() = 0;
};


// ============================================================
// 2) Observable Interface
//    - jispe log observe kar rahe hain (Stock system)
// ============================================================
class StockObservable {
public:
    // observer register karne ke liye
    virtual void addObserver(NotificationAlertObserver* observer) = 0;

    // observer remove karne ke liye
    virtual void removeObserver(NotificationAlertObserver* observer) = 0;

    // sab observers ko inform karne ke liye
    virtual void notifyObservers() = 0;

    // stock set/update karne ke liye
    virtual void setStockCount(int newStock) = 0;

    // current stock getter
    virtual int getStockCount() = 0;
};


// ============================================================
// 3) Concrete Observable: IphoneStockObservable
//    - Ye actual product ka stock maintain karega
//    - Aur notify karega jab stock 0 -> >0 jaaye
// ============================================================
class IphoneStockObservable : public StockObservable {
private:
    vector<NotificationAlertObserver*> observersList;
    int stockCount = 0;

public:
    void addObserver(NotificationAlertObserver* observer) override {
        observersList.push_back(observer);
    }

    void removeObserver(NotificationAlertObserver* observer) override {
        // vector se remove karna
        observersList.erase(
            remove(observersList.begin(), observersList.end(), observer),
            observersList.end()
        );
    }

    void notifyObservers() override {
        for (auto observer : observersList) {
            observer->update(); // sabko update call
        }
    }

    void setStockCount(int newStock) override {
        // agar pehle stock 0 tha aur ab positive hua => notify karo
        if (stockCount == 0 && newStock > 0) {
            stockCount = newStock;
            notifyObservers();
        } else {
            stockCount = newStock;
        }
    }

    int getStockCount() override {
        return stockCount;
    }
};


// ============================================================
// 4) Concrete Observer: EmailAlertObserver
//    - Ye observer notify hote hi email bhejega (simulate print)
// ============================================================
class EmailAlertObserver : public NotificationAlertObserver {
private:
    string emailId;
    StockObservable* observable;  // jis stock ko observe kar rahe hain

public:
    EmailAlertObserver(string emailId, StockObservable* observable) {
        this->emailId = emailId;
        this->observable = observable;
    }

    void update() override {
        cout << "[EMAIL] Stock is available now! Email sent to: "
             << emailId
             << " | Current Stock: " << observable->getStockCount()
             << "\n";
    }
};


// ============================================================
// 5) Concrete Observer: MobileAlertObserver
//    - Ye observer notify hote hi mobile msg bhejega (simulate print)
// ============================================================
class MobileAlertObserver : public NotificationAlertObserver {
private:
    string phoneNumber;
    StockObservable* observable;

public:
    MobileAlertObserver(string phoneNumber, StockObservable* observable) {
        this->phoneNumber = phoneNumber;
        this->observable = observable;
    }

    void update() override {
        cout << "[MOBILE] Stock is available now! SMS sent to: "
             << phoneNumber
             << " | Current Stock: " << observable->getStockCount()
             << "\n";
    }
};


// ============================================================
// 6) main()
//    - Observable create
//    - Observers create
//    - Register observers
//    - Stock update => notify
// ============================================================
int main() {

    // 1) Observable object (iPhone stock system)
    StockObservable* iphoneStock = new IphoneStockObservable();

    // 2) Observers (email + mobile)
    NotificationAlertObserver* emailObserver1 =
        new EmailAlertObserver("rahul@gmail.com", iphoneStock);

    NotificationAlertObserver* mobileObserver1 =
        new MobileAlertObserver("9999999999", iphoneStock);

    // 3) Observers register karo
    iphoneStock->addObserver(emailObserver1);
    iphoneStock->addObserver(mobileObserver1);

    // 4) Initial stock 0 hai (kuch notify nahi hoga)
    cout << "Setting stock = 0\n";
    iphoneStock->setStockCount(0);

    // 5) Stock 10 hoga => 0 -> 10, notify hoga ✅x
    cout << "\nSetting stock = 10\n";
    iphoneStock->setStockCount(10);

    // 6) Stock 20 hoga => already >0 tha, notify nahi hoga
    cout << "\nSetting stock = 20\n";
    iphoneStock->setStockCount(20);

    // 7) Stock 0 hoga => out of stock
    cout << "\nSetting stock = 0\n";
    iphoneStock->setStockCount(0);

    // 8) Stock 5 hoga => 0 -> 5 again notify ✅
    cout << "\nSetting stock = 5\n";
    iphoneStock->setStockCount(5);

    return 0;
}
