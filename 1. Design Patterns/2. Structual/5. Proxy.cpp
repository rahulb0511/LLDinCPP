#include <iostream>
using namespace std;

// ✅ Interface (like DAO interface in Java)
class EmployeeDAO {
public:
    virtual void createEmployee(string clientRole, string empName) = 0;
    virtual void getEmployee(string clientRole, int empId) = 0;
    virtual void deleteEmployee(string clientRole, int empId) = 0;
};

// ✅ Real Object (Actual DB logic)
class EmployeeDAOImpl : public EmployeeDAO {
public:
    void createEmployee(string clientRole, string empName) override {
        cout << "[DB] Employee created: " << empName << "\n";
    }

    void getEmployee(string clientRole, int empId) override {
        cout << "[DB] Fetching employee details for ID: " << empId << "\n";
    }

    void deleteEmployee(string clientRole, int empId) override {
        cout << "[DB] Employee deleted with ID: " << empId << "\n";
    }
};

// ✅ Proxy (Gatekeeper)
class EmployeeDAOProxy : public EmployeeDAO {
private:
    EmployeeDAO* realDAO;

    bool isAdmin(string role) {
        return role == "ADMIN";
    }

public:
    EmployeeDAOProxy() {
        realDAO = new EmployeeDAOImpl();
    }

    void createEmployee(string clientRole, string empName) override {
        if (!isAdmin(clientRole)) {
            cout << "[Proxy] Access Denied ❌ Only ADMIN can create employee\n";
            return;
        }
        cout << "[Proxy] Access Granted ✅\n";
        realDAO->createEmployee(clientRole, empName);
    }

    void getEmployee(string clientRole, int empId) override {
        // ✅ both ADMIN and USER can read
        cout << "[Proxy] Read Access Granted ✅\n";
        realDAO->getEmployee(clientRole, empId);
    }

    void deleteEmployee(string clientRole, int empId) override {
        if (!isAdmin(clientRole)) {
            cout << "[Proxy] Access Denied ❌ Only ADMIN can delete employee\n";
            return;
        }
        cout << "[Proxy] Access Granted ✅\n";
        realDAO->deleteEmployee(clientRole, empId);
    }
};

int main() {
    EmployeeDAO* dao = new EmployeeDAOProxy();

    cout << "\n--- USER trying operations ---\n";
    dao->getEmployee("USER", 101);
    dao->createEmployee("USER", "Rahul");
    dao->deleteEmployee("USER", 101);

    cout << "\n--- ADMIN trying operations ---\n";
    dao->getEmployee("ADMIN", 201);
    dao->createEmployee("ADMIN", "Aman");
    dao->deleteEmployee("ADMIN", 201);

    return 0;
}
