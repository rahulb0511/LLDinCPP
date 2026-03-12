#include <bits/stdc++.h>
#include <iostream>
#include <thread>
using namespace std;

//FUNCTIONAL REQUIREMENTS
/*
Design a Parking Lot system that supports multiple vehicle types (2-wheeler, 4-wheeler).

At the entry gate, the system should allocate an available parking spot based on vehicle type.

If no spot is available, the system should return “Parking Full”.

After spot allocation, the system should generate a ticket containing: ticketId, vehicleNumber, entryTime, and spot details.

At the exit gate, the user provides the ticket and the system calculates parking charges using entryTime and exitTime.

The system should support multiple pricing models (e.g., hourly and per-minute).

The user should be able to pay using multiple payment modes (e.g., UPI or Credit Card).

On successful payment, the exit should be allowed and the parking spot should be marked available again.
*/


// =========================================================
// 1) VEHICLE
// vehicle -> vehicle no string, vehicle type enum (2w, 4w)
// =========================================================

enum class VehicleType {
    TWO_WHEELER,
    FOUR_WHEELER
};

class Vehicle {
public:
    string vehicleNo;
    VehicleType type;

    Vehicle(string vehicleNo, VehicleType type) {
        this->vehicleNo = vehicleNo;
        this->type = type;
    }
};

// =========================================================
// 2) PARKING SPOT
// Parking spot-> id, isempty, vehicle, price, type
// abstract price() -> two implementation -> 2w ps, 4w ps
// =========================================================

class ParkingSpot {
public:
    int id;
    bool isEmpty;
    Vehicle* vehicle;
    double price;
    VehicleType type;

    ParkingSpot(int id, VehicleType type, double price) {
        this->id = id;
        this->type = type;
        this->price = price;
        this->isEmpty = true;
        this->vehicle = nullptr;
    }

    virtual double getPrice() = 0; // abstract price()
};

class TwoWheelerParkingSpot : public ParkingSpot {
public:
    TwoWheelerParkingSpot(int id, double price)
        : ParkingSpot(id, VehicleType::TWO_WHEELER, price) {}

    double getPrice() override {
        return price;
    }
};

class FourWheelerParkingSpot : public ParkingSpot {
public:
    FourWheelerParkingSpot(int id, double price)
        : ParkingSpot(id, VehicleType::FOUR_WHEELER, price) {}

    double getPrice() override {
        return price;
    }
};

// =========================================================
// 3) TICKET
// ticket -> int entrytime (epoc), parkingspot, vehicle
// =========================================================

class Ticket {
public:
    long long entryTime; // epoch seconds
    ParkingSpot* parkingSpot;
    Vehicle* vehicle;

    Ticket(long long entryTime, ParkingSpot* ps, Vehicle* v) {
        this->entryTime = entryTime;
        this->parkingSpot = ps;
        this->vehicle = v;
    }
};

// =========================================================
// 4) PARKING SPOT MANAGER (Singleton)
// use a parkingspotmanager to manage parking spot, make it singleton
// =========================================================

class ParkingSpotManager {
private:
    static ParkingSpotManager* instance;
    static mutex mtx;

    vector<ParkingSpot*> parkingSpots;

    ParkingSpotManager() {}

public:
    static ParkingSpotManager* getInstance() {
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);
            if(instance==nullptr) {
                instance = new ParkingSpotManager();
            }
        }
        return instance;
    }

    void addParkingSpot(ParkingSpot* ps) {
        parkingSpots.push_back(ps);
    }

    ParkingSpot* findParkingSpot(VehicleType type) {
        for (auto ps : parkingSpots) {
            if (ps->isEmpty == true && ps->type == type) {
                return ps;
            }
        }
        return nullptr;
    }

    void updateParkingSpot(ParkingSpot* ps, Vehicle* v, bool makeEmpty) {
        if (makeEmpty == true) {
            ps->vehicle = nullptr;
            ps->isEmpty = true;
        } else {
            ps->vehicle = v;
            ps->isEmpty = false;
        }
    }

    void printAllSpots() {
        cout << "\n--- Parking Spots Status ---\n";
        for (auto ps : parkingSpots) {
            cout << "SpotId=" << ps->id
                 << " Type=" << (ps->type == VehicleType::TWO_WHEELER ? "2W" : "4W")
                 << " Empty=" << (ps->isEmpty ? "YES" : "NO");

            if (!ps->isEmpty && ps->vehicle != nullptr) {
                cout << " Vehicle=" << ps->vehicle->vehicleNo;
            }
            cout << "\n";
        }
        cout << "---------------------------\n";
    }
};
ParkingSpotManager* ParkingSpotManager::instance = nullptr;
mutex ParkingSpotManager::mtx;


// =========================================================
// 5) PRICING STRATEGY + FACTORY
// use pricing strategy -> hourly, minute and a factory as well
// =========================================================

enum class PricingType {
    HOURLY,
    MINUTE
};

class PricingStrategy {
public:
    virtual double calculateCost(long long entryTime, long long exitTime, double spotPrice) = 0;
};

class HourlyPricingStrategy : public PricingStrategy {
public:
    double calculateCost(long long entryTime, long long exitTime, double spotPrice) override {
        long long totalSeconds = exitTime - entryTime;
        long long hours = totalSeconds / 3600;
        if (totalSeconds % 3600 != 0) hours++;

        if (hours <= 0) hours = 1;

        return hours * spotPrice;
    }
};

class MinutePricingStrategy : public PricingStrategy {
public:
    double calculateCost(long long entryTime, long long exitTime, double spotPrice) override {
        long long totalSeconds = exitTime - entryTime;
        long long minutes = totalSeconds / 60;
        if (totalSeconds % 60 != 0) minutes++;

        if (minutes <= 0) minutes = 1;

        // example: per minute price = spotPrice / 60
        double perMinute = spotPrice / 60.0;
        return minutes * perMinute;
    }
};

class PricingStrategyFactory {
public:
    static PricingStrategy* getPricingStrategy(PricingType type) {
        if (type == PricingType::HOURLY) {
            return new HourlyPricingStrategy();
        } else {
            return new MinutePricingStrategy();
        }
    }
};

// =========================================================
// 6) PAYMENT STRATEGY + FACTORY
// use payment strategy -> cc, upi and a factory to create payment obj
// =========================================================

enum class PaymentMode {
    CREDIT_CARD,
    UPI
};

class PaymentStrategy {
public:
    virtual bool pay(double amount) = 0;
};

class CreditCardPayment : public PaymentStrategy {
public:
    bool pay(double amount) override {
        cout << "[Payment] Paid " << amount << " using CREDIT CARD ✅\n";
        return true;
    }
};

class UPIPayment : public PaymentStrategy {
public:
    bool pay(double amount) override {
        cout << "[Payment] Paid " << amount << " using UPI ✅\n";
        return true;
    }
};

class PaymentFactory {
public:
    static PaymentStrategy* getPaymentStrategy(PaymentMode mode) {
        if (mode == PaymentMode::CREDIT_CARD) {
            return new CreditCardPayment();
        } else {
            return new UPIPayment();
        }
    }
};

// =========================================================
// 7) ENTRANCE GATE (Singleton)
// entrance gate -> findps, updateps, generateticket -> singleton
// =========================================================

class EntranceGate {
private:
    static EntranceGate* instance;
    ParkingSpotManager* parkingSpotManager;

    EntranceGate() {
        parkingSpotManager = ParkingSpotManager::getInstance();
    }

public:
    static EntranceGate* getInstance() {
        if (instance == nullptr) {
            instance = new EntranceGate();
        }
        return instance;
    }

    ParkingSpot* findPS(Vehicle* vehicle) {
        return parkingSpotManager->findParkingSpot(vehicle->type);
    }

    void updatePS(ParkingSpot* ps, Vehicle* v) {
        parkingSpotManager->updateParkingSpot(ps, v, false);
    }

    Ticket* generateTicket(ParkingSpot* ps, Vehicle* v) {
        long long epoch = time(nullptr);
        Ticket* ticket = new Ticket(epoch, ps, v);
        return ticket;
    }

    Ticket* processVehicleEntry(Vehicle* v) {
        cout << "\n[EntranceGate] Vehicle arrived: " << v->vehicleNo << "\n";
        ParkingSpot* ps = findPS(v);

        if (ps == nullptr) {
            cout << "[EntranceGate] No Parking Spot Available ❌\n";
            return nullptr;
        }

        updatePS(ps, v);
        Ticket* ticket = generateTicket(ps, v);

        cout << "[EntranceGate] Ticket Generated ✅\n";
        cout << " Ticket EntryTime=" << ticket->entryTime
             << " SpotId=" << ps->id
             << " SpotType=" << (ps->type == VehicleType::TWO_WHEELER ? "2W" : "4W")
             << "\n";

        return ticket;
    }
};

EntranceGate* EntranceGate::instance = nullptr;

// =========================================================
// 8) EXIT GATE (Singleton)
// exit gate -> cost calculation, payment, update parkingspot -> singleton
// =========================================================

class ExitGate {
private:
    static ExitGate* instance;
    ParkingSpotManager* parkingSpotManager;

    ExitGate() {
        parkingSpotManager = ParkingSpotManager::getInstance();
    }

public:
    static ExitGate* getInstance() {
        if (instance == nullptr) {
            instance = new ExitGate();
        }
        return instance;
    }

    double costCalculation(Ticket* ticket, PricingType pricingType) {
        PricingStrategy* pricingStrategy = PricingStrategyFactory::getPricingStrategy(pricingType);

        long long exitTime = time(nullptr);
        double spotPrice = ticket->parkingSpot->getPrice();

        double cost = pricingStrategy->calculateCost(ticket->entryTime, exitTime, spotPrice);

        cout << "\n[ExitGate] Cost Calculation ✅\n";
        cout << " EntryTime=" << ticket->entryTime
             << " ExitTime=" << exitTime
             << " SpotPrice=" << spotPrice
             << " TotalCost=" << cost
             << "\n";

        return cost;
    }

    bool payment(double amount, PaymentMode mode) {
        PaymentStrategy* paymentStrategy = PaymentFactory::getPaymentStrategy(mode);
        return paymentStrategy->pay(amount);
    }

    void updateParkingSpot(Ticket* ticket) {
        parkingSpotManager->updateParkingSpot(ticket->parkingSpot, nullptr, true);
        cout << "[ExitGate] Parking Spot freed ✅ SpotId=" << ticket->parkingSpot->id << "\n";
    }

    void processVehicleExit(Ticket* ticket, PricingType pricingType, PaymentMode paymentMode) {
        cout << "\n[ExitGate] Vehicle exit request: " << ticket->vehicle->vehicleNo << "\n";

        double amount = costCalculation(ticket, pricingType);

        bool paid = payment(amount, paymentMode);
        if (!paid) {
            cout << "[ExitGate] Payment Failed ❌ Exit blocked\n";
            return;
        }

        updateParkingSpot(ticket);

        cout << "[ExitGate] Exit Allowed ✅\n";
    }
};

ExitGate* ExitGate::instance = nullptr;

// =========================================================
// MAIN (Demo Flow)
// Single entrance, single exit
// =========================================================

int main() {
    // Setup Parking Spots via ParkingSpotManager Singleton
    ParkingSpotManager* manager = ParkingSpotManager::getInstance();

    // Add some spots
    manager->addParkingSpot(new TwoWheelerParkingSpot(1, 20));  // 2W price = 20 per hour
    manager->addParkingSpot(new TwoWheelerParkingSpot(2, 20));

    manager->addParkingSpot(new FourWheelerParkingSpot(101, 50)); // 4W price = 50 per hour
    manager->addParkingSpot(new FourWheelerParkingSpot(102, 50));

    manager->printAllSpots();

    // Create Entrance and Exit Gate (Singleton)
    EntranceGate* entrance = EntranceGate::getInstance();
    ExitGate* exitGate = ExitGate::getInstance();

    // Vehicles
    Vehicle* bike1 = new Vehicle("KA-01-BIKE-1111", VehicleType::TWO_WHEELER);
    Vehicle* car1  = new Vehicle("KA-01-CAR-9999", VehicleType::FOUR_WHEELER);

    // Vehicle Entry
    Ticket* t1 = entrance->processVehicleEntry(bike1);
    Ticket* t2 = entrance->processVehicleEntry(car1);

    manager->printAllSpots();

    // Simulate time passing (for demo)
    cout << "\n[System] Sleeping 2 seconds to simulate parking time...\n";
    this_thread::sleep_for(chrono::seconds(2));

    // Vehicle Exit (Hourly + UPI)
    if (t1 != nullptr) {
        exitGate->processVehicleExit(t1, PricingType::HOURLY, PaymentMode::UPI);
    }

    // Vehicle Exit (Minute + Credit Card)
    if (t2 != nullptr) {
        exitGate->processVehicleExit(t2, PricingType::MINUTE, PaymentMode::CREDIT_CARD);
    }

    manager->printAllSpots();

    return 0;
}
