#include <iostream>
using namespace std;

// ✅ 1) Prototype Interface (Java style)
class Vehicle {
public:
    virtual Vehicle* clone() = 0;
    virtual void show() = 0;
    virtual ~Vehicle() {}
};

// ✅ 2) Concrete Prototype: Car
class Car : public Vehicle {
private:
    string model;
    int price;

public:
    Car(string model, int price) {
        this->model = model;
        this->price = price;
    }

    // ✅ Copy Constructor (important for cloning)
    Car(const Car& other) {
        model = other.model;
        price = other.price;
    }

    Vehicle* clone() override {
        return new Car(*this); // ✅ clone using copy constructor
    }

    void setPrice(int newPrice) {
        price = newPrice;
    }

    void show() override {
        cout << "Car -> Model: " << model << ", Price: " << price << endl;
    }
};

// ✅ 3) Concrete Prototype: Bike
class Bike : public Vehicle {
private:
    string brand;

public:
    Bike(string brand) {
        this->brand = brand;
    }

    Bike(const Bike& other) {
        brand = other.brand;
    }

    Vehicle* clone() override {
        return new Bike(*this);
    }

    void show() override {
        cout << "Bike -> Brand: " << brand << endl;
    }
};

int main() {
    // ✅ Original Objects (Prototypes)
    Vehicle* originalCar = new Car("BMW M4", 100);
    Vehicle* originalBike = new Bike("Royal Enfield");

    cout << "Original Objects:\n";
    originalCar->show();
    originalBike->show();

    // ✅ Cloning
    Vehicle* clonedCar = originalCar->clone();
    Vehicle* clonedBike = originalBike->clone();

    cout << "\nCloned Objects:\n";
    clonedCar->show();
    clonedBike->show();

    // ✅ Modify cloned Car (to prove it's independent)
    Car* carClonePtr = dynamic_cast<Car*>(clonedCar);
    if(carClonePtr != nullptr) {
        carClonePtr->setPrice(200);
    }

    cout << "\nAfter modifying clonedCar:\n";
    cout << "Original Car:\n";
    originalCar->show();

    cout << "Cloned Car:\n";
    clonedCar->show();

    return 0;
}
