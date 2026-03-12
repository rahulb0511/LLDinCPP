#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// ======================================================
// 1) Mediator Interface -> Example - Online Bidding System
// ======================================================
class AuctionMediator {
public:
    virtual void addBidder(class Bidder* bidder) = 0;
    virtual void placeBid(Bidder* bidder, int amount) = 0;
};

// ======================================================
// 2) Colleague Abstract Class (Bidder)
// ======================================================
class Bidder {
protected:
    string name;
    AuctionMediator* auction;

public:
    Bidder(string name, AuctionMediator* auction) {
        this->name = name;
        this->auction = auction;
    }

    string getName() {
        return name;
    }

    // Bidder bids through mediator
    void bid(int amount) {
        cout << "\n" << name << " placed a bid of " << amount << "\n";
        auction->placeBid(this, amount);
    }

    // Mediator notifies all bidders
    virtual void receiveNotification(string msg) = 0;
};

// ======================================================
// 3) Concrete Mediator: Auction
// ======================================================
class Auction : public AuctionMediator {
private:
    vector<Bidder*> bidders;
    int highestBid = 0;
    string highestBidder = "None";

public:
    void addBidder(Bidder* bidder) override {
        bidders.push_back(bidder);
        cout << bidder->getName() << " joined the auction\n";
    }

    void placeBid(Bidder* bidder, int amount) override {

        if (amount <= highestBid) {
            bidder->receiveNotification(
                "Bid rejected! Current highest bid is " + to_string(highestBid) +
                " by " + highestBidder
            );
            return;
        }

        // Accept bid
        highestBid = amount;
        highestBidder = bidder->getName();

        // Notify all other bidders
        for (auto b : bidders) {
            if (b != bidder) {
                b->receiveNotification(
                    "New Highest Bid: " + to_string(highestBid) +
                    " by " + highestBidder
                );
            }
        }
    }
};

// ======================================================
// 4) Concrete Colleague: NormalBidder
// ======================================================
class NormalBidder : public Bidder {
public:
    NormalBidder(string name, AuctionMediator* auction) : Bidder(name, auction) {}

    void receiveNotification(string msg) override {
        cout << "[" << name << " Notification] " << msg << "\n";
    }
};

// ======================================================
// MAIN
// ======================================================
int main() {

    // Mediator
    AuctionMediator* auction = new Auction();

    // Bidders (colleagues)
    Bidder* rahul = new NormalBidder("Rahul", auction);
    Bidder* aman  = new NormalBidder("Aman", auction);
    Bidder* riya  = new NormalBidder("Riya", auction);

    // register bidders to auction
    auction->addBidder(rahul);
    auction->addBidder(aman);
    auction->addBidder(riya);

    // bidding starts
    rahul->bid(1000);
    aman->bid(1200);
    riya->bid(1100);   // rejected
    rahul->bid(1500);

    return 0;
}
