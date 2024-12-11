#include <iostream>
#include <queue>
#include <string>
#include <map>
using namespace std;

// Struct for Movie with priority (showtime in minutes from midnight)
struct Movie {
    string name;
    int showtime; // Priority: showtime in minutes from midnight

    Movie(string n, int t) : name(n), showtime(t) {}

    // Overload operator to create a Min-Heap (earliest time comes first)
    bool operator<(const Movie& other) const {
        return showtime > other.showtime;
    }
};

// Binary Search Tree Node for Seats
struct SeatNode {
    int seatNumber;
    bool isBooked;
    SeatNode* left;
    SeatNode* right;

    SeatNode(int seat) : seatNumber(seat), isBooked(false), left(nullptr), right(nullptr) {}
};

// Binary Search Tree Class for Managing Seats
class SeatBST {
private:
    SeatNode* root;

    SeatNode* insert(SeatNode* node, int seatNumber) {
        if (!node) return new SeatNode(seatNumber);
        if (seatNumber < node->seatNumber)
            node->left = insert(node->left, seatNumber);
        else if (seatNumber > node->seatNumber)
            node->right = insert(node->right, seatNumber);
        return node;
    }

    void inorder(SeatNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << "  Seat " << node->seatNumber << (node->isBooked ? " (Booked)" : " (Available)") << endl;
        inorder(node->right);
    }

    SeatNode* search(SeatNode* node, int seatNumber) {
        if (!node || node->seatNumber == seatNumber) return node;
        if (seatNumber < node->seatNumber)
            return search(node->left, seatNumber);
        return search(node->right, seatNumber);
    }

    SeatNode* deleteNode(SeatNode* node, int seatNumber) {
        // If the node is empty, return it
        if (!node) return node;

        // Traverse the tree to find the seat to delete
        if (seatNumber < node->seatNumber)
            node->left = deleteNode(node->left, seatNumber);
        else if (seatNumber > node->seatNumber)
            node->right = deleteNode(node->right, seatNumber);
        else { // Seat found
            // Node with only one child or no child
            if (!node->left) {
                SeatNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                SeatNode* temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            SeatNode* temp = findMin(node->right);
            node->seatNumber = temp->seatNumber; // Copy the inorder successor's content to this node
            node->right = deleteNode(node->right, temp->seatNumber); // Delete the inorder successor
        }
        return node;
    }

    SeatNode* findMin(SeatNode* node) {
        SeatNode* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

public:
    SeatBST() : root(nullptr) {}

    void addSeat(int seatNumber) {
        root = insert(root, seatNumber);
    }

    void viewSeats() {
        if (!root) {
            cout << "No seats available.\n";
            return;
        }
        inorder(root);
    }

    bool bookSeat(int seatNumber) {
        SeatNode* seat = search(root, seatNumber);
        if (!seat) {
            cout << "Seat " << seatNumber << " does not exist.\n";
            return false;
        }
        if (seat->isBooked) {
            cout << "Seat " << seatNumber << " is already booked.\n";
            return false;
        }
        seat->isBooked = true;
        cout << "Seat " << seatNumber << " successfully booked.\n";
        return true;
    }

    bool deleteSeat(int seatNumber) {
        SeatNode* seat = search(root, seatNumber);
        if (!seat) {
            cout << "Seat " << seatNumber << " does not exist.\n";
            return false;
        }
        root = deleteNode(root, seatNumber);
        cout << "Seat " << seatNumber << " successfully deleted.\n";
        return true;
    }

    bool cancelBooking(int seatNumber) {
        SeatNode* seat = search(root, seatNumber);
        if (!seat) {
            cout << "Seat " << seatNumber << " does not exist.\n";
            return false;
        }
        if (!seat->isBooked) {
            cout << "Seat " << seatNumber << " is not booked.\n";
            return false;
        }
        seat->isBooked = false; // Reset the booking status
        root = deleteNode(root, seatNumber); // Delete the seat node
        cout << "Booking for Seat " << seatNumber << " successfully canceled and seat deleted.\n";
        return true;
    }
};

// Max-Heap for Priority Customers based on age
class PriorityQueue {
private:
    priority_queue<pair<int, string>> heap;  // Max-Heap by default using the first element (age)

public:
    void addRequest(string customerName, int age) {
        heap.push({age, customerName});
    }

    void processNextRequest() {
        if (heap.empty()) {
            cout << "No pending requests.\n";
            return;
        }
        auto next = heap.top();
        heap.pop();
        cout << "Processing request for " << next.second << " (Age: " << next.first << ")\n";
    }

    void viewRequests() {
        if (heap.empty()) {
            cout << "No pending requests.\n";
            return;
        }
        cout << "Pending requests:\n";
        auto tempHeap = heap; // Copy heap to view contents
        while (!tempHeap.empty()) {
            auto req = tempHeap.top();
            cout << "  " << req.second << " (Age: " << req.first << ")\n";
            tempHeap.pop();
        }
    }
};

int main() {
    // Min-Heap for movies based on showtime
    priority_queue<Movie> movieHeap;
    map<string, SeatBST> movies;

    // Add movies to heap with showtimes
    movieHeap.push(Movie("Hello, Love, Again", 13 * 60)); // 1:00 PM
    movieHeap.push(Movie("Moana 2", 14 * 60 + 30));       // 2:30 PM
    movieHeap.push(Movie("The Wicked", 17 * 60));         // 5:00 PM

    // Add seats for each movie in the map
    for (int i = 1; i <= 10; i++) {
        movies["Hello, Love, Again"].addSeat(i);
        movies["Moana 2"].addSeat(i);
        movies["The Wicked"].addSeat(i);
    }

    string currentMovie;
    int choice;

    cout << "=============================================\n";
    cout << "       Welcome to SM Cinema Legazpi!        \n";
    cout << "        Your entertainment destination!      \n";
    cout << "=============================================\n";

    PriorityQueue priorityQueue;

    do {
        cout << "\n--- Main Menu ---\n";
        if (currentMovie.empty()) {
            cout << "No movie selected.\n";
        } else {
            cout << "Current Movie: " << currentMovie << "\n";
        }
        cout << "1. View Movies by Showtimes\n";
        cout << "2. Choose Movie\n";
        cout << "3. View Seats\n";
        cout << "4. Book Seat\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Add Priority Request\n";
        cout << "7. Process Next Priority Request\n";
        cout << "8. View Priority Requests\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "\nMovies by Showtimes:\n";
            auto tempHeap = movieHeap; // Copy heap to view contents
            while (!tempHeap.empty()) {
                auto movie = tempHeap.top();
                tempHeap.pop();
                int hours = movie.showtime / 60;
                int minutes = movie.showtime % 60;

                string am_pm = (hours >= 12) ? "PM" : "AM";
                if (hours > 12) hours -= 12; // Convert to 12-hour format
                if (hours == 0) hours = 12;  // Midnight case (12 AM)

                cout << "  " << movie.name << " - " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << am_pm << "\n";
            }
            break;
        }
        case 2: {
            cout << "\nEnter the name of the movie you want to select: ";
            cin.ignore();
            getline(cin, currentMovie);

            if (movies.find(currentMovie) == movies.end()) {
                cout << "Invalid movie selection. Please try again.\n";
                currentMovie.clear();
            } else {
                cout << "You selected: " << currentMovie << endl;

                auto tempHeap = movieHeap; // Copy heap to view contents
                while (!tempHeap.empty()) {
                    auto movie = tempHeap.top();
                    tempHeap.pop();
                    if (movie.name == currentMovie) {
                        int hours = movie.showtime / 60;
                        int minutes = movie.showtime % 60;

                        string am_pm = (hours >= 12) ? "PM" : "AM";
                        if (hours > 12) hours -= 12;
                        if (hours == 0) hours = 12;

                        cout << "Thank you for choosing SM Cinema. Your chosen movie, '" << currentMovie
                             << "', will start at " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << am_pm << ".\n";
                        break;
                    }
                }
            }
            break;
        }

        case 3:
            if (currentMovie.empty()) {
                cout << "Please select a movie first.\n";
            } else {
                cout << "\nAvailable Seats for " << currentMovie << ":\n";
                movies[currentMovie].viewSeats();
            }
            break;

        case 4: {
            if (currentMovie.empty()) {
                cout << "Please select a movie first.\n";
            } else {
                int seatNumber;
                cout << "Enter seat number to book: ";
                cin >> seatNumber;
                movies[currentMovie].bookSeat(seatNumber);
            }
            break;
        }

        case 5: {
            if (currentMovie.empty()) {
                cout << "Please select a movie first.\n";
            } else {
                int seatNumber;
                cout << "Enter seat number to cancel booking: ";
                cin >> seatNumber;
                movies[currentMovie].cancelBooking(seatNumber);  // Now cancels and deletes the seat
            }
            break;
        }

        case 6: {
            string customerName;
            int age;

            cout << "Enter customer name: ";
            cin >> customerName;
            cout << "Enter customer age: ";
            cin >> age;

            priorityQueue.addRequest(customerName, age);
            cout << "Request added for " << customerName << " (Age: " << age << ").\n";
            break;
        }
        case 7:
            priorityQueue.processNextRequest();
            break;
        case 8:
            priorityQueue.viewRequests();
            break;
        case 0:
            cout << "Thank you for visiting SM Cinema Legazpi. Have a great day!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
