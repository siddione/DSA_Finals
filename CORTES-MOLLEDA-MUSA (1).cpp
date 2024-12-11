#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

// ANSI color codes for text styling
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Node structure for Binary Tree
struct Node {
    int seatNumber; // Seat number for booking (1 to 10)
    Node* left;
    Node* right;

    Node(int num) : seatNumber(num), left(nullptr), right(nullptr) {}
};

class SeatTree {
private:
    Node* root;
    int seatCount;

    void preorderTraversal(Node* node) {
        if (node == nullptr) return;
        cout << node->seatNumber << " ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr) return;
        inorderTraversal(node->left);
        cout << node->seatNumber << " ";
        inorderTraversal(node->right);
    }

    void postorderTraversal(Node* node) {
        if (node == nullptr) return;
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        cout << node->seatNumber << " ";
    }

public:
    SeatTree() : root(nullptr), seatCount(0) {}

    void insert(int seatNumber) {
        if (seatCount >= 10) {
            cout << RED << "Cannot book more than 10 seats." << RESET << endl;
            return;
        }

        if (root == nullptr) {
            root = new Node(seatNumber);
            seatCount++;
            return;
        }

        Node* current = root;
        while (true) {
            if (seatNumber < current->seatNumber) {
                if (current->left == nullptr) {
                    current->left = new Node(seatNumber);
                    seatCount++;
                    break;
                }
                current = current->left;
            } else {
                if (current->right == nullptr) {
                    current->right = new Node(seatNumber);
                    seatCount++;
                    break;
                }
                current = current->right;
            }
        }
    }

    void viewPreorder() {
        cout << CYAN << "Pre-order Traversal: " << RESET;
        preorderTraversal(root);
        cout << endl;
    }

    void viewInorder() {
        cout << CYAN << "In-order Traversal: " << RESET;
        inorderTraversal(root);
        cout << endl;
    }

    void viewPostorder() {
        cout << CYAN << "Post-order Traversal: " << RESET;
        postorderTraversal(root);
        cout << endl;
    }
};

// Custom Max-Heap for Priority Requests
class PriorityHeap {
private:
    vector<pair<int, string>> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].first <= heap[parent].first) break;
            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (index < size) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap[left].first > heap[largest].first) {
                largest = left;
            }

            if (right < size && heap[right].first > heap[largest].first) {
                largest = right;
            }

            if (largest == index) break;

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    void addRequest(const string &customerName, int age) {
        heap.emplace_back(age, customerName);
        heapifyUp(heap.size() - 1);
    }

    void processNextRequest() {
        if (heap.empty()) {
            cout << RED << "No pending requests.\n" << RESET;
            return;
        }
        auto next = heap.front();
        pop();
        cout << GREEN << "Processing request for " << next.second << " (Age: " << next.first << ")\n" << RESET;
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    void viewRequests() {
        if (heap.empty()) {
            cout << RED << "No pending requests.\n" << RESET;
            return;
        }

        cout << CYAN << "Pending requests:\n" << RESET;
        for (const auto &req : heap) {
            cout << "  " << req.second << " (Age: " << req.first << ")\n";
        }
    }
};

// Custom Min-Heap for Movies by Showtimes
class MovieHeap {
public:
    struct Movie {
        string name;
        int showtime; // Priority: showtime in minutes from midnight

        Movie(string n, int t) : name(n), showtime(t) {}
    };

private:
    vector<Movie> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].showtime >= heap[parent].showtime) break;
            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (index < size) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left].showtime < heap[smallest].showtime) {
                smallest = left;
            }

            if (right < size && heap[right].showtime < heap[smallest].showtime) {
                smallest = right;
            }

            if (smallest == index) break;

            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    void push(const string &name, int showtime) {
        heap.emplace_back(name, showtime);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    bool empty() const {
        return heap.empty();
    }

    Movie top() const {
        if (heap.empty()) throw runtime_error("Heap is empty");
        return heap[0];
    }

    void print() {
        for (const auto &movie : heap) {
            int hours = movie.showtime / 60;
            int minutes = movie.showtime % 60;
            string am_pm = (hours >= 12) ? "PM" : "AM";
            if (hours > 12) hours -= 12;
            if (hours == 0) hours = 12;

            cout << YELLOW << "  " << movie.name << " - " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << am_pm << RESET << "\n";
        }
    }

    vector<Movie> getMovies() const {
        return heap; // Return all movies in the heap
    }
};

int main() {
    MovieHeap movieHeap;
    map<string, int> movies; // Map to store movies and their showtimes

    SeatTree seatTree;

    // Add movies to heap with showtimes
    movieHeap.push("Hello, Love, Again", 13 * 60); // 1:00 PM
    movieHeap.push("Moana 2", 14 * 60 + 30);       // 2:30 PM
    movieHeap.push("The Wicked", 17 * 60);         // 5:00 PM

    PriorityHeap priorityHeap;
    int choice;

    do {
        cout << CYAN << "=============================================\n";
        cout << "       Welcome to SM Cinema Legazpi!\n";
        cout << "        Your entertainment destination!\n";
        cout << "=============================================\n";

        cout << "--- Main Menu ---\n";
        cout << "1. View Movies by Showtimes\n";
        cout << "2. Choose Movie\n";
        cout << "3. View Booked Seats\n";
        cout << "4. Book Seat\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Add Priority Request\n";
        cout << "7. Process Next Priority Request\n";
        cout << "8. View Priority Requests\n";
        cout << "0. Exit\n" << RESET;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "\nMovies by Showtimes:\n";
            movieHeap.print();
            break;

        case 2: {
                if (movieHeap.empty()) {
                    cout << RED << "No movies available to choose from.\n" << RESET;
                    break;
                }

                cout << "\nAvailable Movies:\n";
                movieHeap.print();

                cout << "Enter the movie name: ";
                string chosenMovie;
                cin.ignore(); // Clear the input buffer
                getline(cin, chosenMovie);

                bool found = false;
                vector<MovieHeap::Movie> movies = movieHeap.getMovies();  // Get all movies
                for (const auto &movie : movies) {  // Iterate over the movies
                    if (movie.name == chosenMovie) {
                        cout << GREEN << "You selected: " << movie.name << " at ";
                        int hours = movie.showtime / 60;
                        int minutes = movie.showtime % 60;
                        string am_pm = (hours >= 12) ? "PM" : "AM";
                        if (hours > 12) hours -= 12;
                        if (hours == 0) hours = 12;

                        cout << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << am_pm << "\n" << RESET;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << RED << "Movie not found. Please enter a valid movie name.\n" << RESET;
                }
                break;
            }

        case 3:
            seatTree.viewPreorder();
            seatTree.viewInorder();
            seatTree.viewPostorder();
            break;

        case 4: {
            int seat;
            cout << "Enter seat number to book (1-10): ";
            cin >> seat;
            seatTree.insert(seat);
            break;
        }

        case 6: {
            string customerName;
            int age;
            cout << "Enter customer name: ";
            cin.ignore();
            getline(cin, customerName);
            cout << "Enter customer age: ";
            cin >> age;
            priorityHeap.addRequest(customerName, age);
            break;
        }

        case 7:
            priorityHeap.processNextRequest();
            break;

        case 8:
            priorityHeap.viewRequests();
            break;

        case 0:
            cout << CYAN << "Thank you for visiting SM Cinema. Goodbye!" << RESET << endl;
            break;

        default:
            cout << RED << "Invalid choice. Please try again.\n" << RESET;
        }

    } while (choice != 0);

    return 0;
}

/*In the provided C++ code, the design employs heaps instead of queues for managing priority requests and movie showtimes.
Specifically, a **Max-Heap** is used in the `PriorityHeap` class to handle priority requests, where the highest priority
(based on age) is processed first. This approach replaces a traditional queue structure, allowing for efficient handling
of priority requests, particularly for senior citizens who are given higher priority for seat reservations. Similarly,
the `MovieHeap` class uses a **Min-Heap** to manage movie showtimes, ensuring that movies are displayed in order of their
showtime, with the earliest showtime being prioritized. This use of heaps rather than queues offers an optimized way of managing
priority-based operations and improves efficiency, as heaps maintain their ordered structure with each insertion and removal.
Furthermore, in the `SeatTree` class, a binary tree is used for seat management, where each node represents a seat, and insertion is
done based on seat number. The heap-based design offers an alternative to queues, ensuring that priority requests and movie scheduling
are handled in an efficient manner.*/
