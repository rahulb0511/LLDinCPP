#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/* Functional Requirements
Add movies with id, name, duration and map them to a city

Fetch all movies running in a given city

Add theatres in a city with screens + shows

Each screen should have seats (0–99) with categories: Silver / Gold / Platinum

Create shows with showId, movie, screen, startTime

Get all shows for a movie in a city, grouped theatre-wise

Allow user to book a specific seat number for a selected show

Prevent booking if the seat is already booked (no double booking)

On successful booking, create a Booking record with show + bookedSeats + payment reference

Print booking result as SUCCESS / FAIL with relevant details
*/

// ============================================================
// STEP 1) ENUMS (Same as Java Enums)
// ============================================================

enum class SeatCategory {
    SILVER,
    GOLD,
    PLATINUM
};

enum class City {
    Bangalore,
    Delhi
};

// Helper function: City to string (for printing)
string cityToString(City city) {
    if (city == City::Bangalore) return "Bangalore";
    return "Delhi";
}

// ============================================================
// STEP 2) BASIC ENTITIES (Movie, Seat, Screen, Show, Theatre)
// We will keep them simple like Java POJO classes
// ============================================================

// -------------------- Movie --------------------
class Movie {
public:
    int movieId;
    string movieName;
    int movieDurationInMinutes;

    Movie() {
        movieId = 0;
        movieDurationInMinutes = 0;
    }

    int getMovieId() { return movieId; }
    void setMovieId(int movieId) { this->movieId = movieId; }

    string getMovieName() { return movieName; }
    void setMovieName(string movieName) { this->movieName = movieName; }

    int getMovieDuration() { return movieDurationInMinutes; }
    void setMovieDuration(int movieDuration) { this->movieDurationInMinutes = movieDuration; }
};

// -------------------- Seat --------------------
class Seat {
public:
    int seatId;
    int row; // unused (same as Java code)
    SeatCategory seatCategory;

    Seat() {
        seatId = 0;
        row = 0;
        seatCategory = SeatCategory::SILVER;
    }

    int getSeatId() { return seatId; }
    void setSeatId(int seatId) { this->seatId = seatId; }

    int getRow() { return row; }
    void setRow(int row) { this->row = row; }

    SeatCategory getSeatCategory() { return seatCategory; }
    void setSeatCategory(SeatCategory seatCategory) { this->seatCategory = seatCategory; }
};

// -------------------- Screen --------------------
class Screen {
public:
    int screenId;
    vector<Seat*> seats;

    Screen() { screenId = 0; }

    int getScreenId() { return screenId; }
    void setScreenId(int screenId) { this->screenId = screenId; }

    vector<Seat*> getSeats() { return seats; }
    void setSeats(vector<Seat*> seats) { this->seats = seats; }
};

// -------------------- Show --------------------
class Show {
public:
    int showId;
    Movie* movie;
    Screen* screen;
    int showStartTime;
    vector<int> bookedSeatIds; // stores booked seat numbers

    Show() {
        showId = 0;
        movie = nullptr;
        screen = nullptr;
        showStartTime = 0;
    }

    int getShowId() { return showId; }
    void setShowId(int showId) { this->showId = showId; }

    Movie* getMovie() { return movie; }
    void setMovie(Movie* movie) { this->movie = movie; }

    Screen* getScreen() { return screen; }
    void setScreen(Screen* screen) { this->screen = screen; }

    int getShowStartTime() { return showStartTime; }
    void setShowStartTime(int showStartTime) { this->showStartTime = showStartTime; }

    vector<int> getBookedSeatIds() { return bookedSeatIds; }
    void setBookedSeatIds(vector<int> bookedSeatIds) { this->bookedSeatIds = bookedSeatIds; }
};

// -------------------- Theatre --------------------
class Theatre {
public:
    int theatreId;
    string address; // unused like Java
    City city;
    vector<Screen*> screens;
    vector<Show*> shows;

    Theatre() {
        theatreId = 0;
        address = "";
        city = City::Bangalore;
    }

    int getTheatreId() { return theatreId; }
    void setTheatreId(int theatreId) { this->theatreId = theatreId; }

    string getAddress() { return address; }
    void setAddress(string address) { this->address = address; }

    City getCity() { return city; }
    void setCity(City city) { this->city = city; }

    vector<Screen*> getScreen() { return screens; }
    void setScreen(vector<Screen*> screens) { this->screens = screens; }

    vector<Show*> getShows() { return shows; }
    void setShows(vector<Show*> shows) { this->shows = shows; }
};

// ============================================================
// STEP 3) BOOKING + PAYMENT (same as Java classes)
// Booking -> show, bookedSeats, payment
// Payment -> paymentId (dummy)
// ============================================================

class Payment {
public:
    int paymentId;
    Payment() { paymentId = 0; }
};

class Booking {
public:
    Show* show;
    vector<Seat*> bookedSeats;
    Payment* payment;

    Booking() {
        show = nullptr;
        payment = nullptr;
    }

    Show* getShow() { return show; }
    void setShow(Show* show) { this->show = show; }

    vector<Seat*> getBookedSeats() { return bookedSeats; }
    void setBookedSeats(vector<Seat*> bookedSeats) { this->bookedSeats = bookedSeats; }

    Payment* getPayment() { return payment; }
    void setPayment(Payment* payment) { this->payment = payment; }
};

// ============================================================
// STEP 4) MOVIE CONTROLLER
// cityVsMovies + allMovies
// addMovie(), getMovieByName(), getMoviesByCity()
// ============================================================

class MovieController {
public:
    // city -> movies
    unordered_map<int, vector<Movie*>> cityVsMovies;  //c++ me map ke key me obj store ni kr skte, you have to write hash
                                                     // function for that, better int/string me store kro aur class me 
                                                     //behaviour daal dojo ki string or int return kre unique

    // list of all movies
    vector<Movie*> allMovies;

    MovieController() {}

    void addMovie(Movie* movie, City city) {
        allMovies.push_back(movie);

        int c = (int)city;
        cityVsMovies[c].push_back(movie);
    }

    Movie* getMovieByName(string movieName) {
        for (auto movie : allMovies) {
            if (movie->getMovieName() == movieName) return movie;
        }
        return nullptr;
    }

    vector<Movie*> getMoviesByCity(City city) {
        int c = (int)city;
        return cityVsMovies[c];
    }
};

// ============================================================
// STEP 5) THEATRE CONTROLLER
// cityVsTheatre + allTheatre
// addTheatre()
// getAllShow(movie, city) -> theatre -> list of shows
// ============================================================

class TheatreController {
public:
    // city -> theatres
    unordered_map<int, vector<Theatre*>> cityVsTheatre;

    // all theatres
    vector<Theatre*> allTheatre;

    TheatreController() {}

    void addTheatre(Theatre* theatre, City city) {
        allTheatre.push_back(theatre);

        int c = (int)city;
        cityVsTheatre[c].push_back(theatre);
    }

    unordered_map<Theatre*, vector<Show*>> getAllShow(Movie* movie, City city) {
        unordered_map<Theatre*, vector<Show*>> theatreVsShows;

        int c = (int)city;
        vector<Theatre*> theatres = cityVsTheatre[c];

        for (auto theatre : theatres) {
            vector<Show*> givenMovieShows;

            for (auto show : theatre->getShows()) {
                if (movie != nullptr && show->getMovie() != nullptr &&
                    show->getMovie()->getMovieId() == movie->getMovieId()) {
                    givenMovieShows.push_back(show);
                }
            }

            if (!givenMovieShows.empty()) {
                theatreVsShows[theatre] = givenMovieShows;
            }
        }

        return theatreVsShows;
    }
};

// ============================================================
// STEP 6) BOOKMYSHOW SYSTEM (Main Class)
// It has controllers and contains the full flow:
//
// initialize()
//   -> createMovies()
//   -> createTheatre()
//
// createBooking(userCity, movieName)
//   -> find movie by city + name
//   -> find all theatres/shows for that movie in that city
//   -> pick first theatre and first show
//   -> book seat 30 if not already booked
// ============================================================

class BookMyShow {
public:
    MovieController* movieController;
    TheatreController* theatreController;

    BookMyShow() {
        movieController = new MovieController();
        theatreController = new TheatreController();
    }

    // --------------------------------------------------------
    // initialize() = createMovies + createTheatre
    // --------------------------------------------------------
    void initialize() {
        createMovies();
        createTheatre();
    }

    // --------------------------------------------------------
    // createBooking(city, movieName)
    // This is the same logic as your Java main example
    // --------------------------------------------------------
    void createBooking(City userCity, string movieName) {
        cout << "\n========== CREATE BOOKING ==========\n";
        cout << "UserCity=" << cityToString(userCity) << " Movie=" << movieName << "\n";

        // 1) Get movies running in this city
        vector<Movie*> moviesInCity = movieController->getMoviesByCity(userCity);

        // 2) Find movie by name
        Movie* interestedMovie = nullptr;
        for (auto movie : moviesInCity) {
            if (movie->getMovieName() == movieName) {
                interestedMovie = movie;
                break;
            }
        }

        if (interestedMovie == nullptr) {
            cout << "Movie not found in this city ❌\n";
            return;
        }

        // 3) Get all shows (theatre wise) for this movie in this city
        unordered_map<Theatre*, vector<Show*>> showsTheatreWise =
            theatreController->getAllShow(interestedMovie, userCity);

        if (showsTheatreWise.empty()) {
            cout << "No shows found for this movie ❌\n";
            return;
        }

        // 4) Pick first theatre and first show (same as Java)
        auto firstEntry = showsTheatreWise.begin();
        Theatre* theatre = firstEntry->first;
        vector<Show*> runningShows = firstEntry->second;
        Show* interestedShow = runningShows[0];

        cout << "Selected TheatreId=" << theatre->getTheatreId()
             << " ShowId=" << interestedShow->getShowId()
             << " StartTime=" << interestedShow->getShowStartTime() << "\n";

        // 5) Try booking seat 30
        int seatNumber = 30;

        vector<int> bookedSeatIds = interestedShow->getBookedSeatIds();

        bool alreadyBooked = false;
        for (int id : bookedSeatIds) {
            if (id == seatNumber) {
                alreadyBooked = true;
                break;
            }
        }

        if (alreadyBooked) {
            cout << "seat already booked, try again ❌\n";
            return;
        }

        // 6) Mark seat booked
        bookedSeatIds.push_back(seatNumber);
        interestedShow->setBookedSeatIds(bookedSeatIds);

        // 7) Create booking object
        Booking* booking = new Booking();

        // 8) Find seat object in screen seat list
        vector<Seat*> myBookedSeats;
        for (auto screenSeat : interestedShow->getScreen()->getSeats()) {
            if (screenSeat->getSeatId() == seatNumber) {
                myBookedSeats.push_back(screenSeat);
            }
        }

        booking->setBookedSeats(myBookedSeats);
        booking->setShow(interestedShow);

        cout << "BOOKING SUCCESSFUL ✅ Seat=" << seatNumber << "\n";
    }

private:
    // --------------------------------------------------------
    // createMovies()
    // Adds AVENGERS + BAAHUBALI in Bangalore + Delhi
    // --------------------------------------------------------
    void createMovies() {
        Movie* avengers = new Movie();
        avengers->setMovieId(1);
        avengers->setMovieName("AVENGERS");
        avengers->setMovieDuration(128);

        Movie* baahubali = new Movie();
        baahubali->setMovieId(2);
        baahubali->setMovieName("BAAHUBALI");
        baahubali->setMovieDuration(180);

        movieController->addMovie(avengers, City::Bangalore);
        movieController->addMovie(avengers, City::Delhi);

        movieController->addMovie(baahubali, City::Bangalore);
        movieController->addMovie(baahubali, City::Delhi);
    }

    // --------------------------------------------------------
    // createSeats()
    // Creates 0..99 seats with categories:
    // 0-39 SILVER, 40-69 GOLD, 70-99 PLATINUM
    // --------------------------------------------------------
    vector<Seat*> createSeats() {
        vector<Seat*> seats;

        for (int i = 0; i < 40; i++) {
            Seat* seat = new Seat();
            seat->setSeatId(i);
            seat->setSeatCategory(SeatCategory::SILVER);
            seats.push_back(seat);
        }

        for (int i = 40; i < 70; i++) {
            Seat* seat = new Seat();
            seat->setSeatId(i);
            seat->setSeatCategory(SeatCategory::GOLD);
            seats.push_back(seat);
        }

        for (int i = 70; i < 100; i++) {
            Seat* seat = new Seat();
            seat->setSeatId(i);
            seat->setSeatCategory(SeatCategory::PLATINUM);
            seats.push_back(seat);
        }

        return seats;
    }

    // --------------------------------------------------------
    // createScreen()
    // Creates 1 screen with screenId=1 and seat list
    // --------------------------------------------------------
    vector<Screen*> createScreen() {
        vector<Screen*> screens;

        Screen* screen1 = new Screen();
        screen1->setScreenId(1);
        screen1->setSeats(createSeats());

        screens.push_back(screen1);
        return screens;
    }

    // --------------------------------------------------------
    // createShows(showId, screen, movie, startTime)
    // Creates a show object and returns it
    // --------------------------------------------------------
    Show* createShows(int showId, Screen* screen, Movie* movie, int showStartTime) {
        Show* show = new Show();
        show->setShowId(showId);
        show->setScreen(screen);
        show->setMovie(movie);
        show->setShowStartTime(showStartTime);
        return show;
    }

    // --------------------------------------------------------
    // createTheatre()
    // Creates 2 theatres:
    // 1) INOX Bangalore -> 2 shows
    // 2) PVR Delhi      -> 2 shows
    // Then add theatres into TheatreController
    // --------------------------------------------------------
    void createTheatre() {
        Movie* avengerMovie = movieController->getMovieByName("AVENGERS");
        Movie* baahubaliMovie = movieController->getMovieByName("BAAHUBALI");

        // Theatre 1: INOX Bangalore
        Theatre* inoxTheatre = new Theatre();
        inoxTheatre->setTheatreId(1);
        inoxTheatre->setScreen(createScreen());
        inoxTheatre->setCity(City::Bangalore);

        vector<Show*> inoxShows;
        inoxShows.push_back(createShows(1, inoxTheatre->getScreen()[0], avengerMovie, 8));
        inoxShows.push_back(createShows(2, inoxTheatre->getScreen()[0], baahubaliMovie, 16));
        inoxTheatre->setShows(inoxShows);

        // Theatre 2: PVR Delhi
        Theatre* pvrTheatre = new Theatre();
        pvrTheatre->setTheatreId(2);
        pvrTheatre->setScreen(createScreen());
        pvrTheatre->setCity(City::Delhi);

        vector<Show*> pvrShows;
        pvrShows.push_back(createShows(3, pvrTheatre->getScreen()[0], avengerMovie, 13));
        pvrShows.push_back(createShows(4, pvrTheatre->getScreen()[0], baahubaliMovie, 20));
        pvrTheatre->setShows(pvrShows);

        // Add theatres into controller
        theatreController->addTheatre(inoxTheatre, City::Bangalore);
        theatreController->addTheatre(pvrTheatre, City::Delhi);
    }
};

// ============================================================
// STEP 7) MAIN FUNCTION
// ============================================================

int main() {
    BookMyShow* bookMyShow = new BookMyShow();

    // Step 1: setup data
    bookMyShow->initialize();

    // Step 2: booking attempt 1 (should succeed)
    bookMyShow->createBooking(City::Bangalore, "BAAHUBALI");

    // Step 3: booking attempt 2 (same seat again -> fail)
    bookMyShow->createBooking(City::Bangalore, "BAAHUBALI");

    return 0;
}
