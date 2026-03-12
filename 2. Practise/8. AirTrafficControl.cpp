#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// ======================================================
// 1) Mediator Interface
// ======================================================
class ATCMediator {
public:
    virtual void registerAircraft(class Aircraft* aircraft) = 0;
    virtual void requestLanding(Aircraft* aircraft) = 0;
    virtual void requestTakeOff(Aircraft* aircraft) = 0;
    virtual void notifyAll(string msg, Aircraft* sender) = 0;
};

// ======================================================
// 2) Colleague Abstract Class: Aircraft
// ======================================================
class Aircraft {
protected:
    string id;
    ATCMediator* tower;

public:
    Aircraft(string id, ATCMediator* tower) {
        this->id = id;
        this->tower = tower;
    }

    string getId() {
        return id;
    }

    void requestLanding() {
        cout << "\n" << id << " requesting LANDING...\n";
        tower->requestLanding(this);
    }

    void requestTakeOff() {
        cout << "\n" << id << " requesting TAKEOFF...\n";
        tower->requestTakeOff(this);
    }

    virtual void receiveMessage(string msg) = 0;
};

// ======================================================
// 3) Concrete Mediator: AirTrafficControlTower
// ======================================================
class AirTrafficControlTower : public ATCMediator {
private:
    vector<Aircraft*> aircrafts;

    // simple runway status
    bool runwayFree = true;

public:
    void registerAircraft(Aircraft* aircraft) override {
        aircrafts.push_back(aircraft);
        cout << aircraft->getId() << " registered with ATC Tower\n";
    }

    void requestLanding(Aircraft* aircraft) override {

        if (runwayFree == false) {
            aircraft->receiveMessage("LANDING DENIED ❌ Runway is busy. Please wait.");
            return;
        }

        // runway occupy
        runwayFree = false;
        aircraft->receiveMessage("LANDING APPROVED ✅ You may land now.");

        // notify others (simple broadcast)
        notifyAll("Runway is now BUSY due to LANDING of " + aircraft->getId(), aircraft);

        // after landing complete (simulate)
        cout << aircraft->getId() << " landed successfully.\n";
        runwayFree = true;

        notifyAll("Runway is now FREE ✅", aircraft);
    }

    void requestTakeOff(Aircraft* aircraft) override {

        if (runwayFree == false) {
            aircraft->receiveMessage("TAKEOFF DENIED ❌ Runway is busy. Please wait.");
            return;
        }

        // runway occupy
        runwayFree = false;
        aircraft->receiveMessage("TAKEOFF APPROVED ✅ You may takeoff now.");

        notifyAll("Runway is now BUSY due to TAKEOFF of " + aircraft->getId(), aircraft);

        // after takeoff complete (simulate)
        cout << aircraft->getId() << " took off successfully.\n";
        runwayFree = true;

        notifyAll("Runway is now FREE ✅", aircraft);
    }

    void notifyAll(string msg, Aircraft* sender) override {
        for (auto a : aircrafts) {
            if (a != sender) {
                a->receiveMessage(msg);
            }
        }
    }
};

// ======================================================
// 4) Concrete Colleague: PassengerPlane
// ======================================================
class PassengerPlane : public Aircraft {
public:
    PassengerPlane(string id, ATCMediator* tower) : Aircraft(id, tower) {}

    void receiveMessage(string msg) override {
        cout << "[" << id << " Message] " << msg << "\n";
    }
};

// ======================================================
// 5) Concrete Colleague: CargoPlane
// ======================================================
class CargoPlane : public Aircraft {
public:
    CargoPlane(string id, ATCMediator* tower) : Aircraft(id, tower) {}

    void receiveMessage(string msg) override {
        cout << "[" << id << " Message] " << msg << "\n";
    }
};

// ======================================================
// MAIN
// ======================================================
int main() {

    // Mediator (Tower)
    ATCMediator* tower = new AirTrafficControlTower();

    // Aircrafts (Colleagues)
    Aircraft* indigo101 = new PassengerPlane("Indigo-101", tower);
    Aircraft* airindia202 = new PassengerPlane("AirIndia-202", tower);
    Aircraft* cargoX55 = new CargoPlane("CargoX-55", tower);

    // Register to tower
    tower->registerAircraft(indigo101);
    tower->registerAircraft(airindia202);
    tower->registerAircraft(cargoX55);

    // Requests
    indigo101->requestLanding();
    cargoX55->requestTakeOff();
    airindia202->requestLanding();

    return 0;
}
