#include <iostream>
using namespace std;

// ✅ Subsystem 1: External Unit (Outdoor)
class ACExternalUnit {
public:
    void startCompressor() {
        cout << "External Unit: Compressor started ✅\n";
    }

    void stopCompressor() {
        cout << "External Unit: Compressor stopped ✅\n";
    }
};

// ✅ Subsystem 2: Internal Unit (Indoor)
class ACInternalUnit {
public:
    void startBlower() {
        cout << "Internal Unit: Blower started ✅\n";
    }

    void stopBlower() {
        cout << "Internal Unit: Blower stopped ✅\n";
    }

    void setModeCooling() {
        cout << "Internal Unit: Mode set to COOLING ✅\n";
    }

    void setTemperature(int temp) {
        cout << "Internal Unit: Temperature set to " << temp << "°C ✅\n";
    }
};

// ✅ Optional subsystem: Thermostat
class Thermostat {
public:
    void calibrate() {
        cout << "Thermostat: Calibrated ✅\n";
    }
};

// ✅ Optional subsystem: Power Supply
class PowerSupply {
public:
    void powerOn() {
        cout << "PowerSupply: Power ON ✅\n";
    }

    void powerOff() {
        cout << "PowerSupply: Power OFF ✅\n";
    }
};

// ✅ Facade (Simple interface for client)
class ACFacade {
private:
    ACExternalUnit* externalUnit;
    ACInternalUnit* internalUnit;
    Thermostat* thermostat;
    PowerSupply* power;

public:
    ACFacade() {
        externalUnit = new ACExternalUnit();
        internalUnit = new ACInternalUnit();
        thermostat = new Thermostat();
        power = new PowerSupply();
    }

    void turnOnAC(int temperature) {
        cout << "\n===== Turning ON AC =====\n";

        power->powerOn();
        thermostat->calibrate();

        internalUnit->setModeCooling();
        internalUnit->setTemperature(temperature);
        internalUnit->startBlower();

        externalUnit->startCompressor();

        cout << "✅ AC is now ON\n";
        cout << "=========================\n";
    }

    void turnOffAC() {
        cout << "\n===== Turning OFF AC =====\n";

        externalUnit->stopCompressor();
        internalUnit->stopBlower();

        power->powerOff();

        cout << "✅ AC is now OFF\n";
        cout << "==========================\n";
    }
};

int main() {
    ACFacade ac;

    ac.turnOnAC(22);
    ac.turnOffAC();

    return 0;
}
