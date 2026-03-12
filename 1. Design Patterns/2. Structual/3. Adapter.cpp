#include <iostream>
using namespace std;

// ✅ Target Interface (Client expects weight in KG)
class WeightMachineInKg {
public:
    virtual double getWeightInKg() = 0;
};

class WeightMachineInPoundAbst {
public:
    virtual double getWeightInPound()=0;
};

// ✅ Adaptee (Existing machine gives weight in POUNDS)
class WeightMachineInPound : public WeightMachineInPoundAbst{
public:
    double getWeightInPound() override {
        return 180.0;  // suppose machine gives 180 pounds
    }
};

// ✅ Adapter (Converts Pound -> Kg)
class WeightMachineAdapter : public WeightMachineInKg {
private:
    WeightMachineInPoundAbst* weightMachine;

public:
    WeightMachineAdapter(WeightMachineInPoundAbst* machine) {
        weightMachine = machine;
    }

    double getWeightInKg() override {
        double pounds = weightMachine->getWeightInPound();
        double kg = pounds * 0.453592;  // conversion formula
        return kg;
    }
};

int main() {
    // Old weight machine (Pounds)
    WeightMachineInPoundAbst* poundMachine = new WeightMachineInPound();

    // Adapter makes it usable in KG
    WeightMachineInKg* kgMachine = new WeightMachineAdapter(poundMachine);

    cout << "Weight in KG: " << kgMachine->getWeightInKg() << " kg\n";

    return 0;
}
