#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/* Functional Requirements

Register users

Add rental stores with location

Add/remove vehicles in a store

Search available vehicles in a store

Create reservation for an available vehicle (pickup + return store)

Calculate rental price using pricing mode (hourly/daily/weekly)

Mark vehicle BOOKED after reservation

Process payment (card/cash/paypal)

Cancel reservation and make vehicle AVAILABLE

Complete rental and make vehicle AVAILABLE

Transfer vehicle to return store if different from pickup store
*/


/* =========================================================
   1) ENUMS (simplified)
   ========================================================= */

enum class VehicleType {
    ECONOMY,
    LUXURY,
    SUV,
    BIKE,
    AUTO
};

enum class VehicleStatus {
    AVAILABLE,
    BOOKED,
    UNAVAILABLE
};

enum class ReservationStatus {
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

enum class PricingMode {
    HOURLY,
    DAILY,
    WEEKLY
};

enum class PaymentMode {
    CREDIT_CARD,
    CASH,
    PAYPAL
};

/* =========================================================
   2) Helper functions (print friendly)
   ========================================================= */

string vehicleTypeToString(VehicleType t) {
    if (t == VehicleType::ECONOMY) return "ECONOMY";
    if (t == VehicleType::LUXURY) return "LUXURY";
    if (t == VehicleType::SUV) return "SUV";
    if (t == VehicleType::BIKE) return "BIKE";
    return "AUTO";
}

string vehicleStatusToString(VehicleStatus s) {
    if (s == VehicleStatus::AVAILABLE) return "AVAILABLE";
    if (s == VehicleStatus::BOOKED) return "BOOKED";
    return "UNAVAILABLE";
}

string reservationStatusToString(ReservationStatus s) {
    if (s == ReservationStatus::CONFIRMED) return "CONFIRMED";
    if (s == ReservationStatus::CANCELLED) return "CANCELLED";
    return "COMPLETED";
}

/* =========================================================
   3) LOCATION
   ========================================================= */

class Location {
public:
    string address;
    string city;
    string state;
    string zipCode;

    Location(string address, string city, string state, string zipCode) {
        this->address = address;
        this->city = city;
        this->state = state;
        this->zipCode = zipCode;
    }

    string toString() {
        return address + ", " + city + ", " + state + " - " + zipCode;
    }
};

/* =========================================================
   4) VEHICLE (Abstract Base Class)
   ========================================================= */

class Vehicle {
protected:
    string registrationNumber;
    string model;
    VehicleType type;
    VehicleStatus status;
    double baseRentalPrice; // base per day (simple)

public:
    Vehicle(string registrationNumber, string model, VehicleType type, double baseRentalPrice) {
        this->registrationNumber = registrationNumber;
        this->model = model;
        this->type = type;
        this->status = VehicleStatus::AVAILABLE;
        this->baseRentalPrice = baseRentalPrice;
    }

    virtual double calculateRentalFee(int days) = 0; // optional

    string getRegistrationNumber() { return registrationNumber; }
    string getModel() { return model; }
    VehicleType getType() { return type; }
    VehicleStatus getStatus() { return status; }
    void setStatus(VehicleStatus st) { status = st; }
    double getBaseRentalPrice() { return baseRentalPrice; }
};

/* =========================================================
   5) CONCRETE VEHICLES
   ========================================================= */

class EconomyVehicle : public Vehicle {
public:
    EconomyVehicle(string reg, string model, double basePrice)
        : Vehicle(reg, model, VehicleType::ECONOMY, basePrice) {}

    double calculateRentalFee(int days) override {
        return getBaseRentalPrice() * days * 1.0;
    }
};

class LuxuryVehicle : public Vehicle {
public:
    LuxuryVehicle(string reg, string model, double basePrice)
        : Vehicle(reg, model, VehicleType::LUXURY, basePrice) {}

    double calculateRentalFee(int days) override {
        return (getBaseRentalPrice() * days * 2.5) + 50.0;
    }
};

class SUVVehicle : public Vehicle {
public:
    SUVVehicle(string reg, string model, double basePrice)
        : Vehicle(reg, model, VehicleType::SUV, basePrice) {}

    double calculateRentalFee(int days) override {
        return getBaseRentalPrice() * days * 1.5;
    }
};

class BikeVehicle : public Vehicle {
public:
    BikeVehicle(string reg, string model, double basePrice)
        : Vehicle(reg, model, VehicleType::BIKE, basePrice) {}

    double calculateRentalFee(int days) override {
        return getBaseRentalPrice() * days * 0.5;
    }
};

class AutoVehicle : public Vehicle {
public:
    AutoVehicle(string reg, string model, double basePrice)
        : Vehicle(reg, model, VehicleType::AUTO, basePrice) {}

    double calculateRentalFee(int days) override {
        return getBaseRentalPrice() * days * 1.2;
    }
};

/* =========================================================
   6) VEHICLE FACTORY (Factory Pattern)
   ========================================================= */

class VehicleFactory {
public:
    static Vehicle* createVehicle(VehicleType type, string reg, string model, double basePrice) {
        if (type == VehicleType::ECONOMY) return new EconomyVehicle(reg, model, basePrice);
        if (type == VehicleType::LUXURY) return new LuxuryVehicle(reg, model, basePrice);
        if (type == VehicleType::SUV) return new SUVVehicle(reg, model, basePrice);
        if (type == VehicleType::BIKE) return new BikeVehicle(reg, model, basePrice);
        if (type == VehicleType::AUTO) return new AutoVehicle(reg, model, basePrice);

        cout << "[VehicleFactory] Unsupported vehicle type ❌\n";
        return nullptr;
    }
};

/* =========================================================
   7) RENTAL STORE
   ========================================================= */

class RentalStore {
private:
    int id;
    string name;
    Location* location;
    unordered_map<string, Vehicle*> vehicles; // regNo -> Vehicle

public:
    RentalStore(int id, string name, Location* location) {
        this->id = id;
        this->name = name;
        this->location = location;
    }

    int getId() { return id; }
    string getName() { return name; }
    Location* getLocation() { return location; }

    void addVehicle(Vehicle* vehicle) {
        vehicles[vehicle->getRegistrationNumber()] = vehicle;
    }

    void removeVehicle(string registrationNumber) {
        vehicles.erase(registrationNumber);
    }

    Vehicle* getVehicle(string registrationNumber) {
        if (vehicles.find(registrationNumber) == vehicles.end()) return nullptr;
        return vehicles[registrationNumber];
    }

    vector<Vehicle*> getAvailableVehicles() {
        vector<Vehicle*> ans;
        for (auto &it : vehicles) {
            Vehicle* v = it.second;
            if (v->getStatus() == VehicleStatus::AVAILABLE) {
                ans.push_back(v);
            }
        }
        return ans;
    }

    void printInventory() {
        cout << "\n--- Inventory of Store: " << name << " ---\n";
        cout << "Location: " << location->toString() << "\n";

        for (auto &it : vehicles) {
            Vehicle* v = it.second;
            cout << "Reg=" << v->getRegistrationNumber()
                 << " Model=" << v->getModel()
                 << " Type=" << vehicleTypeToString(v->getType())
                 << " Status=" << vehicleStatusToString(v->getStatus())
                 << " BasePrice=" << v->getBaseRentalPrice()
                 << "\n";
        }

        cout << "----------------------------------------\n";
    }
};

/* =========================================================
   8) USER
   ========================================================= */

class Reservation;

class User {
private:
    int id;
    string name;
    string email;
    vector<Reservation*> reservations;

public:
    User(int id, string name, string email) {
        this->id = id;
        this->name = name;
        this->email = email;
    }

    int getId() { return id; }
    string getName() { return name; }
    string getEmail() { return email; }

    void addReservation(Reservation* r) {
        reservations.push_back(r);
    }
};

/* =========================================================
   9) PRICING STRATEGY (Strategy + Factory)
   ========================================================= */

class PricingStrategy {
public:
    virtual double calculateRentalPrice(Vehicle* vehicle, int period) = 0;
};

class HourlyPricingStrategy : public PricingStrategy {
public:
    double calculateRentalPrice(Vehicle* vehicle, int hours) override {
        // simple assumption: base price is per day, hourly = 20% of daily per hour
        return vehicle->getBaseRentalPrice() * 0.2 * hours;
    }
};

class DailyPricingStrategy : public PricingStrategy {
public:
    double calculateRentalPrice(Vehicle* vehicle, int days) override {
        return vehicle->getBaseRentalPrice() * days;
    }
};

class WeeklyPricingStrategy : public PricingStrategy {
public:
    double calculateRentalPrice(Vehicle* vehicle, int days) override {
        double dailyRate = vehicle->getBaseRentalPrice();
        int weeks = days / 7;
        int remainingDays = days % 7;

        double weeklyPrice = (dailyRate * 7 * 0.8) * weeks; // 20% discount
        double remainingPrice = dailyRate * remainingDays;

        return weeklyPrice + remainingPrice;
    }
};

class PricingFactory {
public:
    static PricingStrategy* getPricingStrategy(PricingMode mode) {
        if (mode == PricingMode::HOURLY) return new HourlyPricingStrategy();
        if (mode == PricingMode::DAILY) return new DailyPricingStrategy();
        return new WeeklyPricingStrategy();
    }
};

/* =========================================================
   10) RESERVATION (simplified)
   - CONFIRMED / CANCELLED / COMPLETED
   ========================================================= */

class Reservation {
private:
    int id;
    User* user;
    Vehicle* vehicle;
    RentalStore* pickupStore;
    RentalStore* returnStore;

    ReservationStatus status;
    double totalAmount;

public:
    Reservation(int id, User* user, Vehicle* vehicle,
                RentalStore* pickupStore, RentalStore* returnStore,
                PricingStrategy* pricingStrategy, int pricingPeriod) {

        this->id = id;
        this->user = user;
        this->vehicle = vehicle;
        this->pickupStore = pickupStore;
        this->returnStore = returnStore;

        this->status = ReservationStatus::CONFIRMED;

        this->totalAmount = pricingStrategy->calculateRentalPrice(vehicle, pricingPeriod);

        // vehicle becomes busy as soon as reservation is confirmed
        vehicle->setStatus(VehicleStatus::BOOKED);
    }

    int getId() { return id; }
    Vehicle* getVehicle() { return vehicle; }
    double getTotalAmount() { return totalAmount; }
    ReservationStatus getStatus() { return status; }

    void cancelReservation() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::CANCELLED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);
        }
    }

    void completeRental() {
        if (status == ReservationStatus::CONFIRMED) {
            status = ReservationStatus::COMPLETED;
            vehicle->setStatus(VehicleStatus::AVAILABLE);

            // if vehicle returned to different store => move it
            if (pickupStore->getId() != returnStore->getId()) {
                pickupStore->removeVehicle(vehicle->getRegistrationNumber());
                returnStore->addVehicle(vehicle);
            }
        }
    }
};

/* =========================================================
   11) RESERVATION MANAGER
   ========================================================= */

class ReservationManager {
private:
    unordered_map<int, Reservation*> reservations;
    int nextReservationId;

public:
    ReservationManager() {
        nextReservationId = 1;
    }

    Reservation* createReservation(User* user, Vehicle* vehicle,
                                   RentalStore* pickupStore, RentalStore* returnStore,
                                   PricingStrategy* pricingStrategy, int pricingPeriod) {

        Reservation* r = new Reservation(
            nextReservationId++,
            user,
            vehicle,
            pickupStore,
            returnStore,
            pricingStrategy,
            pricingPeriod
        );

        reservations[r->getId()] = r;
        user->addReservation(r);

        return r;
    }

    Reservation* getReservation(int id) {
        if (reservations.find(id) == reservations.end()) return nullptr;
        return reservations[id];
    }

    void cancelReservation(int reservationId) {
        Reservation* r = getReservation(reservationId);
        if (r != nullptr) r->cancelReservation();
    }

    void completeRental(int reservationId) {
        Reservation* r = getReservation(reservationId);
        if (r != nullptr) r->completeRental();
    }
};

/* =========================================================
   12) PAYMENT STRATEGY + FACTORY
   ========================================================= */

class PaymentStrategy {
public:
    virtual bool processPayment(double amount) = 0;
};

class CreditCardPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        cout << "[Payment] CREDIT CARD done ✅ Amount=" << amount << "\n";
        return true;
    }
};

class CashPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        cout << "[Payment] CASH done ✅ Amount=" << amount << "\n";
        return true;
    }
};

class PaypalPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override {
        cout << "[Payment] PAYPAL done ✅ Amount=" << amount << "\n";
        return true;
    }
};

class PaymentFactory {
public:
    static PaymentStrategy* getPaymentStrategy(PaymentMode mode) {
        if (mode == PaymentMode::CREDIT_CARD) return new CreditCardPayment();
        if (mode == PaymentMode::CASH) return new CashPayment();
        return new PaypalPayment();
    }
};

/* =========================================================
   13) PAYMENT PROCESSOR
   ========================================================= */

class PaymentProcessor {
public:
    bool processPayment(double amount, PaymentStrategy* paymentStrategy) {
        return paymentStrategy->processPayment(amount);
    }
};

/* =========================================================
   14) RENTAL SYSTEM (Singleton)
   ========================================================= */

class RentalSystem {
private:
    static RentalSystem* instance;

    vector<RentalStore*> stores;
    unordered_map<int, User*> users;

    ReservationManager* reservationManager;
    PaymentProcessor* paymentProcessor;

    RentalSystem() {
        reservationManager = new ReservationManager();
        paymentProcessor = new PaymentProcessor();
    }

public:
    static RentalSystem* getInstance() {
        if (instance == nullptr) {
            instance = new RentalSystem();
        }
        return instance;
    }

    void addStore(RentalStore* store) {
        stores.push_back(store);
    }

    RentalStore* getStore(int storeId) {
        for (auto s : stores) {
            if (s->getId() == storeId) return s;
        }
        return nullptr;
    }

    void registerUser(User* user) {
        int uid = user->getId();

        if (users.find(uid) != users.end()) {
            cout << "[RentalSystem] User already exists ❌ id=" << uid << "\n";
            return;
        }

        users[uid] = user;
        cout << "[RentalSystem] User registered ✅ id=" << uid << "\n";
    }

    vector<Vehicle*> searchVehicles(int storeId) {
        RentalStore* store = getStore(storeId);
        if (store == nullptr) return {};
        return store->getAvailableVehicles();
    }

    Reservation* createReservation(int userId, string vehicleReg,
                                   int pickupStoreId, int returnStoreId,
                                   PricingMode pricingMode, int pricingPeriod) {

        if (users.find(userId) == users.end()) {
            cout << "[RentalSystem] User not found ❌\n";
            return nullptr;
        }

        User* user = users[userId];
        RentalStore* pickupStore = getStore(pickupStoreId);
        RentalStore* returnStore = getStore(returnStoreId);

        if (pickupStore == nullptr || returnStore == nullptr) {
            cout << "[RentalSystem] Store not found ❌\n";
            return nullptr;
        }

        Vehicle* vehicle = pickupStore->getVehicle(vehicleReg);

        if (vehicle == nullptr) {
            cout << "[RentalSystem] Vehicle not found in pickup store ❌\n";
            return nullptr;
        }

        if (vehicle->getStatus() != VehicleStatus::AVAILABLE) {
            cout << "[RentalSystem] Vehicle not available ❌\n";
            return nullptr;
        }

        PricingStrategy* pricingStrategy = PricingFactory::getPricingStrategy(pricingMode);

        Reservation* reservation = reservationManager->createReservation(
            user,
            vehicle,
            pickupStore,
            returnStore,
            pricingStrategy,
            pricingPeriod
        );

        cout << "[RentalSystem] Reservation created ✅ id=" << reservation->getId()
             << " Amount=" << reservation->getTotalAmount() << "\n";

        return reservation;
    }

    bool processPayment(int reservationId, PaymentMode paymentMode) {
        Reservation* reservation = reservationManager->getReservation(reservationId);

        if (reservation == nullptr) {
            cout << "[RentalSystem] Reservation not found ❌\n";
            return false;
        }

        PaymentStrategy* paymentStrategy = PaymentFactory::getPaymentStrategy(paymentMode);

        bool ok = paymentProcessor->processPayment(reservation->getTotalAmount(), paymentStrategy);

        if (ok) {
            cout << "[RentalSystem] Payment success ✅ Reservation already CONFIRMED\n";
            return true;
        }

        cout << "[RentalSystem] Payment failed ❌\n";
        return false;
    }

    void completeRental(int reservationId) {
        reservationManager->completeRental(reservationId);
        cout << "[RentalSystem] Rental completed ✅ reservationId=" << reservationId << "\n";
    }

    void cancelReservation(int reservationId) {
        reservationManager->cancelReservation(reservationId);
        cout << "[RentalSystem] Reservation cancelled ✅ reservationId=" << reservationId << "\n";
    }
};

RentalSystem* RentalSystem::instance = nullptr;

/* =========================================================
   15) MAIN (Demo)
   ========================================================= */

int main() {
    RentalSystem* rentalSystem = RentalSystem::getInstance();

    // ---- Create stores
    RentalStore* store1 = new RentalStore(
        1, "Downtown Rentals",
        new Location("123 Main St", "New York", "NY", "10001")
    );

    RentalStore* store2 = new RentalStore(
        2, "Airport Rentals",
        new Location("456 Airport Rd", "Los Angeles", "CA", "90045")
    );

    rentalSystem->addStore(store1);
    rentalSystem->addStore(store2);

    // ---- Create vehicles using Factory
    Vehicle* economyCar = VehicleFactory::createVehicle(
        VehicleType::ECONOMY, "EC001", "Toyota Corolla", 50.0
    );

    Vehicle* luxuryCar = VehicleFactory::createVehicle(
        VehicleType::LUXURY, "LX001", "Mercedes S-Class", 200.0
    );

    Vehicle* suvCar = VehicleFactory::createVehicle(
        VehicleType::SUV, "SV001", "Honda CRV", 75.0
    );

    Vehicle* bike1 = VehicleFactory::createVehicle(
        VehicleType::BIKE, "BK001", "Yamaha FZ", 20.0
    );

    // ---- Add vehicles to stores
    store1->addVehicle(economyCar);
    store1->addVehicle(luxuryCar);
    store2->addVehicle(suvCar);
    store2->addVehicle(bike1);

    store1->printInventory();
    store2->printInventory();

    // ---- Register users
    User* user1 = new User(123, "ABC", "abc@gmail.com");
    User* user2 = new User(345, "BCD", "bcd@yahoo.com");

    rentalSystem->registerUser(user1);
    rentalSystem->registerUser(user2);

    // ---- Search available vehicles in store1
    cout << "\n[Search] Available vehicles in Store1:\n";
    vector<Vehicle*> available = rentalSystem->searchVehicles(1);

    for (auto v : available) {
        cout << "Reg=" << v->getRegistrationNumber()
             << " Model=" << v->getModel()
             << " Type=" << vehicleTypeToString(v->getType())
             << " BasePrice=" << v->getBaseRentalPrice()
             << "\n";
    }

    // ---- Create reservation (Daily pricing)
    Reservation* r1 = rentalSystem->createReservation(
        user1->getId(),
        "EC001",
        1, 1,
        PricingMode::DAILY,
        4 // days
    );

    // ---- Payment for reservation
    if (r1 != nullptr) {
        cout << "\n[Payment] Paying for reservationId=" << r1->getId() << "\n";
        bool paid = rentalSystem->processPayment(r1->getId(), PaymentMode::CREDIT_CARD);

        if (paid) {
            cout << "\n[System] Completing rental...\n";
            rentalSystem->completeRental(r1->getId());
        }
    }

    // ---- Inventory after rental
    store1->printInventory();

    return 0;
}
