#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =======================
// Cart Item
// =======================
class CartItem {
public:
    string name;
    string type;
    double price;

    CartItem(string name, string type, double price) {
        this->name = name;
        this->type = type;
        this->price = price;
    }
};

// =======================
// Context (state shared by decorators)
// =======================
class CouponContext {
public:
    int nextItemIndex = 0; // used by "next item coupon"
};

// =======================
// Component Interface
// =======================
class PriceCalculator {
public:
    virtual vector<double> calculateFinalPrices(vector<CartItem>& items, CouponContext& ctx) = 0;
};

// =======================
// Base Component (no coupon)
// =======================
class BasePriceCalculator : public PriceCalculator {
public:
    vector<double> calculateFinalPrices(vector<CartItem>& items, CouponContext& ctx) override {
        vector<double> prices;
        for (auto &item : items) prices.push_back(item.price);
        return prices;
    }
};

// =======================
// Decorator Base Class
// =======================
class CouponDecorator : public PriceCalculator {
protected:
    PriceCalculator* wrapped; // jis calculator ko wrap kar rahe hain

public:
    CouponDecorator(PriceCalculator* wrapped) {
        this->wrapped = wrapped;
    }
};

// =======================================================
// Decorator 1: N% off on ALL items
// =======================================================
class PercentageOffAllCoupon : public CouponDecorator {
private:
    double percent;

public:
    PercentageOffAllCoupon(PriceCalculator* wrapped, double percent)
        : CouponDecorator(wrapped) {
        this->percent = percent;
    }

    vector<double> calculateFinalPrices(vector<CartItem>& items, CouponContext& ctx) override {
        // 1) pehle wrapped calculator ka output lo
        vector<double> prices = wrapped->calculateFinalPrices(items, ctx);

        // 2) then apply this coupon on top
        for (int i = 0; i < (int)prices.size(); i++) {
            prices[i] = prices[i] - (prices[i] * percent / 100.0);
        }
        return prices;
    }
};

// =======================================================
// Decorator 2: P% off on NEXT item (only one item)
// =======================================================
class PercentageOffNextItemCoupon : public CouponDecorator {
private:
    double percent;

public:
    PercentageOffNextItemCoupon(PriceCalculator* wrapped, double percent)
        : CouponDecorator(wrapped) {
        this->percent = percent;
    }

    vector<double> calculateFinalPrices(vector<CartItem>& items, CouponContext& ctx) override {
        vector<double> prices = wrapped->calculateFinalPrices(items, ctx);

        if (ctx.nextItemIndex < (int)prices.size()) {
            int idx = ctx.nextItemIndex;
            prices[idx] = prices[idx] - (prices[idx] * percent / 100.0);
            ctx.nextItemIndex++; // move pointer
        }

        return prices;
    }
};

// =======================================================
// Decorator 3: D% off on Nth item of Type T
// =======================================================
class PercentageOffNthItemOfTypeCoupon : public CouponDecorator {
private:
    string targetType;
    int n;
    double percent;

public:
    PercentageOffNthItemOfTypeCoupon(PriceCalculator* wrapped, string targetType, int n, double percent)
        : CouponDecorator(wrapped) {
        this->targetType = targetType;
        this->n = n;
        this->percent = percent;
    }

    vector<double> calculateFinalPrices(vector<CartItem>& items, CouponContext& ctx) override {
        vector<double> prices = wrapped->calculateFinalPrices(items, ctx);

        int count = 0;
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].type == targetType) {
                count++;
                if (count == n) {
                    prices[i] = prices[i] - (prices[i] * percent / 100.0);
                    break; // only once
                }
            }
        }
        return prices;
    }
};

// =======================
// Helper: Total
// =======================
double getTotal(vector<double>& prices) {
    double sum = 0;
    for (double p : prices) sum += p;
    return sum;
}

// =======================
// MAIN
// =======================
int main() {

    vector<CartItem> items;
    items.push_back(CartItem("iPhone 15", "ELECTRONICS", 80000));
    items.push_back(CartItem("Chips", "FOOD", 100));
    items.push_back(CartItem("Laptop", "ELECTRONICS", 60000));
    items.push_back(CartItem("Burger", "FOOD", 200));

    CouponContext ctx;

    // ✅ DECORATOR CHAIN (actual decorator usage)
    PriceCalculator* calculator = new BasePriceCalculator();

    // coupons sequentially wrap the previous one (Decorator pattern)
    calculator = new PercentageOffAllCoupon(calculator, 10);                  // 10% off all
    calculator = new PercentageOffNextItemCoupon(calculator, 20);             // 20% off next item
    calculator = new PercentageOffNthItemOfTypeCoupon(calculator, "FOOD", 2, 50); // 50% off 2nd FOOD

    vector<double> finalPrices = calculator->calculateFinalPrices(items, ctx);

    cout << "------ FINAL ITEM PRICES ------\n";
    for (int i = 0; i < (int)items.size(); i++) {
        cout << items[i].name << " -> " << finalPrices[i] << "\n";
    }
    cout << "------------------------------\n";

    cout << "Final Total = " << getTotal(finalPrices) << "\n";

    return 0;
}
