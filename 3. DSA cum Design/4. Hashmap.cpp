#include <bits/stdc++.h>
#include <iostream>
using namespace std;

class Node {
public:
    string key;
    string value;
    Node* next;

    Node(string k, string v) {
        key = k;
        value = v;
        next = nullptr;
    }
};

class HashMap {
private:
    Node** buckets;       // dynamic array of head pointers
    int bucketCount;      // b
    int sizeCount;        // n
    const double LOAD_FACTOR = 0.7;
    const long long p = 31;   // base

private:
    // ✅ p*a[0] + p^2*a[1] + ... so on
    long long hashFunction(const string& key) {
        long long hashValue = 0;
        long long power = p; // p^1

        for (int i = 0; i < (int)key.size(); i++) {
            int a = (int)key[i];               // taking ASCII value directly
            hashValue += power * a;            // p^k * a[i]
            power = power * p;                 // next power (p^2, p^3, ...)
        }
        return hashValue;
    }

    // ✅ compression function: bring hash into bucket range
    int getBucketIndex(const string& key) {
        long long hashValue = hashFunction(key);

        // avoid negative due to overflow scenarios
        if (hashValue < 0) hashValue = -hashValue;

        return (int)(hashValue % bucketCount);
    }

    // ✅ search key inside linked list and return node
    Node* searchInBucket(Node* head, const string& key) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->key == key) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // ✅ rehash when load factor > 0.7
    void rehash() {
        int oldBucketCount = bucketCount;
        Node** oldBuckets = buckets;

        bucketCount = bucketCount * 2;
        buckets = new Node*[bucketCount];

        for (int i = 0; i < bucketCount; i++) {
            buckets[i] = nullptr;
        }

        // reset size and re-insert everything
        int oldSize = sizeCount;
        sizeCount = 0;

        for (int i = 0; i < oldBucketCount; i++) {
            Node* head = oldBuckets[i];

            while (head != nullptr) {
                put(head->key, head->value);
                head = head->next;
            }
        }

        // (No destructor asked, so we are not freeing old nodes here)
        // This is fine for interview-style / learning purpose.
    }

public:
    HashMap(int initialBucketCount = 8) {
        bucketCount = initialBucketCount;
        sizeCount = 0;

        buckets = new Node*[bucketCount];
        for (int i = 0; i < bucketCount; i++) {
            buckets[i] = nullptr;
        }
    }

    // ✅ Insert / Update
    void put(string key, string value) {
        int idx = getBucketIndex(key);

        Node* head = buckets[idx];
        Node* existing = searchInBucket(head, key);

        // update if exists
        if (existing != nullptr) {
            existing->value = value;
            return;
        }

        // insert new at head (fast)
        Node* newNode = new Node(key, value);
        newNode->next = buckets[idx];
        buckets[idx] = newNode;

        sizeCount++;

        double load = (1.0 * sizeCount) / bucketCount;
        if (load > LOAD_FACTOR) {
            rehash();
        }
    }

    // ✅ Get value
    string get(string key) {
        int idx = getBucketIndex(key);
        Node* node = searchInBucket(buckets[idx], key);

        if (node == nullptr) return "NOT_FOUND";
        return node->value;
    }

    // ✅ Contains Key
    bool containsKey(string key) {
        int idx = getBucketIndex(key);
        Node* node = searchInBucket(buckets[idx], key);
        return node != nullptr;
    }

    // ✅ Remove key
    bool removeKey(string key) {
        int idx = getBucketIndex(key);

        Node* head = buckets[idx];
        Node* prev = nullptr;
        Node* curr = head;

        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    buckets[idx] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                sizeCount--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    // ✅ Size
    int size() {
        return sizeCount;
    }

    // ✅ For debugging: print all buckets
    void print() {
        cout << "---- HashMap ----\n";
        cout << "Buckets: " << bucketCount << ", Size: " << sizeCount
             << ", Load: " << (1.0 * sizeCount) / bucketCount << "\n\n";

        for (int i = 0; i < bucketCount; i++) {
            cout << i << " : ";
            Node* head = buckets[i];
            while (head != nullptr) {
                cout << "(" << head->key << " -> " << head->value << ") ";
                head = head->next;
            }
            cout << "\n";
        }
        cout << "-----------------\n";
    }
};

int main() {
    HashMap mp;

    mp.put("name", "Rahul");
    mp.put("city", "Bangalore");
    mp.put("company", "Infinera");

    cout << mp.get("name") << "\n";      // Rahul
    cout << mp.get("city") << "\n";      // Bangalore
    cout << mp.get("unknown") << "\n";   // NOT_FOUND

    mp.put("name", "Rahul Bharti"); // update
    cout << mp.get("name") << "\n"; // Rahul Bharti

    cout << mp.containsKey("company") << "\n"; // 1
    cout << mp.removeKey("company") << "\n";   // 1
    cout << mp.containsKey("company") << "\n"; // 0

    // Trigger rehash quickly
    mp.put("a", "1");
    mp.put("b", "2");
    mp.put("c", "3");
    mp.put("d", "4");
    mp.put("e", "5");
    mp.put("f", "6");
    mp.put("g", "7");

    mp.print();

    return 0;
}
