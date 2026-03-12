#include <iostream>
using namespace std;

/*
    ==================================================
        1) Abstract Handler (like Java abstract class)
    ==================================================
*/
class MoneyHandler {
protected:
    MoneyHandler* nextHandler;

public:
    MoneyHandler() {
        nextHandler = nullptr;
    }

    void setNextHandler(MoneyHandler* next) {
        this->nextHandler = next;
    }

    // each handler must implement its own dispense logic
    virtual void dispense(int amount) = 0;
};

/*
    ==================================================
        2) Base Class for Note Handlers (Reusable Logic)
    ==================================================
    This avoids code duplication for 100/200/500/1000 handlers.
*/
class NoteHandler : public MoneyHandler {
protected:
    int noteValue;     // 100, 200, 500, 1000
    int numNotes;      // available notes in ATM

public:
    NoteHandler(int noteValue, int numNotes) {
        this->noteValue = noteValue;
        this->numNotes = numNotes;
    }

    void dispense(int amount) override {
        // Step 1: figure out how many notes needed
        int notesNeeded = amount / noteValue;

        // Step 2: ATM me itne notes hai ya nahi?
        if (notesNeeded > numNotes) {
            notesNeeded = numNotes;   // we can only give what we have
        }

        // Step 3: dispense these notes
        if (notesNeeded > 0) {
            cout << "Dispensing " << notesNeeded
                 << " x ₹" << noteValue << " notes.\n";
        }

        // Step 4: update available notes
        numNotes -= notesNeeded;

        // Step 5: remaining amount
        int remainingAmount = amount - (notesNeeded * noteValue);

        // Step 6: if remaining is still > 0, forward to next handler
        if (remainingAmount > 0) {
            if (nextHandler != nullptr) {
                nextHandler->dispense(remainingAmount);
            } else {
                cout << "Remaining amount ₹" << remainingAmount
                     << " cannot be fulfilled (Insufficient cash in ATM)\n";
            }
        }
    }
};

/*
    ==================================================
        3) Concrete Handlers (Java style)
    ==================================================
    Ye sirf values pass kar rahe hain base class ko.
*/

class ThousandHandler : public NoteHandler {
public:
    ThousandHandler(int numNotes) : NoteHandler(1000, numNotes) {}
};

class FiveHundredHandler : public NoteHandler {
public:
    FiveHundredHandler(int numNotes) : NoteHandler(500, numNotes) {}
};

class TwoHundredHandler : public NoteHandler {
public:
    TwoHundredHandler(int numNotes) : NoteHandler(200, numNotes) {}
};

class HundredHandler : public NoteHandler {
public:
    HundredHandler(int numNotes) : NoteHandler(100, numNotes) {}
};

/*
    ==================================================
        4) Client Code
    ==================================================
*/
int main() {
    // Step 1: Create handlers
    MoneyHandler* h1000 = new ThousandHandler(3);
    MoneyHandler* h500  = new FiveHundredHandler(5);
    MoneyHandler* h200  = new TwoHundredHandler(10);
    MoneyHandler* h100  = new HundredHandler(20);

    // Step 2: Make chain (1000 -> 500 -> 200 -> 100)
    h1000->setNextHandler(h500);
    h500->setNextHandler(h200);
    h200->setNextHandler(h100);

    // Step 3: Withdraw request
    int amountToWithdraw = 4000;
    cout << "\nDispensing amount: ₹" << amountToWithdraw << "\n";

    // Step 4: Start chain from highest note handler
    h1000->dispense(amountToWithdraw);

    return 0;
}
