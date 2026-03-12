#include <iostream>
using namespace std;

class VendingMachine;
class State;

// ---------------- STATE INTERFACE ----------------

class State {
public:
    virtual void insertCoin(VendingMachine* vm, int amount) = 0;
    virtual void selectItem(VendingMachine* vm, int item) = 0;
    virtual void dispense(VendingMachine* vm) = 0;
    virtual void refund(VendingMachine* vm) = 0;
};

// ---------------- ITEM ----------------

class Item {
public:
    string name;
    int price;
    int quantity;

    Item(string n, int p, int q) {
        name = n;
        price = p;
        quantity = q;
    }
};

// ---------------- FORWARD STATES ----------------

class IdleState;
class HasMoneyState;
class DispenseState;
class OutOfStockState;

// ---------------- VENDING MACHINE ----------------

class VendingMachine {

public:
    State* currentState;

    State* idleState;
    State* hasMoneyState;
    State* dispenseState;
    State* outOfStockState;

    int balance;

    Item* items[3];

    VendingMachine();

    void setState(State* s) {
        currentState = s;
    }

    void insertCoin(int amount) {
        currentState->insertCoin(this, amount);
    }

    void selectItem(int item) {
        currentState->selectItem(this, item);
    }

    void dispense() {
        currentState->dispense(this);
    }

    void refund() {
        currentState->refund(this);
    }

    void showItems() {
        cout << "\nItems:\n";
        for(int i=0;i<3;i++) {
            cout << i << " "
                 << items[i]->name
                 << " Price:" << items[i]->price
                 << " Qty:" << items[i]->quantity
                 << endl;
        }
    }
};

// ---------------- IDLE STATE ----------------

class IdleState : public State {

public:

    void insertCoin(VendingMachine* vm, int amount) {
        vm->balance += amount;
        cout << "Coin inserted: " << amount << endl;

        vm->setState(vm->hasMoneyState);
    }

    void selectItem(VendingMachine*, int) {
        cout << "Insert coin first\n";
    }

    void dispense(VendingMachine*) {
        cout << "Insert coin first\n";
    }

    void refund(VendingMachine*) {
        cout << "No money to refund\n";
    }
};

// ---------------- HAS MONEY STATE ----------------

class HasMoneyState : public State {

public:

    void insertCoin(VendingMachine* vm, int amount) {
        vm->balance += amount;
        cout << "More coin inserted\n";
    }

    void selectItem(VendingMachine* vm, int item) {

        Item* it = vm->items[item];

        if(it->quantity == 0) {
            cout << "Item out of stock\n";
            vm->setState(vm->outOfStockState);
            return;
        }

        if(vm->balance < it->price) {
            cout << "Not enough balance\n";
            return;
        }

        vm->balance -= it->price;

        vm->setState(vm->dispenseState);
        vm->dispense();
    }

    void dispense(VendingMachine*) {
        cout << "Select item first\n";
    }

    void refund(VendingMachine* vm) {
        cout << "Refund: " << vm->balance << endl;
        vm->balance = 0;

        vm->setState(vm->idleState);
    }
};

// ---------------- DISPENSE STATE ----------------

class DispenseState : public State {

public:

    void insertCoin(VendingMachine*, int) {
        cout << "Wait, dispensing item\n";
    }

    void selectItem(VendingMachine*, int) {
        cout << "Already dispensing\n";
    }

    void dispense(VendingMachine* vm) {

        cout << "Select item index to dispense: ";
        int index;
        cin >> index;

        Item* it = vm->items[index];

        it->quantity--;

        cout << "Dispensed: " << it->name << endl;

        if(vm->balance > 0)
            cout << "Remaining balance: " << vm->balance << endl;

        vm->setState(vm->idleState);
    }

    void refund(VendingMachine*) {
        cout << "Cannot refund during dispensing\n";
    }
};

// ---------------- OUT OF STOCK STATE ----------------

class OutOfStockState : public State {

public:

    void insertCoin(VendingMachine*, int) {
        cout << "Machine empty\n";
    }

    void selectItem(VendingMachine*, int) {
        cout << "Machine empty\n";
    }

    void dispense(VendingMachine*) {
        cout << "Machine empty\n";
    }

    void refund(VendingMachine* vm) {
        cout << "Refund: " << vm->balance << endl;
        vm->balance = 0;
    }
};

// ---------------- CONSTRUCTOR ----------------

VendingMachine::VendingMachine() {

    idleState = new IdleState();
    hasMoneyState = new HasMoneyState();
    dispenseState = new DispenseState();
    outOfStockState = new OutOfStockState();

    currentState = idleState;

    balance = 0;

    items[0] = new Item("Coke",20,5);
    items[1] = new Item("Pepsi",25,5);
    items[2] = new Item("Chips",15,5);
}

// ---------------- MAIN ----------------

int main() {

    VendingMachine vm;

    vm.showItems();

    vm.insertCoin(50);

    vm.selectItem(0);

    vm.refund();
}