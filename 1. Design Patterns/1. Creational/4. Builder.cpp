#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// BUILDER DESIGN PATTERN (JAVA STYLE - C++)
// =========================================================
//
// ✅ Problem:
// If class has many fields -> constructor overloading mess
//
// ✅ Solution:
// Use Builder to set optional fields step-by-step
//
// Example: UserProfile
// =========================================================


// =========================================================
// Product Class
// =========================================================
class UserProfile {
private:
    // Required
    string name;
    int age;

    // Optional
    string email;
    string phone;
    string city;

public:
    // ✅ Constructor private-like simulation
    // We keep it public for simplicity (interview me ok),
    // but object should be created using Builder
    UserProfile(UserProfileBuilder* ub) {
        this->name = ub->name;
        this->age = ub->age;
        this->email = ub->email;
        this->phone = ub->phone;
        this->city = ub->city;
    }

    void show() {
        cout << "\n=== User Profile ===\n";
        cout << "Name: " << name << "\n";
        cout << "Age : " << age << "\n";
        cout << "Email: " << (email.empty() ? "N/A" : email) << "\n";
        cout << "Phone: " << (phone.empty() ? "N/A" : phone) << "\n";
        cout << "City : " << (city.empty() ? "N/A" : city) << "\n";
    }
};


// =========================================================
// Builder Class
// =========================================================
class UserProfileBuilder {
public:
    // Required fields
    string name;
    int age;

    // Optional fields (default empty)
    string email = "";
    string phone = "";
    string city = "";

public:
    // ✅ Constructor only required fields
    UserProfileBuilder(string name, int age) {
        this->name = name;
        this->age = age;
    }

    // ✅ Setter methods return "this" (method chaining)
    UserProfileBuilder* setEmail(string email) {
        this->email = email;
        return this;
    }

    UserProfileBuilder* setPhone(string phone) {
        this->phone = phone;
        return this;
    }

    UserProfileBuilder* setCity(string city) {
        this->city = city;
        return this;
    }

    // ✅ Final build method
    UserProfile* build() {
        return new UserProfile(this);
    }
};


// =========================================================
// ✅ MAIN
// =========================================================
int main() {

    // ✅ Java-style Builder usage
    UserProfileBuilder* upb = new UserProfileBuilder("Rahul", 23);
    UserProfile* user1 = upb->setEmail("rahul@gmail.com")
        ->setPhone("9999999999")
        ->setCity("Bangalore")
        ->build();
    user1->show();

    // ✅ Another user with fewer optional fields
    UserProfileBuilder* upb2 = new UserProfileBuilder("Aman", 21);
    UserProfile* user2 = upb2->setCity("Delhi")->build();

    user2->show();

    delete user1;
    delete user2;

    return 0;
}
