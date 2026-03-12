#include <bits/stdc++.h>
#include <queue>
#include <iostream>
#include <map>
using namespace std;

/* ===========================================================
   ENUMS (Java enum style)
   =========================================================== */

enum class BallType {
    NORMAL,
    WIDEBALL,
    NOBALL
};

enum class RunType {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    SIX
};

enum class WicketType {
    RUNOUT,
    BOLD,
    CATCH
};

enum class PlayerType {
    BATSMAN,
    BOWLER,
    WICKETKEEPER,
    CAPTAIN,
    ALLROUNDER
};

/* ===========================================================
   Forward declarations (because classes reference each other)
   =========================================================== */

class Team;
class OverDetails;
class BallDetails;
class PlayerDetails;
class Wicket;

/* ===========================================================
   Utility functions
   =========================================================== */

double getRandom01() {
    return (double)rand() / (double)RAND_MAX; // 0 to 1
}

int runTypeToRuns(RunType rt) {
    if (rt == RunType::ONE) return 1;
    if (rt == RunType::TWO) return 2;
    if (rt == RunType::THREE) return 3;
    if (rt == RunType::FOUR) return 4;
    if (rt == RunType::SIX) return 6;
    return 0;
}

string runTypeToString(RunType rt) {
    if (rt == RunType::ZERO) return "0";
    if (rt == RunType::ONE) return "1";
    if (rt == RunType::TWO) return "2";
    if (rt == RunType::THREE) return "3";
    if (rt == RunType::FOUR) return "4";
    if (rt == RunType::SIX) return "6";
    return "?";
}

/* ===========================================================
   Score Cards
   =========================================================== */

class BattingScoreCard {
public:
    int totalRuns = 0;
    int totalBallsPlayed = 0;
    int totalFours = 0;
    int totalSix = 0;
    double strikeRate = 0.0;
    Wicket* wicketDetails = nullptr;
};

class BowlingScoreCard {
public:
    int totalOversCount = 0;
    int runsGiven = 0;
    int wicketsTaken = 0;
    int noBallCount = 0;
    int wideBallCount = 0;
    double economyRate = 0.0;
};

/* ===========================================================
   Person + PlayerDetails
   =========================================================== */

class Person {
public:
    string name;
    int age = 0;
    string address;
};

class PlayerDetails {
public:
    Person person;
    PlayerType playerType;

    BattingScoreCard battingScoreCard;
    BowlingScoreCard bowlingScoreCard;

    PlayerDetails() {}

    PlayerDetails(Person p, PlayerType type) {
        person = p;
        playerType = type;
    }

    void printBattingScoreCard() {
        cout << "PlayerName: " << person.name
             << " -- totalRuns: " << battingScoreCard.totalRuns
             << " -- totalBallsPlayed: " << battingScoreCard.totalBallsPlayed
             << " -- 4s: " << battingScoreCard.totalFours
             << " -- 6s: " << battingScoreCard.totalSix
             << " -- outby: ";

        if (battingScoreCard.wicketDetails != nullptr) {
            cout << battingScoreCard.wicketDetails->takenBy->person.name;
        } else {
            cout << "notout";
        }
        cout << "\n";
    }

    void printBowlingScoreCard() {
        cout << "PlayerName: " << person.name
             << " -- totalOversThrown: " << bowlingScoreCard.totalOversCount
             << " -- totalRunsGiven: " << bowlingScoreCard.runsGiven
             << " -- WicketsTaken: " << bowlingScoreCard.wicketsTaken
             << "\n";
    }
};

/* ===========================================================
   Wicket
   =========================================================== */

class Wicket {
public:
    WicketType wicketType;
    PlayerDetails* takenBy;
    OverDetails* overDetail;
    BallDetails* ballDetail;

    Wicket(WicketType wt, PlayerDetails* by, OverDetails* over, BallDetails* ball) {
        wicketType = wt;
        takenBy = by;
        overDetail = over;
        ballDetail = ball;
    }
};

/* ===========================================================
   ScoreUpdater Observer (Interface)
   =========================================================== */

class ScoreUpdaterObserver {
public:
    virtual void update(BallDetails* ballDetails) = 0; // pure virtual like Java interface
};

/* ===========================================================
   BallDetails
   =========================================================== */

class BattingScoreUpdater;
class BowlingScoreUpdater;

class BallDetails {
public:
    int ballNumber;
    BallType ballType;
    RunType runType;

    PlayerDetails* playedBy = nullptr;
    PlayerDetails* bowledBy = nullptr;

    Wicket* wicket = nullptr;

    vector<ScoreUpdaterObserver*> scoreUpdaterObserverList;

    BallDetails(int bn);

    void startBallDelivery(Team* battingTeam, Team* bowlingTeam, OverDetails* over);

private:
    void notifyUpdaters(BallDetails* ballDetails);
    RunType getRunType();
    bool isWicketTaken();
};

/* ===========================================================
   OverDetails
   =========================================================== */

class OverDetails {
public:
    int overNumber;
    vector<BallDetails*> balls;
    int extraBallsCount = 0;
    PlayerDetails* bowledBy = nullptr;

    OverDetails(int on, PlayerDetails* bowler) {
        overNumber = on;
        bowledBy = bowler;
    }

    bool startOver(Team* battingTeam, Team* bowlingTeam, int runsToWin);
};

/* ===========================================================
   Player Controllers
   =========================================================== */

class PlayerBattingController {
public:
    queue<PlayerDetails*> yetToPlay;
    PlayerDetails* striker = nullptr;
    PlayerDetails* nonStriker = nullptr;

    PlayerBattingController() {}

    PlayerBattingController(queue<PlayerDetails*> playing11) {
        yetToPlay = playing11;
    }

    void getNextPlayer() {
        if (yetToPlay.empty()) {
            throw runtime_error("No more batsman left");
        }

        if (striker == nullptr) {
            striker = yetToPlay.front();
            yetToPlay.pop();
        }

        if (nonStriker == nullptr) {
            nonStriker = yetToPlay.front();
            yetToPlay.pop();
        }
    }

    PlayerDetails* getStriker() { return striker; }
    PlayerDetails* getNonStriker() { return nonStriker; }

    void setStriker(PlayerDetails* p) { striker = p; }
    void setNonStriker(PlayerDetails* p) { nonStriker = p; }
};

class PlayerBowlingController {
public:
    deque<PlayerDetails*> bowlersList;
    map<PlayerDetails*, int> bowlerVsOverCount;
    PlayerDetails* currentBowler = nullptr;

    PlayerBowlingController() {}

    PlayerBowlingController(vector<PlayerDetails*> bowlers) {
        setBowlersList(bowlers);
    }

    void setBowlersList(vector<PlayerDetails*> bowlers) {
        bowlersList.clear();
        bowlerVsOverCount.clear();

        for (auto bowler : bowlers) {
            bowlersList.push_back(bowler);
            bowlerVsOverCount[bowler] = 0;
        }
    }

    void getNextBowler(int maxOverCountPerBowler) {
        PlayerDetails* playerDetails = bowlersList.front();
        bowlersList.pop_front();

        if (bowlerVsOverCount[playerDetails] + 1 == maxOverCountPerBowler) {
            currentBowler = playerDetails;
        } else {
            currentBowler = playerDetails;
            bowlersList.push_back(playerDetails);
            bowlerVsOverCount[playerDetails] = bowlerVsOverCount[playerDetails] + 1;
        }
    }

    PlayerDetails* getCurrentBowler() { return currentBowler; }
};

/* ===========================================================
   Team
   =========================================================== */

class Team {
public:
    string teamName;

    queue<PlayerDetails*> playing11;     // original playing11 queue (same)
    vector<PlayerDetails*> bench;        // not used much here

    PlayerBattingController battingController;
    PlayerBowlingController bowlingController;

    bool isWinner = false;

    Team(string name,
         queue<PlayerDetails*> p11,
         vector<PlayerDetails*> b,
         vector<PlayerDetails*> bowlers)
    {
        teamName = name;
        playing11 = p11;
        bench = b;
        battingController = PlayerBattingController(p11);
        bowlingController = PlayerBowlingController(bowlers);
    }

    string getTeamName() { return teamName; }

    void chooseNextBatsMan() {
        battingController.getNextPlayer();
    }

    void chooseNextBowler(int maxOverCountPerBowler) {
        bowlingController.getNextBowler(maxOverCountPerBowler);
    }

    PlayerDetails* getStriker() { return battingController.getStriker(); }
    PlayerDetails* getNonStriker() { return battingController.getNonStriker(); }

    void setStriker(PlayerDetails* p) { battingController.setStriker(p); }
    void setNonStriker(PlayerDetails* p) { battingController.setNonStriker(p); }

    PlayerDetails* getCurrentBowler() { return bowlingController.getCurrentBowler(); }

    void printBattingScoreCard() {
        // playing11 queue ka copy bna ke print karenge (queue iterate nahi hota direct)
        queue<PlayerDetails*> temp = playing11;
        while (!temp.empty()) {
            temp.front()->printBattingScoreCard();
            temp.pop();
        }
    }

    void printBowlingScoreCard() {
        queue<PlayerDetails*> temp = playing11;
        while (!temp.empty()) {
            PlayerDetails* p = temp.front();
            temp.pop();
            if (p->bowlingScoreCard.totalOversCount > 0) {
                p->printBowlingScoreCard();
            }
        }
    }

    int getTotalRuns() {
        int totalRun = 0;
        queue<PlayerDetails*> temp = playing11;
        while (!temp.empty()) {
            PlayerDetails* p = temp.front();
            temp.pop();
            totalRun += p->battingScoreCard.totalRuns;
        }
        return totalRun;
    }
};

/* ===========================================================
   Score Updaters
   =========================================================== */

class BattingScoreUpdater : public ScoreUpdaterObserver {
public:
    void update(BallDetails* ballDetails) override {
        int run = runTypeToRuns(ballDetails->runType);

        if (ballDetails->runType == RunType::FOUR) {
            ballDetails->playedBy->battingScoreCard.totalFours++;
        }
        if (ballDetails->runType == RunType::SIX) {
            ballDetails->playedBy->battingScoreCard.totalSix++;
        }

        ballDetails->playedBy->battingScoreCard.totalRuns += run;
        ballDetails->playedBy->battingScoreCard.totalBallsPlayed++;

        if (ballDetails->wicket != nullptr) {
            ballDetails->playedBy->battingScoreCard.wicketDetails = ballDetails->wicket;
        }
    }
};

class BowlingScoreUpdater : public ScoreUpdaterObserver {
public:
    void update(BallDetails* ballDetails) override {
        // over complete count -> last ball normal ho
        if (ballDetails->ballNumber == 6 && ballDetails->ballType == BallType::NORMAL) {
            ballDetails->bowledBy->bowlingScoreCard.totalOversCount++;
        }

        int run = runTypeToRuns(ballDetails->runType);
        ballDetails->bowledBy->bowlingScoreCard.runsGiven += run;

        if (ballDetails->wicket != nullptr) {
            ballDetails->bowledBy->bowlingScoreCard.wicketsTaken++;
        }

        if (ballDetails->ballType == BallType::NOBALL) {
            ballDetails->bowledBy->bowlingScoreCard.noBallCount++;
        }

        if (ballDetails->ballType == BallType::WIDEBALL) {
            ballDetails->bowledBy->bowlingScoreCard.wideBallCount++;
        }
    }
};

/* ===========================================================
   BallDetails Implementation
   =========================================================== */

BallDetails::BallDetails(int bn) {
    ballNumber = bn;

    // observers attach (same as Java)
    scoreUpdaterObserverList.push_back(new BowlingScoreUpdater());
    scoreUpdaterObserverList.push_back(new BattingScoreUpdater());
}

void BallDetails::startBallDelivery(Team* battingTeam, Team* bowlingTeam, OverDetails* over) {
    playedBy = battingTeam->getStriker();
    bowledBy = over->bowledBy;

    ballType = BallType::NORMAL;

    if (isWicketTaken()) {
        runType = RunType::ZERO;
        wicket = new Wicket(WicketType::BOLD, bowlingTeam->getCurrentBowler(), over, this);
        battingTeam->setStriker(nullptr);
    } else {
        runType = getRunType();

        if (runType == RunType::ONE || runType == RunType::THREE) {
            PlayerDetails* temp = battingTeam->getStriker();
            battingTeam->setStriker(battingTeam->getNonStriker());
            battingTeam->setNonStriker(temp);
        }
    }

    notifyUpdaters(this);
}

void BallDetails::notifyUpdaters(BallDetails* ballDetails) {
    for (auto observer : scoreUpdaterObserverList) {
        observer->update(ballDetails);
    }
}

RunType BallDetails::getRunType() {
    double val = getRandom01();

    if (val <= 0.2) {
        return RunType::ONE;
    } else if (val >= 0.3 && val <= 0.5) {
        return RunType::TWO;
    } else {
        if (val >= 0.6 && val <= 0.8) return RunType::FOUR;
        return RunType::SIX;
    }
}

bool BallDetails::isWicketTaken() {
    return getRandom01() < 0.2;
}

/* ===========================================================
   OverDetails Implementation
   =========================================================== */

bool OverDetails::startOver(Team* battingTeam, Team* bowlingTeam, int runsToWin) {
    int ballCount = 1;

    while (ballCount <= 6) {
        BallDetails* ball = new BallDetails(ballCount);
        ball->startBallDelivery(battingTeam, bowlingTeam, this);

        if (ball->ballType == BallType::NORMAL) {
            balls.push_back(ball);
            ballCount++;

            if (ball->wicket != nullptr) {
                battingTeam->chooseNextBatsMan();
            }

            if (runsToWin != -1 && battingTeam->getTotalRuns() >= runsToWin) {
                battingTeam->isWinner = true;
                return true;
            }
        } else {
            extraBallsCount++;
        }
    }

    return false;
}

/* ===========================================================
   MatchType (Interface) + Types
   =========================================================== */

class MatchType {
public:
    virtual int noOfOvers() = 0;
    virtual int maxOverCountBowlers() = 0;
};

class OneDayMatchType : public MatchType {
public:
    int noOfOvers() override { return 50; }
    int maxOverCountBowlers() override { return 10; }
};

class T20MatchType : public MatchType {
public:
    int noOfOvers() override { return 20; }
    int maxOverCountBowlers() override { return 5; }
};

/* ===========================================================
   InningDetails
   =========================================================== */

class InningDetails {
public:
    Team* battingTeam;
    Team* bowlingTeam;
    MatchType* matchType;
    vector<OverDetails*> overs;

    InningDetails(Team* bt, Team* bw, MatchType* mt) {
        battingTeam = bt;
        bowlingTeam = bw;
        matchType = mt;
    }

    void start(int runsToWin) {
        try {
            battingTeam->chooseNextBatsMan();
        } catch (...) {
            // ignore (same)
        }

        int noOfOvers = matchType->noOfOvers();

        for (int overNumber = 1; overNumber <= noOfOvers; overNumber++) {
            bowlingTeam->chooseNextBowler(matchType->maxOverCountBowlers());

            OverDetails* over = new OverDetails(overNumber, bowlingTeam->getCurrentBowler());
            overs.push_back(over);

            try {
                bool won = over->startOver(battingTeam, bowlingTeam, runsToWin);
                if (won) break;
            } catch (...) {
                break;
            }

            // striker change after over
            PlayerDetails* temp = battingTeam->getStriker();
            battingTeam->setStriker(battingTeam->getNonStriker());
            battingTeam->setNonStriker(temp);
        }
    }

    int getTotalRuns() {
        return battingTeam->getTotalRuns();
    }
};

/* ===========================================================
   Match
   =========================================================== */

class Match {
public:
    Team* teamA;
    Team* teamB;

    string venue;
    Team* tossWinner = nullptr;

    InningDetails* innings[2] = {nullptr, nullptr};
    MatchType* matchType;

    Match(Team* a, Team* b, string v, MatchType* mt) {
        teamA = a;
        teamB = b;
        venue = v;
        matchType = mt;
    }

    void startMatch() {
        tossWinner = toss(teamA, teamB);

        for (int inning = 1; inning <= 2; inning++) {
            Team* battingTeam;
            Team* bowlingTeam;
            InningDetails* inningDetails;

            if (inning == 1) {
                battingTeam = tossWinner;
                bowlingTeam = (tossWinner->getTeamName() == teamA->getTeamName()) ? teamB : teamA;

                inningDetails = new InningDetails(battingTeam, bowlingTeam, matchType);
                inningDetails->start(-1);
            } else {
                bowlingTeam = tossWinner;
                battingTeam = (tossWinner->getTeamName() == teamA->getTeamName()) ? teamB : teamA;

                inningDetails = new InningDetails(battingTeam, bowlingTeam, matchType);
                inningDetails->start(innings[0]->getTotalRuns());

                // winner check
                if (bowlingTeam->getTotalRuns() > battingTeam->getTotalRuns()) {
                    bowlingTeam->isWinner = true;
                }
            }

            innings[inning - 1] = inningDetails;

            cout << "\n";
            cout << "INNING " << inning << " -- total Run: " << battingTeam->getTotalRuns() << "\n";
            cout << "---Batting ScoreCard : " << battingTeam->teamName << "---\n";
            battingTeam->printBattingScoreCard();

            cout << "\n";
            cout << "---Bowling ScoreCard : " << bowlingTeam->teamName << "---\n";
            bowlingTeam->printBowlingScoreCard();
        }

        cout << "\n";
        if (teamA->isWinner) {
            cout << "---WINNER--- " << teamA->teamName << "\n";
        } else {
            cout << "---WINNER--- " << teamB->teamName << "\n";
        }
    }

private:
    Team* toss(Team* a, Team* b) {
        return (getRandom01() < 0.5) ? a : b;
    }
};

/* ===========================================================
   Main
   =========================================================== */

class Main {
public:
    Team* addTeam(string name) {
        queue<PlayerDetails*> players;

        PlayerDetails* p1  = addPlayer(name + "1",  PlayerType::ALLROUNDER);
        PlayerDetails* p2  = addPlayer(name + "2",  PlayerType::ALLROUNDER);
        PlayerDetails* p3  = addPlayer(name + "3",  PlayerType::ALLROUNDER);
        PlayerDetails* p4  = addPlayer(name + "4",  PlayerType::ALLROUNDER);
        PlayerDetails* p5  = addPlayer(name + "5",  PlayerType::ALLROUNDER);
        PlayerDetails* p6  = addPlayer(name + "6",  PlayerType::ALLROUNDER);
        PlayerDetails* p7  = addPlayer(name + "7",  PlayerType::ALLROUNDER);
        PlayerDetails* p8  = addPlayer(name + "8",  PlayerType::ALLROUNDER);
        PlayerDetails* p9  = addPlayer(name + "9",  PlayerType::ALLROUNDER);
        PlayerDetails* p10 = addPlayer(name + "10", PlayerType::ALLROUNDER);
        PlayerDetails* p11 = addPlayer(name + "11", PlayerType::ALLROUNDER);

        players.push(p1);
        players.push(p2);
        players.push(p3);
        players.push(p4);
        players.push(p5);
        players.push(p6);
        players.push(p7);
        players.push(p8);
        players.push(p9);
        players.push(p10);
        players.push(p11);

        vector<PlayerDetails*> bowlers;
        bowlers.push_back(p8);
        bowlers.push_back(p9);
        bowlers.push_back(p10);
        bowlers.push_back(p11);

        Team* team = new Team(name, players, {}, bowlers);
        return team;
    }

    PlayerDetails* addPlayer(string name, PlayerType type) {
        Person person;
        person.name = name;

        PlayerDetails* p = new PlayerDetails(person, type);
        return p;
    }
};

int main() {
    srand(time(nullptr));

    Main ob;
    Team* teamA = ob.addTeam("India");
    Team* teamB = ob.addTeam("SriLanka");

    MatchType* matchType = new T20MatchType();

    Match match(teamA, teamB, "SMS STADIUM", matchType);
    match.startMatch();

    return 0;
}
