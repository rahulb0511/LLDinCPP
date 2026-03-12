#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =====================================================
// IMPLEMENTOR (Bridge ka 2nd part)
// =====================================================
class BreatheImplementor {
public:
    virtual void breatheProcess() = 0;
};

// Land breathing implementation
class LandBreathe : public BreatheImplementor {
public:
    void breatheProcess() override {
        cout << "[LandBreathe] Breathing through nose in air...\n";
    }
};

// Water breathing implementation
class WaterBreathe : public BreatheImplementor {
public:
    void breatheProcess() override {
        cout << "[WaterBreathe] Breathing through gills in water...\n";
    }
};

// Tree breathing implementation (optional different)
class TreeBreathe : public BreatheImplementor {
public:
    void breatheProcess() override {
        cout << "[TreeBreathe] Breathing via stomata (CO2 in, O2 out)...\n";
    }
};




// =====================================================
// ABSTRACTION (Bridge ka 1st part)
// =====================================================
class LivingThing {
protected:
    BreatheImplementor* breatheImpl;

public:
    LivingThing(BreatheImplementor* impl) {
        this->breatheImpl = impl;
    }

    virtual void breatheProcess() = 0;
};

// =====================================================
// REFINED ABSTRACTIONS
// =====================================================
class Dog : public LivingThing {
public:
    Dog(BreatheImplementor* impl) : LivingThing(impl) {}

    void breatheProcess() override {
        cout << "Dog -> ";
        breatheImpl->breatheProcess();
    }
};

class Fish : public LivingThing {
public:
    Fish(BreatheImplementor* impl) : LivingThing(impl) {}

    void breatheProcess() override {
        cout << "Fish -> ";
        breatheImpl->breatheProcess();
    }
};

class Tree : public LivingThing {
public:
    Tree(BreatheImplementor* impl) : LivingThing(impl) {}

    void breatheProcess() override {
        cout << "Tree -> ";
        breatheImpl->breatheProcess();
    }
};

// =====================================================
// CLIENT
// =====================================================
int main() {
    // implementors
    BreatheImplementor* land = new LandBreathe();
    BreatheImplementor* water = new WaterBreathe();
    BreatheImplementor* treeBreath = new TreeBreathe();

    // abstractions using implementors
    LivingThing* dog = new Dog(land);
    LivingThing* fish = new Fish(water);
    LivingThing* tree = new Tree(treeBreath);

    dog->breatheProcess();
    fish->breatheProcess();
    tree->breatheProcess();

    // cleanup (no destructors used)
    delete dog;
    delete fish;
    delete tree;

    delete land;
    delete water;
    delete treeBreath;

    return 0;
}
