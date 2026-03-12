#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// SINGLETON DESIGN PATTERN (4 WAYS) - JAVA STYLE IN C++
// =========================================================
//
// ✅ 1) Eager Initialization Singleton
// ✅ 2) Lazy Initialization Singleton
// ✅ 3) Thread-Safe Singleton (Mutex)
// ✅ 4) Double-Checked Locking Singleton (Optimized)
//
// NOTE:
// - Java me GC hota hai, C++ me delete karna padta hai
// - Singleton me generally delete nahi karte (program end tak alive)
// =========================================================


// =========================================================
// 1) EAGER INITIALIZATION SINGLETON
// ---------------------------------------------------------
// ✅ Instance program start hote hi create ho jaata hai
// ✅ Thread-safe by default (usually)
// ❌ Agar object heavy hai but use nahi hua => waste
// =========================================================
class LoggerEager {
private:
    static LoggerEager* instance;

    LoggerEager() {
        cout << "[Eager] Constructor called\n";
    }

public:
    LoggerEager(const LoggerEager&) = delete;
    LoggerEager& operator=(const LoggerEager&) = delete;

    static LoggerEager* getInstance() {
        return instance;
    }

    void log(const string& msg) {
        cout << "[Eager LOG] " << msg << "\n";
    }
};

// ✅ Eagerly created here
LoggerEager* LoggerEager::instance = new LoggerEager();



// =========================================================
// 2) LAZY INITIALIZATION SINGLETON
// ---------------------------------------------------------
// ✅ First time getInstance() call hoga tab create hoga
// ✅ Efficient if rarely used
// ❌ Thread-safe nahi (2 threads => 2 objects ban sakte)
// =========================================================
class LoggerLazy {
private:
    static LoggerLazy* instance;

    LoggerLazy() {
        cout << "[Lazy] Constructor called\n";
    }

public:
    LoggerLazy(const LoggerLazy&) = delete;
    LoggerLazy& operator=(const LoggerLazy&) = delete;

    static LoggerLazy* getInstance() {
        if (instance == nullptr) {
            instance = new LoggerLazy();
        }
        return instance;
    }

    void log(const string& msg) {
        cout << "[Lazy LOG] " << msg << "\n";
    }
};

LoggerLazy* LoggerLazy::instance = nullptr;



// =========================================================
// 3) THREAD-SAFE SINGLETON (MUTEX LOCK)
// ---------------------------------------------------------
// ✅ 100% thread-safe
// ❌ Every call locks mutex => slower
// =========================================================
class LoggerThreadSafe {
private:
    static LoggerThreadSafe* instance;
    static mutex mtx;

    LoggerThreadSafe() {
        cout << "[ThreadSafe] Constructor called\n";
    }

public:
    LoggerThreadSafe(const LoggerThreadSafe&) = delete;
    LoggerThreadSafe& operator=(const LoggerThreadSafe&) = delete;

    static LoggerThreadSafe* getInstance() {
        lock_guard<mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new LoggerThreadSafe();
        }
        return instance;
    }

    void log(const string& msg) {
        cout << "[ThreadSafe LOG] " << msg << "\n";
    }
};

LoggerThreadSafe* LoggerThreadSafe::instance = nullptr;
mutex LoggerThreadSafe::mtx;



// =========================================================
// 4) DOUBLE-CHECKED LOCKING SINGLETON (Best for interviews)
// ---------------------------------------------------------
// ✅ Thread-safe
// ✅ Faster than full mutex lock on every call
// ✅ Lock only when instance needs to be created
// =========================================================
class LoggerDCL {
private:
    static LoggerDCL* instance;
    static mutex mtx;

    LoggerDCL() {
        cout << "[DCL] Constructor called\n";
    }

public:
    LoggerDCL(const LoggerDCL&) = delete;
    LoggerDCL& operator=(const LoggerDCL&) = delete;    // we should do both, copy constructor se copy bn jayega nhi to

    static LoggerDCL* getInstance() {
        // ✅ 1st check (no lock)
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);

            // ✅ 2nd check (with lock)
            if (instance == nullptr) {
                instance = new LoggerDCL();
            }
        }
        return instance;
    }
};

LoggerDCL* LoggerDCL::instance = nullptr;
mutex LoggerDCL::mtx;


class Sample {
private:
    static Sample* instance;
    static mutex mtx;
    Sample() {

    }
public:
    Sample(const Sample&) = delete;
    Sample& operator=(const Sample&) = delete;
    static Sample* getInstance() {
        if(instance==nullptr) {
            lock_guard<mutex> lock(mtx);
            if(instance==nullptr) {
                instance =  new Sample();
            }
        }
        return instance;
    }
};
Sample* Sample::instance = nullptr;
mutex Sample::mtx;



// =========================================================
// ✅ MAIN (Test all 4)
// =========================================================
int main() {

    cout << "\n=============================\n";
    cout << "1) EAGER SINGLETON\n";
    cout << "=============================\n";
    LoggerEager* e1 = LoggerEager::getInstance();
    LoggerEager* e2 = LoggerEager::getInstance();
    e1->log("Hello from eager");
    cout << "Same? " << (e1 == e2 ? "YES ✅" : "NO ❌") << "\n";


    cout << "\n=============================\n";
    cout << "2) LAZY SINGLETON\n";
    cout << "=============================\n";
    LoggerLazy* l1 = LoggerLazy::getInstance();
    LoggerLazy* l2 = LoggerLazy::getInstance();
    l1->log("Hello from lazy");
    cout << "Same? " << (l1 == l2 ? "YES ✅" : "NO ❌") << "\n";


    cout << "\n=============================\n";
    cout << "3) THREAD SAFE SINGLETON (MUTEX)\n";
    cout << "=============================\n";
    LoggerThreadSafe* t1 = LoggerThreadSafe::getInstance();
    LoggerThreadSafe* t2 = LoggerThreadSafe::getInstance();
    t1->log("Hello from thread-safe mutex singleton");
    cout << "Same? " << (t1 == t2 ? "YES ✅" : "NO ❌") << "\n";


    cout << "\n=============================\n";
    cout << "4) DOUBLE CHECKED LOCKING (DCL)\n";
    cout << "=============================\n";
    LoggerDCL* d1 = LoggerDCL::getInstance();
    LoggerDCL* d2 = LoggerDCL::getInstance();
    //d1->log("Hello from DCL singleton");
    cout << "Same? " << (d1 == d2 ? "YES ✅" : "NO ❌") << "\n";

    return 0;
}
