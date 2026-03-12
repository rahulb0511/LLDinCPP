#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <iomanip>
using namespace std;

/*
1. User can join/leave a group
2. User can add expense in a group
3. User can settle an expense in a group
4. Add expense should be based on several strategy (equal, percent, exact)
5. User can't leave group without settling expense
6. User can also add individual expense, outside group (one-one)
7. Notification can be send when expense is settled.
*/

// ============================================================
// STEP 1) ENUMS
// ============================================================

enum class SplitType {
    EQUAL,
    EXACT,
    PERCENTAGE
};

// ============================================================
// STEP 2) BASIC MODELS
// Split (userId, amount)
// ============================================================

class Split {
public:
    string userId;
    double amount;

    Split(string userId, double amount) {
        this->userId = userId;
        this->amount = amount;
    }
};

// ============================================================
// STEP 3) OBSERVER PATTERN
// Observer -> interface
// User -> observer (will receive notifications)
// ============================================================

class Observer {
public:
    virtual void update(string message) = 0;
};

// ============================================================
// STEP 4) SPLIT STRATEGY (Strategy Pattern)
// SplitStrategy -> interface
// EqualSplit, ExactSplit, PercentageSplit -> strategies
// ============================================================

class SplitStrategy {
public:
    virtual vector<Split> calculateSplit(double totalAmount,
                                         vector<string> userIds,
                                         vector<double> values = {}) = 0;
};

// -------------------- Equal Split --------------------
class EqualSplit : public SplitStrategy {
public:
    vector<Split> calculateSplit(double totalAmount,
                                 vector<string> userIds,
                                 vector<double> values = {}) override {
        vector<Split> splits;
        double perUser = totalAmount / userIds.size();

        for (auto userId : userIds) {
            splits.push_back(Split(userId, perUser));
        }
        return splits;
    }
};

// -------------------- Exact Split --------------------
class ExactSplit : public SplitStrategy {
public:
    vector<Split> calculateSplit(double totalAmount,
                                 vector<string> userIds,
                                 vector<double> values = {}) override {
        vector<Split> splits;

        // Basic validation (optional)
        // values.size() should be same as userIds.size()

        for (int i = 0; i < (int)userIds.size(); i++) {
            splits.push_back(Split(userIds[i], values[i]));
        }
        return splits;
    }
};

// -------------------- Percentage Split --------------------
class PercentageSplit : public SplitStrategy {
public:
    vector<Split> calculateSplit(double totalAmount,
                                 vector<string> userIds,
                                 vector<double> values = {}) override {
        vector<Split> splits;

        // values[i] = percentage for userIds[i]

        for (int i = 0; i < (int)userIds.size(); i++) {
            double amt = (totalAmount * values[i]) / 100.0;
            splits.push_back(Split(userIds[i], amt));
        }
        return splits;
    }
};

// ============================================================
// STEP 5) SPLIT FACTORY (Factory Pattern)
// ============================================================

class SplitFactory {
public:
    static SplitStrategy* getSplitStrategy(SplitType type) {
        if (type == SplitType::EQUAL) return new EqualSplit();
        if (type == SplitType::EXACT) return new ExactSplit();
        if (type == SplitType::PERCENTAGE) return new PercentageSplit();
        return new EqualSplit();
    }
};

// ============================================================
// STEP 6) USER (Concrete Observer)
// balances: otherUserId -> amount
// +ve  => other owes you
// -ve  => you owe other
// ============================================================

class User : public Observer {
public:
    static int nextUserId;

    string userId;
    string name;
    string email;

    map<string, double> balances;

    User(string name, string email) {
        this->userId = "user" + to_string(++nextUserId);
        this->name = name;
        this->email = email;
    }

    void update(string message) override {
        cout << "[NOTIFICATION to " << name << "]: " << message << "\n";
    }

    void updateBalance(string otherUserId, double amount) {
        balances[otherUserId] += amount;

        // remove small zeros
        if (abs(balances[otherUserId]) < 0.01) {
            balances.erase(otherUserId);
        }
    }

    double getTotalOwed() {
        double total = 0;
        for (auto &it : balances) {
            if (it.second < 0) total += abs(it.second);
        }
        return total;
    }

    double getTotalOwing() {
        double total = 0;
        for (auto &it : balances) {
            if (it.second > 0) total += it.second;
        }
        return total;
    }
};
int User::nextUserId = 0;

// ============================================================
// STEP 7) EXPENSE MODEL
// ============================================================

class Expense {
public:
    static int nextExpenseId;

    string expenseId;
    string description;
    double totalAmount;
    string paidByUserId;
    vector<Split> splits;
    string groupId; // empty means individual expense

    Expense(string desc, double amount, string paidBy, vector<Split> splits, string groupId = "") {
        this->expenseId = "expense" + to_string(++nextExpenseId);
        this->description = desc;
        this->totalAmount = amount;
        this->paidByUserId = paidBy;
        this->splits = splits;
        this->groupId = groupId;
    }
};
int Expense::nextExpenseId = 0;

// ============================================================
// STEP 8) DEBT SIMPLIFIER
// groupBalances[A][B] = +200 => B owes A 200
// ============================================================

class DebtSimplifier {
public:
    static map<string, map<string, double>> simplifyDebts(map<string, map<string, double>> groupBalances) {

        // netAmounts[user] = +ve => user should receive
        // netAmounts[user] = -ve => user should pay
        map<string, double> netAmounts;

        for (auto &it : groupBalances) {
            netAmounts[it.first] = 0;
        }

        // process only positive amounts to avoid double counting
        for (auto &creditorEntry : groupBalances) {
            string creditorId = creditorEntry.first;

            for (auto &debtorEntry : creditorEntry.second) {
                string debtorId = debtorEntry.first;
                double amount = debtorEntry.second;

                if (amount > 0.01) {
                    netAmounts[creditorId] += amount;
                    netAmounts[debtorId] -= amount;
                }
            }
        }

        vector<pair<string, double>> creditors;
        vector<pair<string, double>> debtors;

        for (auto &it : netAmounts) {
            if (it.second > 0.01) creditors.push_back({it.first, it.second});
            else if (it.second < -0.01) debtors.push_back({it.first, -it.second});
        }

        sort(creditors.begin(), creditors.end(),
             [](auto &a, auto &b) { return a.second > b.second; });

        sort(debtors.begin(), debtors.end(),
             [](auto &a, auto &b) { return a.second > b.second; });

        map<string, map<string, double>> simplified;
        for (auto &it : groupBalances) simplified[it.first] = map<string, double>();

        int i = 0, j = 0;
        while (i < (int)creditors.size() && j < (int)debtors.size()) {
            string creditorId = creditors[i].first;
            string debtorId = debtors[j].first;

            double settle = min(creditors[i].second, debtors[j].second);

            simplified[creditorId][debtorId] = settle;
            simplified[debtorId][creditorId] = -settle;

            creditors[i].second -= settle;
            debtors[j].second -= settle;

            if (creditors[i].second < 0.01) i++;
            if (debtors[j].second < 0.01) j++;
        }

        return simplified;
    }
};

// ============================================================
// STEP 9) GROUP
// Group = Observable
// members = observers
// groupBalances[userA][userB] => balance
// addExpense(), settlePayment(), removeMember(), showBalances()
// ============================================================

class Group {
private:
    User* getUserById(string userId) {
        for (auto member : members) {
            if (member->userId == userId) return member;
        }
        return nullptr;
    }

public:
    static int nextGroupId;

    string groupId;
    string name;

    vector<User*> members; // observers
    map<string, Expense*> groupExpenses;

    // groupBalances[A][B] = +X => B owes A X
    map<string, map<string, double>> groupBalances;

    Group(string name) {
        this->groupId = "group" + to_string(++nextGroupId);
        this->name = name;
    }

    void addMember(User* user) {
        members.push_back(user);
        groupBalances[user->userId] = map<string, double>();

        cout << user->name << " added to group " << name << "\n";
    }

    bool isMember(string userId) {
        return groupBalances.find(userId) != groupBalances.end();
    }

    void notifyMembers(string message) {
        for (auto member : members) {
            member->update(message);
        }
    }

    // groupBalances[from][to] += amount
    // groupBalances[to][from] -= amount
    void updateGroupBalance(string fromUserId, string toUserId, double amount) {
        groupBalances[fromUserId][toUserId] += amount;
        groupBalances[toUserId][fromUserId] -= amount;

        if (abs(groupBalances[fromUserId][toUserId]) < 0.01) {
            groupBalances[fromUserId].erase(toUserId);
        }
        if (abs(groupBalances[toUserId][fromUserId]) < 0.01) {
            groupBalances[toUserId].erase(fromUserId);
        }
    }

    bool canUserLeaveGroup(string userId) {
        if (!isMember(userId)) return false;

        for (auto &it : groupBalances[userId]) {
            if (abs(it.second) > 0.01) return false;
        }
        return true;
    }

    bool removeMember(string userId) {
        if (!isMember(userId)) {
            cout << "User not in group ❌\n";
            return false;
        }

        if (!canUserLeaveGroup(userId)) {
            cout << "\nUser not allowed to leave group without clearing expenses ❌\n";
            return false;
        }

        // remove from members vector
        for (int i = 0; i < (int)members.size(); i++) {
            if (members[i]->userId == userId) {
                members.erase(members.begin() + i);
                break;
            }
        }

        // remove from balances map
        groupBalances.erase(userId);

        // remove userId from others maps
        for (auto &it : groupBalances) {
            it.second.erase(userId);
        }

        return true;
    }

    // Add expense in group
    bool addExpense(string description,
                    double amount,
                    string paidByUserId,
                    vector<string> involvedUsers,
                    SplitType splitType,
                    vector<double> splitValues = {}) {

        if (!isMember(paidByUserId)) {
            cout << "PaidBy user not in group ❌\n";
            return false;
        }

        for (auto uid : involvedUsers) {
            if (!isMember(uid)) {
                cout << "Some involved user not in group ❌\n";
                return false;
            }
        }

        SplitStrategy* strategy = SplitFactory::getSplitStrategy(splitType);
        vector<Split> splits = strategy->calculateSplit(amount, involvedUsers, splitValues);

        Expense* expense = new Expense(description, amount, paidByUserId, splits, groupId);
        groupExpenses[expense->expenseId] = expense;

        // update balances
        for (auto &sp : splits) {
            if (sp.userId != paidByUserId) {
                updateGroupBalance(paidByUserId, sp.userId, sp.amount);
            }
        }

        cout << "\n=========== Sending Notifications ===========\n";
        notifyMembers("New expense added: " + description + " (Rs " + to_string((int)amount) + ")");

        // pretty print
        User* payer = getUserById(paidByUserId);
        cout << "\n=========== Expense Added ===========\n";
        cout << "Group: " << name << "\n";
        cout << "Expense: " << description << " Amount: Rs " << amount << "\n";
        cout << "Paid By: " << (payer ? payer->name : paidByUserId) << "\n";
        cout << "Involved: ";
        for (auto uid : involvedUsers) {
            User* u = getUserById(uid);
            cout << (u ? u->name : uid) << ", ";
        }
        cout << "\n";

        return true;
    }

    bool settlePayment(string fromUserId, string toUserId, double amount) {
        if (!isMember(fromUserId) || !isMember(toUserId)) {
            cout << "user is not a part of this group ❌\n";
            return false;
        }

        updateGroupBalance(fromUserId, toUserId, amount);

        User* fromUser = getUserById(fromUserId);
        User* toUser = getUserById(toUserId);

        string fromName = fromUser ? fromUser->name : fromUserId;
        string toName = toUser ? toUser->name : toUserId;

        notifyMembers("Settlement: " + fromName + " paid " + toName + " Rs " + to_string((int)amount));

        cout << "Settlement in " << name << ": " << fromName
             << " settled Rs " << amount << " with " << toName << "\n";

        return true;
    }

    void showGroupBalances() {
        cout << "\n=== Group Balances for " << name << " ===\n";

        for (auto &entry : groupBalances) {
            string memberId = entry.first;
            User* member = getUserById(memberId);

            cout << (member ? member->name : memberId) << "'s balances:\n";

            if (entry.second.empty()) {
                cout << "  No outstanding balances\n";
                continue;
            }

            for (auto &b : entry.second) {
                string otherId = b.first;
                User* other = getUserById(otherId);

                string otherName = other ? other->name : otherId;
                double balance = b.second;

                if (balance > 0) {
                    cout << "  " << otherName << " owes: Rs " << fixed << setprecision(2) << balance << "\n";
                } else {
                    cout << "  Owes " << otherName << ": Rs " << fixed << setprecision(2) << abs(balance) << "\n";
                }
            }
        }
    }

    void simplifyGroupDebts() {
        groupBalances = DebtSimplifier::simplifyDebts(groupBalances);
        cout << "\nDebts have been simplified for group: " << name << " ✅\n";
    }
};
int Group::nextGroupId = 0;

// ============================================================
// STEP 10) SPLITWISE MAIN SYSTEM (Singleton + Facade)
// Manages users, groups, expenses
// Delegates logic to Group
// ============================================================

class Splitwise {
private:
    map<string, User*> users;
    map<string, Group*> groups;
    map<string, Expense*> expenses;

    static Splitwise* instance;

    Splitwise() {}

public:
    static Splitwise* getInstance() {
        if (instance == nullptr) {
            instance = new Splitwise();
        }
        return instance;
    }

    // -------------------- USER API --------------------
    User* createUser(string name, string email) {
        User* user = new User(name, email);
        users[user->userId] = user;
        cout << "User created: " << name << " (ID: " << user->userId << ")\n";
        return user;
    }

    User* getUser(string userId) {
        if (users.find(userId) == users.end()) return nullptr;
        return users[userId];
    }

    // -------------------- GROUP API --------------------
    Group* createGroup(string name) {
        Group* group = new Group(name);
        groups[group->groupId] = group;
        cout << "Group created: " << name << " (ID: " << group->groupId << ")\n";
        return group;
    }

    Group* getGroup(string groupId) {
        if (groups.find(groupId) == groups.end()) return nullptr;
        return groups[groupId];
    }

    void addUserToGroup(string userId, string groupId) {
        User* user = getUser(userId);
        Group* group = getGroup(groupId);

        if (!user || !group) {
            cout << "User/Group not found ❌\n";
            return;
        }

        group->addMember(user);
    }

    bool removeUserFromGroup(string userId, string groupId) {
        Group* group = getGroup(groupId);
        User* user = getUser(userId);

        if (!group || !user) {
            cout << "Group/User not found ❌\n";
            return false;
        }

        bool ok = group->removeMember(userId);

        if (ok) {
            cout << user->name << " successfully left " << group->name << " ✅\n";
        }
        return ok;
    }

    // -------------------- GROUP EXPENSE API --------------------
    void addExpenseToGroup(string groupId,
                           string description,
                           double amount,
                           string paidByUserId,
                           vector<string> involvedUsers,
                           SplitType splitType,
                           vector<double> splitValues = {}) {
        Group* group = getGroup(groupId);
        if (!group) {
            cout << "Group not found ❌\n";
            return;
        }

        group->addExpense(description, amount, paidByUserId, involvedUsers, splitType, splitValues);
    }

    void settlePaymentInGroup(string groupId, string fromUserId, string toUserId, double amount) {
        Group* group = getGroup(groupId);
        if (!group) {
            cout << "Group not found ❌\n";
            return;
        }

        group->settlePayment(fromUserId, toUserId, amount);
    }

    void showGroupBalances(string groupId) {
        Group* group = getGroup(groupId);
        if (!group) return;
        group->showGroupBalances();
    }

    void simplifyGroupDebts(string groupId) {
        Group* group = getGroup(groupId);
        if (!group) return;
        group->simplifyGroupDebts();
    }

    // -------------------- INDIVIDUAL EXPENSE API --------------------
    void addIndividualExpense(string description,
                              double amount,
                              string paidByUserId,
                              string toUserId,
                              SplitType splitType,
                              vector<double> splitValues = {}) {

        SplitStrategy* strategy = SplitFactory::getSplitStrategy(splitType);

        vector<string> usersList = {paidByUserId, toUserId};
        vector<Split> splits = strategy->calculateSplit(amount, usersList, splitValues);

        Expense* expense = new Expense(description, amount, paidByUserId, splits);
        expenses[expense->expenseId] = expense;

        User* paidByUser = getUser(paidByUserId);
        User* toUser = getUser(toUserId);

        if (!paidByUser || !toUser) {
            cout << "Invalid users for individual expense ❌\n";
            return;
        }

        paidByUser->updateBalance(toUserId, amount);
        toUser->updateBalance(paidByUserId, -amount);

        cout << "Individual expense added: " << description
             << " (Rs " << amount << ") paid by "
             << paidByUser->name << " for " << toUser->name << "\n";
    }

    void showUserBalance(string userId) {
        User* user = getUser(userId);
        if (!user) return;

        cout << "\n=========== Balance for " << user->name << " ===========\n";
        cout << "Total you owe: Rs " << fixed << setprecision(2) << user->getTotalOwed() << "\n";
        cout << "Total others owe you: Rs " << fixed << setprecision(2) << user->getTotalOwing() << "\n";

        cout << "Detailed balances:\n";
        for (auto &it : user->balances) {
            User* other = getUser(it.first);
            if (!other) continue;

            if (it.second > 0) {
                cout << "  " << other->name << " owes you: Rs " << it.second << "\n";
            } else {
                cout << "  You owe " << other->name << ": Rs " << abs(it.second) << "\n";
            }
        }
    }
};

Splitwise* Splitwise::instance = nullptr;

// ============================================================
// STEP 11) MAIN FLOW (Same as your sample)
// ============================================================

int main() {
    Splitwise* manager = Splitwise::getInstance();

    cout << "\n=========== Creating Users ====================\n";
    User* user1 = manager->createUser("Aditya", "aditya@gmail.com");
    User* user2 = manager->createUser("Rohit", "rohit@gmail.com");
    User* user3 = manager->createUser("Manish", "manish@gmail.com");
    User* user4 = manager->createUser("Saurav", "saurav@gmail.com");

    cout << "\n=========== Creating Group and Adding Members ====================\n";
    Group* hostelGroup = manager->createGroup("Hostel Expenses");

    manager->addUserToGroup(user1->userId, hostelGroup->groupId);
    manager->addUserToGroup(user2->userId, hostelGroup->groupId);
    manager->addUserToGroup(user3->userId, hostelGroup->groupId);
    manager->addUserToGroup(user4->userId, hostelGroup->groupId);

    cout << "\n=========== Adding Expenses in group ====================\n";
    vector<string> groupMembers = {user1->userId, user2->userId, user3->userId, user4->userId};
    manager->addExpenseToGroup(hostelGroup->groupId, "Lunch", 800.0,
                               user1->userId, groupMembers, SplitType::EQUAL);

    vector<string> dinnerMembers = {user1->userId, user3->userId, user4->userId};
    vector<double> dinnerAmounts = {200.0, 300.0, 200.0};

    manager->addExpenseToGroup(hostelGroup->groupId, "Dinner", 700.0,
                               user3->userId, dinnerMembers, SplitType::EXACT, dinnerAmounts);

    cout << "\n=========== printing Group-Specific Balances ====================\n";
    manager->showGroupBalances(hostelGroup->groupId);

    cout << "\n=========== Debt Simplification ====================\n";
    manager->simplifyGroupDebts(hostelGroup->groupId);

    cout << "\n=========== printing Group-Specific Balances ====================\n";
    manager->showGroupBalances(hostelGroup->groupId);

    cout << "\n=========== Adding Individual Expense ====================\n";
    manager->addIndividualExpense("Coffee", 40.0, user2->userId, user4->userId, SplitType::EQUAL);

    cout << "\n=========== printing User Balances ====================\n";
    manager->showUserBalance(user1->userId);
    manager->showUserBalance(user2->userId);
    manager->showUserBalance(user3->userId);
    manager->showUserBalance(user4->userId);

    cout << "\n==========Attempting to remove Rohit from group==========\n";
    manager->removeUserFromGroup(user2->userId, hostelGroup->groupId);

    cout << "\n======== Making Settlement to Clear Rohit's Debt ========\n";
    manager->settlePaymentInGroup(hostelGroup->groupId, user2->userId, user3->userId, 200.0);

    cout << "\n======== Attempting to Remove Rohit Again ========\n";
    manager->removeUserFromGroup(user2->userId, hostelGroup->groupId);

    cout << "\n=========== Updated Group Balances ====================\n";
    manager->showGroupBalances(hostelGroup->groupId);

    return 0;
}
