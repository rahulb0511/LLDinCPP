#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/*

Functional Requirements


System building ke andar multiple floors ko support karega.

Har floor par UP / DOWN button hoga jo external request generate karega.

External request me floor number aur direction hoga.

System multiple elevators ko support karega.

Har elevator ka unique ID aur current floor hoga.

Elevator ke states honge: IDLE aur MOVING.

External requests dispatcher ke through elevators ko assign hongi.

Odd floors odd elevators aur even floors even elevators ko assign honge.

Elevator ke andar internal destination buttons honge.

Internal button press hone par internal request generate hogi.

Elevator controller UP requests ko min-heap aur DOWN requests ko max-heap me manage karega.

Elevator destination floor tak move karega aur display update karega.

*/

/* ===========================================================
   Forward Declarations
   =========================================================== */
class Floor;
class Building;
class ElevatorCar;
class ElevatorController;
class ExternalDispatcher;
class InternalDispatcher;
class ElevatorCreator;

/* ===========================================================
   Enums (Java enum style)
   =========================================================== */

enum class Direction {
    UP,
    DOWN
};

enum class ElevatorState {
    MOVING,
    IDLE
};

/* ===========================================================
   ElevatorDisplay
   =========================================================== */

class ElevatorDisplay {
public:
    int floor = 0;
    Direction direction = Direction::UP;

    void setDisplay(int f, Direction d) {
        floor = f;
        direction = d;
    }

    void showDisplay() {
        cout << "Display Floor: " << floor << "\n";
        cout << "Display Direction: " << (direction == Direction::UP ? "UP" : "DOWN") << "\n";
    }
};

/* ===========================================================
   ElevatorDoor
   =========================================================== */

class ElevatorDoor {
public:
    void openDoor() {
        cout << "Opening the Elevator door\n";
    }

    void closeDoor() {
        cout << "Closing the Elevator door\n";
    }
};

/* ===========================================================
   InternalButtons
   =========================================================== */

class InternalButtons {
public:
    InternalDispatcher* dispatcher; // we will set it after dispatcher class defined
    vector<int> availableButtons;
    int buttonSelected = -1;

    InternalButtons() {
        availableButtons = {1,2,3,4,5,6,7,8,9,10};
        dispatcher = nullptr; // will assign later
    }

    void pressButton(int destination, ElevatorCar* elevatorCar);
};

/* ===========================================================
   ElevatorCar
   =========================================================== */

class ElevatorCar {
public:
    int id = 0;

    ElevatorDisplay display;
    InternalButtons internalButtons;

    ElevatorState elevatorState;
    int currentFloor;
    Direction elevatorDirection;
    ElevatorDoor elevatorDoor;

    ElevatorCar() {
        elevatorState = ElevatorState::IDLE;
        currentFloor = 0;
        elevatorDirection = Direction::UP;
    }

    void showDisplay() {
        display.showDisplay();
    }

    void pressButton(int destination) {
        internalButtons.pressButton(destination, this);
    }

    void setDisplay() {
        display.setDisplay(currentFloor, elevatorDirection);
    }

    bool moveElevator(Direction dir, int destinationFloor) {
        int startFloor = currentFloor;

        if (dir == Direction::UP) {
            for (int i = startFloor; i <= destinationFloor; i++) {
                currentFloor = i;
                setDisplay();
                showDisplay();

                if (i == destinationFloor) {
                    return true;
                }
            }
        }

        if (dir == Direction::DOWN) {
            for (int i = startFloor; i >= destinationFloor; i--) {
                currentFloor = i;
                setDisplay();
                showDisplay();

                if (i == destinationFloor) {
                    return true;
                }
            }
        }

        return false;
    }
};

/* ===========================================================
   ElevatorController
   =========================================================== */

class ElevatorController {
public:
    // UP requests -> min heap
    priority_queue<int, vector<int>, greater<int>> upMinPQ;

    // DOWN requests -> max heap
    priority_queue<int> downMaxPQ;

    ElevatorCar* elevatorCar;

    ElevatorController(ElevatorCar* car) {
        elevatorCar = car;
    }

    void submitExternalRequest(int floor, Direction direction) {
        if (direction == Direction::DOWN) {
            downMaxPQ.push(floor);
        } else {
            upMinPQ.push(floor);
        }
    }

    void submitInternalRequest(int floor) {
        // Not implemented in given Java code
        // But place holder hai
    }

    void controlElevator() {
        // Given Java code me while(true) incomplete hai.
        // To yaha bhi simple placeholder.
        while (false) {
            if (elevatorCar->elevatorDirection == Direction::UP) {
                // future logic
            }
        }
    }
};

/* ===========================================================
   ElevatorCreator (Static Elevator list)
   =========================================================== */

class ElevatorCreator {
public:
    static vector<ElevatorController*> elevatorControllerList;

    static void init() {
        if (!elevatorControllerList.empty()) return;

        ElevatorCar* elevatorCar1 = new ElevatorCar();
        elevatorCar1->id = 1;
        ElevatorController* controller1 = new ElevatorController(elevatorCar1);

        ElevatorCar* elevatorCar2 = new ElevatorCar();
        elevatorCar2->id = 2;
        ElevatorController* controller2 = new ElevatorController(elevatorCar2);

        elevatorControllerList.push_back(controller1);
        elevatorControllerList.push_back(controller2);
    }
};

vector<ElevatorController*> ElevatorCreator::elevatorControllerList;

/* ===========================================================
   ExternalDispatcher
   =========================================================== */

class ExternalDispatcher {
public:
    vector<ElevatorController*> elevatorControllerList;

    ExternalDispatcher() {
        ElevatorCreator::init();
        elevatorControllerList = ElevatorCreator::elevatorControllerList;
    }

    void submitExternalRequest(int floor, Direction direction) {
        for (auto elevatorController : elevatorControllerList) {
            int elevatorID = elevatorController->elevatorCar->id;

            if (elevatorID % 2 == 1 && floor % 2 == 1) {
                elevatorController->submitExternalRequest(floor, direction);
                cout << "[ExternalDispatcher] Assigned floor " << floor
                     << " request to Elevator " << elevatorID << "\n";
            } 
            else if (elevatorID % 2 == 0 && floor % 2 == 0) {
                elevatorController->submitExternalRequest(floor, direction);
                cout << "[ExternalDispatcher] Assigned floor " << floor
                     << " request to Elevator " << elevatorID << "\n";
            }
        }
    }
};

/* ===========================================================
   InternalDispatcher
   =========================================================== */

class InternalDispatcher {
public:
    vector<ElevatorController*> elevatorControllerList;

    InternalDispatcher() {
        ElevatorCreator::init();
        elevatorControllerList = ElevatorCreator::elevatorControllerList;
    }

    void submitInternalRequest(int floor, ElevatorCar* elevatorCar) {
        // Java code empty tha
        // yaha bhi empty
        cout << "[InternalDispatcher] Internal request for floor "
             << floor << " from Elevator " << elevatorCar->id << "\n";
    }
};

/* ===========================================================
   InternalButtons Implementation (after InternalDispatcher)
   =========================================================== */

void InternalButtons::pressButton(int destination, ElevatorCar* elevatorCar) {
    if (dispatcher == nullptr) {
        dispatcher = new InternalDispatcher();
    }
    dispatcher->submitInternalRequest(destination, elevatorCar);
}

/* ===========================================================
   Floor
   =========================================================== */

class Floor {
public:
    int floorNumber;
    ExternalDispatcher externalDispatcher;

    Floor(int num) {
        floorNumber = num;
    }

    void pressButton(Direction direction) {
        cout << "[Floor] Floor " << floorNumber << " pressed "
             << (direction == Direction::UP ? "UP" : "DOWN") << "\n";

        externalDispatcher.submitExternalRequest(floorNumber, direction);
    }
};

/* ===========================================================
   Building
   =========================================================== */

class Building {
public:
    vector<Floor*> floorList;

    Building(vector<Floor*> floors) {
        floorList = floors;
    }

    void addFloors(Floor* newFloor) {
        floorList.push_back(newFloor);
    }

    void removeFloors(Floor* removeFloor) {
        // remove pointer match
        floorList.erase(remove(floorList.begin(), floorList.end(), removeFloor), floorList.end());
    }

    vector<Floor*> getAllFloorList() {
        return floorList;
    }
};

/* ===========================================================
   Main
   =========================================================== */

int main() {
    vector<Floor*> floorList;

    Floor* floor1 = new Floor(1);
    Floor* floor2 = new Floor(2);
    Floor* floor3 = new Floor(3);
    Floor* floor4 = new Floor(4);
    Floor* floor5 = new Floor(5);

    floorList.push_back(floor1);
    floorList.push_back(floor2);
    floorList.push_back(floor3);
    floorList.push_back(floor4);
    floorList.push_back(floor5);

    Building* building = new Building(floorList);

    cout << "Building created with floors: ";
    for (auto f : building->getAllFloorList()) {
        cout << f->floorNumber << " ";
    }
    cout << "\n\n";

    // Demo: press buttons
    floor3->pressButton(Direction::UP);
    floor4->pressButton(Direction::DOWN);
    floor1->pressButton(Direction::UP);

    return 0;
}
