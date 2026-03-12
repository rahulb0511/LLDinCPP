#include <bits/stdc++.h>
#include <iostream>
using namespace std;


/*

Happy Flow (Only Flow)

Create warehouse

Create product

Add product stock to warehouse (there can be multiple warehouse)

System updates inventory

System checks stock vs threshold

Stock is sufficient

No alert triggered

Inventory marked healthy

Operations continue normally

*/

// =========================================================
// 1) ENUMS
// =========================================================

enum class ProductCategory {
    ELECTRONICS,
    CLOTHING,
    GROCERY,
    FURNITURE,
    OTHER
};

enum class InventoryOperation {
    ADD,
    REMOVE,
    TRANSFER,
    ADJUST
};

// =========================================================
// 2) PRODUCT (Base Class)
// =========================================================

class Product {
protected:
    string sku;
    string name;
    double price;
    int quantity;
    int threshold;
    ProductCategory category;

public:
    Product() {
        price = 0;
        quantity = 0;
        threshold = 0;
        category = ProductCategory::OTHER;
    }

    string getSku() { return sku; }
    string getName() { return name; }
    double getPrice() { return price; }
    int getQuantity() { return quantity; }
    int getThreshold() { return threshold; }
    ProductCategory getCategory() { return category; }

    void setSku(string sku) { this->sku = sku; }
    void setName(string name) { this->name = name; }
    void setPrice(double price) { this->price = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setThreshold(int threshold) { this->threshold = threshold; }
    void setCategory(ProductCategory category) { this->category = category; }

    void addStock(int qty) {
        quantity += qty;
    }

    bool removeStock(int qty) {
        if (quantity < qty) return false;
        quantity -= qty;
        return true;
    }
};

// =========================================================
// 3) CONCRETE PRODUCTS
// =========================================================

class ElectronicsProduct : public Product {
public:
    string brand;

    ElectronicsProduct(string sku, string name, double price, int quantity, int threshold) {
        setSku(sku);
        setName(name);
        setPrice(price);
        setQuantity(quantity);
        setThreshold(threshold);
        setCategory(ProductCategory::ELECTRONICS);
        brand = "GENERIC";
    }
};

class ClothingProduct : public Product {
public:
    string size;
    string color;

    ClothingProduct(string sku, string name, double price, int quantity, int threshold) {
        setSku(sku);
        setName(name);
        setPrice(price);
        setQuantity(quantity);
        setThreshold(threshold);
        setCategory(ProductCategory::CLOTHING);
        size = "M";
        color = "BLACK";
    }
};

class GroceryProduct : public Product {
public:
    bool refrigerated;

    GroceryProduct(string sku, string name, double price, int quantity, int threshold) {
        setSku(sku);
        setName(name);
        setPrice(price);
        setQuantity(quantity);
        setThreshold(threshold);
        setCategory(ProductCategory::GROCERY);
        refrigerated = false;
    }
};

// =========================================================
// 4) PRODUCT FACTORY (Factory Pattern)
// =========================================================

class ProductFactory {
public:
    Product* createProduct(ProductCategory category,
                           string sku, string name,
                           double price, int quantity, int threshold) {
        if (category == ProductCategory::ELECTRONICS) {
            return new ElectronicsProduct(sku, name, price, quantity, threshold);
        }
        if (category == ProductCategory::CLOTHING) {
            return new ClothingProduct(sku, name, price, quantity, threshold);
        }
        if (category == ProductCategory::GROCERY) {
            return new GroceryProduct(sku, name, price, quantity, threshold);
        }

        // Default product
        Product* p = new Product();
        p->setSku(sku);
        p->setName(name);
        p->setPrice(price);
        p->setQuantity(quantity);
        p->setThreshold(threshold);
        p->setCategory(category);
        return p;
    }
};

// =========================================================
// 5) OBSERVER PATTERN (Low stock alerts)
// =========================================================

class InventoryObserver {
public:
    virtual void update(Product* product, string message) = 0;
};

class SupplierNotifier : public InventoryObserver {
private:
    string supplierName;
    string email;

public:
    SupplierNotifier(string supplierName, string email) {
        this->supplierName = supplierName;
        this->email = email;
    }

    void update(Product* product, string message) override {
        cout << "[SupplierNotifier] Supplier=" << supplierName
             << " Email=" << email
             << " -> " << message
             << " (SKU=" << product->getSku() << ", Name=" << product->getName() << ")\n";
    }
};

class DashboardAlertSystem : public InventoryObserver {
private:
    string alertLevel;

public:
    DashboardAlertSystem(string alertLevel) {
        this->alertLevel = alertLevel;
    }

    void update(Product* product, string message) override {
        cout << "[DashboardAlert][" << alertLevel << "] "
             << message
             << " SKU=" << product->getSku()
             << " Qty=" << product->getQuantity()
             << " Threshold=" << product->getThreshold()
             << "\n";
    }
};

// =========================================================
// 6) REPLENISHMENT STRATEGY (Strategy Pattern)
// =========================================================

class ReplenishmentStrategy {
public:
    virtual void replenish(Product* product) = 0;
};

class JustInTimeStrategy : public ReplenishmentStrategy {
public:
    void replenish(Product* product) override {
        cout << "[Replenishment] Just-In-Time applied ✅ for "
             << product->getName()
             << " (SKU=" << product->getSku() << ")\n";
    }
};

class BulkOrderStrategy : public ReplenishmentStrategy {
public:
    void replenish(Product* product) override {
        cout << "[Replenishment] Bulk-Order applied ✅ for "
             << product->getName()
             << " (SKU=" << product->getSku() << ")\n";
    }
};

// =========================================================
// 7) WAREHOUSE
// id, name, location, products(SKU -> Product*)
// addProduct/removeProduct/getAvailableQuantity
// =========================================================

class Warehouse {
private:
    int id;
    string name;
    string location;
    unordered_map<string, Product*> products;

public:
    Warehouse(int id, string name, string location) {
        this->id = id;
        this->name = name;
        this->location = location;
    }

    int getId() { return id; }
    string getName() { return name; }
    string getLocation() { return location; }

    Product* getProductBySku(string sku) {
        if (products.find(sku) == products.end()) return nullptr;
        return products[sku];
    }

    vector<Product*> getAllProducts() {
        vector<Product*> ans;
        for (auto &it : products) ans.push_back(it.second);
        return ans;
    }

    void addProduct(Product* product, int qty) {
        string sku = product->getSku();

        if (products.find(sku) != products.end()) {
            products[sku]->addStock(qty);
        } else {
            product->addStock(qty);
            products[sku] = product;
        }

        cout << "[Warehouse] Added " << qty << " units of " << product->getName()
             << " (SKU=" << sku << ") in " << name
             << " NewQty=" << products[sku]->getQuantity() << "\n";
    }

    bool removeProduct(string sku, int qty) {
        if (products.find(sku) == products.end()) {
            cout << "[Warehouse] Product not found ❌ SKU=" << sku << " in " << name << "\n";
            return false;
        }

        Product* p = products[sku];

        if (!p->removeStock(qty)) {
            cout << "[Warehouse] Insufficient stock ❌ SKU=" << sku
                 << " Available=" << p->getQuantity()
                 << " Requested=" << qty << "\n";
            return false;
        }

        cout << "[Warehouse] Removed " << qty << " units of " << p->getName()
             << " (SKU=" << sku << ") from " << name
             << " RemainingQty=" << p->getQuantity() << "\n";

        if (p->getQuantity() == 0) {
            products.erase(sku);
            cout << "[Warehouse] SKU=" << sku << " removed fully (qty=0) ✅\n";
        }

        return true;
    }

    int getAvailableQuantity(string sku) {
        Product* p = getProductBySku(sku);
        if (p == nullptr) return 0;
        return p->getQuantity();
    }

    void printInventory() {
        cout << "\n--- Inventory of Warehouse: " << name << " (" << location << ") ---\n";
        if (products.size() == 0) cout << "No products.\n";

        for (auto &it : products) {
            Product* p = it.second;
            cout << "SKU=" << p->getSku()
                 << " Name=" << p->getName()
                 << " Price=" << p->getPrice()
                 << " Qty=" << p->getQuantity()
                 << " Threshold=" << p->getThreshold()
                 << "\n";
        }

        cout << "-------------------------------------------------\n";
    }
};

// =========================================================
// 8) INVENTORY MANAGER (Singleton)
// warehouses, productFactory, observers, replenishmentStrategy
// addWarehouse/addObserver/notify/checkAndReplenish/performCheck/transferStock
// =========================================================

class InventoryManager {
private:
    static InventoryManager* instance;

    vector<Warehouse*> warehouses;
    ProductFactory* productFactory;
    vector<InventoryObserver*> observers;
    ReplenishmentStrategy* replenishmentStrategy;

    InventoryManager() {
        productFactory = new ProductFactory();
        replenishmentStrategy = nullptr;
    }

public:
    static InventoryManager* getInstance() {
        if (instance == nullptr) {
            instance = new InventoryManager();
        }
        return instance;
    }

    ProductFactory* getProductFactory() {
        return productFactory;
    }

    void setReplenishmentStrategy(ReplenishmentStrategy* strategy) {
        replenishmentStrategy = strategy;
    }

    void addWarehouse(Warehouse* wh) {
        warehouses.push_back(wh);
    }

    Warehouse* getWarehouseById(int id) {
        for (auto w : warehouses) {
            if (w->getId() == id) return w;
        }
        return nullptr;
    }

    void addObserver(InventoryObserver* obs) {
        observers.push_back(obs);
    }

    void notifyObservers(Product* product, string msg) {
        for (auto obs : observers) {
            obs->update(product, msg);
        }
    }

    Product* getProductBySku(string sku) {
        for (auto w : warehouses) {
            Product* p = w->getProductBySku(sku);
            if (p != nullptr) return p;
        }
        return nullptr;
    }

    void checkAndReplenish(string sku) {
        Product* p = getProductBySku(sku);

        if (p == nullptr) {
            cout << "[InventoryManager] SKU not found ❌ " << sku << "\n";
            return;
        }

        if (p->getQuantity() < p->getThreshold()) {
            string msg = "LOW STOCK ALERT 🚨 Name=" + p->getName() +
                         " Qty=" + to_string(p->getQuantity()) +
                         " Threshold=" + to_string(p->getThreshold());

            notifyObservers(p, msg);

            if (replenishmentStrategy != nullptr) {
                replenishmentStrategy->replenish(p);
            }
        } else {
            cout << "[InventoryManager] Stock OK ✅ SKU=" << sku
                 << " Qty=" << p->getQuantity()
                 << " Threshold=" << p->getThreshold() << "\n";
        }
    }

    void performInventoryCheck() {
        cout << "\n[InventoryManager] Performing FULL inventory check...\n";
        for (auto w : warehouses) {
            vector<Product*> list = w->getAllProducts();
            for (auto p : list) {
                if (p->getQuantity() < p->getThreshold()) {
                    checkAndReplenish(p->getSku());
                }
            }
        }
    }

    bool transferStock(int fromWarehouseId, int toWarehouseId, string sku, int qty) {
        Warehouse* fromWh = getWarehouseById(fromWarehouseId);
        Warehouse* toWh = getWarehouseById(toWarehouseId);

        if (fromWh == nullptr || toWh == nullptr) {
            cout << "[InventoryManager] Warehouse not found ❌\n";
            return false;
        }

        Product* p = fromWh->getProductBySku(sku);
        if (p == nullptr) {
            cout << "[InventoryManager] SKU not found in FROM warehouse ❌\n";
            return false;
        }

        bool removed = fromWh->removeProduct(sku, qty);
        if (!removed) return false;

        // Simple transfer using same product pointer
        toWh->addProduct(p, qty);

        cout << "[InventoryManager] TRANSFER ✅ SKU=" << sku
             << " Qty=" << qty
             << " From=" << fromWh->getName()
             << " To=" << toWh->getName()
             << "\n";

        return true;
    }
};

InventoryManager* InventoryManager::instance = nullptr;

// =========================================================
// 9) MAIN (Demo)
// =========================================================

int main() {
    InventoryManager* manager = InventoryManager::getInstance();

    Warehouse* wh1 = new Warehouse(1, "Warehouse-1", "Bangalore");
    Warehouse* wh2 = new Warehouse(2, "Warehouse-2", "Delhi");

    manager->addWarehouse(wh1);
    manager->addWarehouse(wh2);

    // Observers
    manager->addObserver(new SupplierNotifier("ABC-Supplier", "abc@supplier.com"));
    manager->addObserver(new DashboardAlertSystem("WARNING"));

    // Strategy
    manager->setReplenishmentStrategy(new JustInTimeStrategy());

    // ProductFactory
    ProductFactory* factory = manager->getProductFactory();

    // Create products
    Product* laptop = factory->createProduct(ProductCategory::ELECTRONICS, "SKU123", "Laptop", 1000.0, 0, 25);
    Product* tshirt = factory->createProduct(ProductCategory::CLOTHING, "SKU456", "T-Shirt", 20.0, 0, 100);
    Product* apple  = factory->createProduct(ProductCategory::GROCERY, "SKU789", "Apple", 1.0, 0, 200);

    // Add stock
    wh1->addProduct(laptop, 15);   // low stock
    wh1->addProduct(tshirt, 120);  // ok
    wh2->addProduct(apple, 50);    // low stock

    wh1->printInventory();
    wh2->printInventory();

    // Full inventory check (alerts + strategy)
    manager->performInventoryCheck();

    // Transfer stock
    cout << "\n[Transfer] Transfer 10 Apple from Warehouse-2 to Warehouse-1\n";
    manager->transferStock(2, 1, "SKU789", 10);

    wh1->printInventory();
    wh2->printInventory();

    // Switch strategy
    manager->setReplenishmentStrategy(new BulkOrderStrategy());

    cout << "\n[Check] Checking SKU123...\n";
    manager->checkAndReplenish("SKU123");

    return 0;
}
