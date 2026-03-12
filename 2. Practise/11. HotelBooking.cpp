#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/* ============================================================
   ENUMS
   ============================================================ */

enum class RoomType {
    SINGLE,
    DOUBLE,
    DELUXE
};

enum class RoomStatus {
    AVAILABLE,
    BOOKED,
    MAINTENANCE
};

enum class BookingStatus {
    CONFIRMED,
    CANCELLED
};

enum class PaymentStatus {
    SUCCESS,
    FAILED
};

/* ============================================================
   Helper convertors (for printing)
   ============================================================ */

string roomTypeToString(RoomType t) {
    if (t == RoomType::SINGLE) return "SINGLE";
    if (t == RoomType::DOUBLE) return "DOUBLE";
    return "DELUXE";
}

string roomStatusToString(RoomStatus s) {
    if (s == RoomStatus::AVAILABLE) return "AVAILABLE";
    if (s == RoomStatus::BOOKED) return "BOOKED";
    return "MAINTENANCE";
}

string bookingStatusToString(BookingStatus s) {
    if (s == BookingStatus::CONFIRMED) return "CONFIRMED";
    return "CANCELLED";
}

/* ============================================================
   User
   ============================================================ */

class User {
public:
    string userId;
    string name;
    string phone;

    User(string userId, string name, string phone) {
        this->userId = userId;
        this->name = name;
        this->phone = phone;
    }
};

/* ============================================================
   Room
   ============================================================ */

class Room {
public:
    string roomId;      // e.g. "R101"
    int roomNumber;     // e.g. 101
    RoomType roomType;
    RoomStatus status;
    double pricePerNight;

    Room(string roomId, int roomNumber, RoomType roomType, double pricePerNight) {
        this->roomId = roomId;
        this->roomNumber = roomNumber;
        this->roomType = roomType;
        this->pricePerNight = pricePerNight;
        this->status = RoomStatus::AVAILABLE;
    }

    bool isAvailable() {
        return status == RoomStatus::AVAILABLE;
    }
};

/* ============================================================
   Hotel
   ============================================================ */

class Hotel {
public:
    string hotelId;
    string name;
    string city;
    vector<Room*> rooms;

    Hotel(string hotelId, string name, string city) {
        this->hotelId = hotelId;
        this->name = name;
        this->city = city;
    }

    void addRoom(Room* room) {
        rooms.push_back(room);
    }

    vector<Room*> getAllRooms() {
        return rooms;
    }
};

/* ============================================================
   Booking
   ============================================================ */

class Booking {
public:
    string bookingId;
    User* user;
    Hotel* hotel;
    Room* room;

    int checkInDay;
    int checkOutDay;

    double totalAmount;
    BookingStatus status;

    Booking(string bookingId, User* user, Hotel* hotel, Room* room, int inDay, int outDay, double totalAmount) {
        this->bookingId = bookingId;
        this->user = user;
        this->hotel = hotel;
        this->room = room;
        this->checkInDay = inDay;
        this->checkOutDay = outDay;
        this->totalAmount = totalAmount;
        this->status = BookingStatus::CONFIRMED;
    }
};

/* ============================================================
   PaymentRequest
   ============================================================ */

class PaymentRequest {
public:
    string bookingId;
    double amount;
    string currency;

    PaymentRequest(string bookingId, double amount, string currency) {
        this->bookingId = bookingId;
        this->amount = amount;
        this->currency = currency;
    }
};

/* ============================================================
   PaymentStrategy (Strategy Pattern)
   ============================================================ */

class PaymentStrategy {
public:
    virtual PaymentStatus pay(PaymentRequest* request) = 0;
};

/* ---------------- Card Payment ---------------- */

class CardPayment : public PaymentStrategy {
public:
    PaymentStatus pay(PaymentRequest* request) override {
        cout << "[CardPayment] Paying " << request->amount << " " << request->currency
             << " for bookingId=" << request->bookingId << "\n";

        int r = rand() % 100;
        if (r < 85) return PaymentStatus::SUCCESS;
        return PaymentStatus::FAILED;
    }
};

/* ---------------- UPI Payment ---------------- */

class UpiPayment : public PaymentStrategy {
public:
    PaymentStatus pay(PaymentRequest* request) override {
        cout << "[UPIPayment] Paying " << request->amount << " " << request->currency
             << " for bookingId=" << request->bookingId << "\n";

        int r = rand() % 100;
        if (r < 90) return PaymentStatus::SUCCESS;
        return PaymentStatus::FAILED;
    }
};

/* ============================================================
   PaymentService (uses strategy)
   ============================================================ */

class PaymentService {
public:
    PaymentStrategy* paymentStrategy;

    PaymentService() {
        paymentStrategy = nullptr;
    }

    void setPaymentStrategy(PaymentStrategy* ps) {
        paymentStrategy = ps;
    }

    PaymentStatus makePayment(PaymentRequest* req) {
        if (paymentStrategy == nullptr) {
            cout << "[PaymentService] No payment strategy selected\n";
            return PaymentStatus::FAILED;
        }
        return paymentStrategy->pay(req);
    }
};

/* ============================================================
   BookingIdGenerator (simple helper)
   ============================================================ */

class BookingIdGenerator {
public:
    static int counter;

    static string generateBookingId() {
        counter++;
        return "BKG-" + to_string(counter);
    }
};

int BookingIdGenerator::counter = 1000;

/* ============================================================
   InventoryService
   - responsible for searching rooms
   - responsible for marking room status
   ============================================================ */

class InventoryService {
public:
    vector<Hotel*> hotels;

    void addHotel(Hotel* hotel) {
        hotels.push_back(hotel);
    }

    vector<Hotel*> searchHotelsByCity(string city) {
        vector<Hotel*> result;

        for (auto h : hotels) {
            if (h->city == city) {
                result.push_back(h);
            }
        }
        return result;
    }

    vector<Room*> searchAvailableRooms(Hotel* hotel, RoomType type) {
        vector<Room*> result;

        for (auto r : hotel->rooms) {
            if (r->roomType == type && r->status == RoomStatus::AVAILABLE) {
                result.push_back(r);
            }
        }
        return result;
    }

    void markRoomBooked(Room* room) {
        room->status = RoomStatus::BOOKED;
    }

    void markRoomAvailable(Room* room) {
        room->status = RoomStatus::AVAILABLE;
    }
};

/* ============================================================
   PricingService
   ============================================================ */

class PricingService {
public:
    double calculateTotal(Room* room, int checkInDay, int checkOutDay) {
        int nights = checkOutDay - checkInDay;
        if (nights <= 0) return 0;

        return nights * room->pricePerNight;
    }
};

/* ============================================================
   BookingService
   - creates booking
   - cancels booking
   ============================================================ */

class BookingService {
public:
    InventoryService* inventoryService;
    PricingService* pricingService;
    PaymentService* paymentService;

    unordered_map<string, Booking*> bookingStore; // bookingId -> booking

    BookingService(InventoryService* inv, PricingService* ps, PaymentService* pay) {
        inventoryService = inv;
        pricingService = ps;
        paymentService = pay;
    }

    Booking* createBooking(User* user, Hotel* hotel, Room* room, int checkInDay, int checkOutDay, string currency) {
        cout << "[BookingService] Creating booking for user=" << user->name
             << " hotel=" << hotel->name << " room=" << room->roomNumber << "\n";

        if (room->status != RoomStatus::AVAILABLE) {
            cout << "[BookingService] Room not available\n";
            return nullptr;
        }

        double total = pricingService->calculateTotal(room, checkInDay, checkOutDay);

        if (total <= 0) {
            cout << "[BookingService] Invalid stay duration\n";
            return nullptr;
        }

        string bookingId = BookingIdGenerator::generateBookingId();

        PaymentRequest* payReq = new PaymentRequest(bookingId, total, currency);
        PaymentStatus payStatus = paymentService->makePayment(payReq);

        if (payStatus == PaymentStatus::FAILED) {
            cout << "[BookingService] Payment failed. Booking not created.\n";
            return nullptr;
        }

        inventoryService->markRoomBooked(room);

        Booking* booking = new Booking(bookingId, user, hotel, room, checkInDay, checkOutDay, total);
        bookingStore[bookingId] = booking;

        cout << "[BookingService] Booking CONFIRMED bookingId=" << bookingId
             << " total=" << total << " " << currency << "\n";

        return booking;
    }

    bool cancelBooking(string bookingId) {
        cout << "[BookingService] Cancelling bookingId=" << bookingId << "\n";

        if (bookingStore.find(bookingId) == bookingStore.end()) {
            cout << "[BookingService] Booking not found\n";
            return false;
        }

        Booking* booking = bookingStore[bookingId];

        if (booking->status == BookingStatus::CANCELLED) {
            cout << "[BookingService] Already cancelled\n";
            return false;
        }

        booking->status = BookingStatus::CANCELLED;
        inventoryService->markRoomAvailable(booking->room);

        cout << "[BookingService] Cancelled successfully\n";
        return true;
    }

    Booking* getBookingById(string bookingId) {
        if (bookingStore.find(bookingId) == bookingStore.end()) return nullptr;
        return bookingStore[bookingId];
    }
};

/* ============================================================
   HotelBookingController
   - entry point for client requests
   ============================================================ */

class HotelBookingController {
public:
    InventoryService* inventoryService;
    BookingService* bookingService;

    HotelBookingController(InventoryService* inv, BookingService* bs) {
        inventoryService = inv;
        bookingService = bs;
    }

    void searchHotels(string city) {
        cout << "\n[Controller] Searching hotels in city=" << city << "\n";

        vector<Hotel*> hotels = inventoryService->searchHotelsByCity(city);

        if (hotels.empty()) {
            cout << "[Controller] No hotels found\n";
            return;
        }

        for (auto h : hotels) {
            cout << "Hotel: " << h->name << " (" << h->city << ")\n";
        }
    }

    void showAvailableRooms(Hotel* hotel, RoomType type) {
        cout << "\n[Controller] Available rooms in hotel=" << hotel->name
             << " type=" << roomTypeToString(type) << "\n";

        vector<Room*> rooms = inventoryService->searchAvailableRooms(hotel, type);

        if (rooms.empty()) {
            cout << "[Controller] No rooms available\n";
            return;
        }

        for (auto r : rooms) {
            cout << "RoomNumber: " << r->roomNumber
                 << " Type: " << roomTypeToString(r->roomType)
                 << " Status: " << roomStatusToString(r->status)
                 << " Price/Night: " << r->pricePerNight
                 << "\n";
        }
    }

    Booking* bookRoom(User* user, Hotel* hotel, Room* room, int inDay, int outDay, string currency) {
        return bookingService->createBooking(user, hotel, room, inDay, outDay, currency);
    }

    void cancelBooking(string bookingId) {
        bookingService->cancelBooking(bookingId);
    }

    void printBooking(string bookingId) {
        Booking* b = bookingService->getBookingById(bookingId);

        if (b == nullptr) {
            cout << "[Controller] Booking not found\n";
            return;
        }

        cout << "\n[Controller] Booking Details\n";
        cout << "BookingId: " << b->bookingId << "\n";
        cout << "User: " << b->user->name << "\n";
        cout << "Hotel: " << b->hotel->name << "\n";
        cout << "Room: " << b->room->roomNumber << " (" << roomTypeToString(b->room->roomType) << ")\n";
        cout << "CheckInDay: " << b->checkInDay << "\n";
        cout << "CheckOutDay: " << b->checkOutDay << "\n";
        cout << "TotalAmount: " << b->totalAmount << "\n";
        cout << "Status: " << bookingStatusToString(b->status) << "\n";
    }
};

/* ============================================================
   MAIN (Demo)
   ============================================================ */

int main() {
    srand(time(nullptr));

    // -------- Create Services --------
    InventoryService* inventoryService = new InventoryService();
    PricingService* pricingService = new PricingService();
    PaymentService* paymentService = new PaymentService();

    // choose payment strategy (UPI / Card)
    paymentService->setPaymentStrategy(new UpiPayment());

    BookingService* bookingService = new BookingService(inventoryService, pricingService, paymentService);

    HotelBookingController* controller = new HotelBookingController(inventoryService, bookingService);

    // -------- Create Data --------
    Hotel* h1 = new Hotel("H1", "Taj Bangalore", "Bangalore");
    h1->addRoom(new Room("R101", 101, RoomType::SINGLE, 2000));
    h1->addRoom(new Room("R102", 102, RoomType::DOUBLE, 3500));
    h1->addRoom(new Room("R103", 103, RoomType::DELUXE, 5000));

    Hotel* h2 = new Hotel("H2", "OYO Koramangala", "Bangalore");
    h2->addRoom(new Room("R201", 201, RoomType::SINGLE, 1200));
    h2->addRoom(new Room("R202", 202, RoomType::DOUBLE, 2000));

    Hotel* h3 = new Hotel("H3", "Novotel Mumbai", "Mumbai");
    h3->addRoom(new Room("R301", 301, RoomType::DELUXE, 6500));

    inventoryService->addHotel(h1);
    inventoryService->addHotel(h2);
    inventoryService->addHotel(h3);

    User* u1 = new User("U1", "Rahul", "9999999999");

    // -------- Flow Demo --------
    controller->searchHotels("Bangalore");

    controller->showAvailableRooms(h1, RoomType::SINGLE);

    // book room
    Booking* booking = controller->bookRoom(u1, h1, h1->rooms[0], 10, 12, "INR");

    if (booking != nullptr) {
        controller->printBooking(booking->bookingId);

        // try booking same room again
        cout << "\nTrying to book SAME room again...\n";
        controller->bookRoom(u1, h1, h1->rooms[0], 10, 12, "INR");

        // cancel booking
        controller->cancelBooking(booking->bookingId);

        // room available again
        controller->showAvailableRooms(h1, RoomType::SINGLE);
    }

    return 0;
}
