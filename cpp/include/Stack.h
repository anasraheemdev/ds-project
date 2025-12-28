#ifndef STACK_H
#define STACK_H

#include <string>
#include <sstream>
#include <ctime>

/**
 * Stack Implementation for Recent Patient Activities
 * ===================================================
 * 
 * WHY STACK (LIFO - Last In, First Out)?
 * --------------------------------------
 * 1. Recent Activity Tracking: The most recent action should be 
 *    displayed/accessed first - perfect LIFO behavior
 * 
 * 2. Undo Functionality: If we add undo feature, stack naturally 
 *    supports "undo last action" pattern
 * 
 * 3. Activity History: Like browser back button - most recent first
 * 
 * 4. Audit Trail: System administrators want to see what just happened
 * 
 * REAL-WORLD ANALOGY:
 * -------------------
 * Think of a stack of papers on a desk - you always add new papers 
 * on top and take from the top. The last paper placed is the first 
 * one you'll pick up.
 * 
 * TIME COMPLEXITY:
 * ----------------
 * - Push:    O(1) - add to top
 * - Pop:     O(1) - remove from top
 * - Peek:    O(1) - view top element
 * - Display: O(n) - show all activities
 * 
 * SPACE COMPLEXITY:
 * -----------------
 * - O(n) where n = number of activities stored
 */

// Activity record structure
struct Activity {
    std::string action;      // Action type: "ADD", "DELETE", "UPDATE", "VIEW", "QUEUE"
    int patientId;           // Related patient ID
    std::string patientName; // Patient name for display
    std::string timestamp;   // When the action occurred
    std::string details;     // Additional details

    Activity() : patientId(0) {}

    Activity(const std::string& act, int id, const std::string& name, 
             const std::string& det = "")
        : action(act), patientId(id), patientName(name), details(det) {
        // Generate timestamp
        time_t now = time(nullptr);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        timestamp = std::string(buf);
    }

    std::string toJSON() const {
        std::stringstream ss;
        ss << "{";
        ss << "\"action\":\"" << action << "\",";
        ss << "\"patientId\":" << patientId << ",";
        ss << "\"patientName\":\"" << patientName << "\",";
        ss << "\"timestamp\":\"" << timestamp << "\",";
        ss << "\"details\":\"" << details << "\"";
        ss << "}";
        return ss.str();
    }
};

// Node for linked stack implementation
struct ActivityNode {
    Activity data;
    ActivityNode* next;

    ActivityNode(const Activity& act) : data(act), next(nullptr) {}
};

class ActivityStack {
private:
    ActivityNode* top;  // Pointer to top of stack
    int size;           // Number of activities
    int maxSize;        // Maximum activities to keep (to prevent memory issues)

public:
    /**
     * Constructor
     * @param max Maximum number of activities to store (default 100)
     */
    ActivityStack(int max = 100) : top(nullptr), size(0), maxSize(max) {}

    /**
     * Destructor - Free all memory
     */
    ~ActivityStack() {
        while (top != nullptr) {
            ActivityNode* toDelete = top;
            top = top->next;
            delete toDelete;
        }
    }

    /**
     * Push new activity onto stack
     * If stack is full, removes oldest activity
     * 
     * @param activity The activity to record
     * 
     * Time: O(1) | Space: O(1)
     */
    void push(const Activity& activity) {
        ActivityNode* newNode = new ActivityNode(activity);
        newNode->next = top;
        top = newNode;
        size++;

        // If exceeds max size, trim from bottom
        if (size > maxSize) {
            trimOldest();
        }
    }

    /**
     * Pop (remove) top activity from stack
     * 
     * @return The removed activity
     * 
     * Time: O(1) | Space: O(1)
     */
    Activity pop() {
        if (isEmpty()) {
            return Activity(); // Return empty activity
        }

        ActivityNode* toDelete = top;
        Activity result = top->data;
        top = top->next;
        delete toDelete;
        size--;
        return result;
    }

    /**
     * Peek at top activity without removing
     * 
     * @return Reference to top activity
     * 
     * Time: O(1) | Space: O(1)
     */
    Activity peek() const {
        if (isEmpty()) {
            return Activity();
        }
        return top->data;
    }

    /**
     * Check if stack is empty
     */
    bool isEmpty() const {
        return top == nullptr;
    }

    /**
     * Get current stack size
     */
    int getSize() const {
        return size;
    }

    /**
     * Get all activities as JSON array (top to bottom)
     * 
     * Time: O(n) | Space: O(n)
     */
    std::string getAllActivitiesJSON() const {
        std::stringstream ss;
        ss << "[";
        ActivityNode* current = top;
        bool first = true;
        while (current != nullptr) {
            if (!first) ss << ",";
            ss << current->data.toJSON();
            first = false;
            current = current->next;
        }
        ss << "]";
        return ss.str();
    }

    /**
     * Get recent N activities as JSON
     * 
     * @param n Number of recent activities to get
     * 
     * Time: O(n) | Space: O(n)
     */
    std::string getRecentActivitiesJSON(int n) const {
        std::stringstream ss;
        ss << "[";
        ActivityNode* current = top;
        int count = 0;
        bool first = true;
        while (current != nullptr && count < n) {
            if (!first) ss << ",";
            ss << current->data.toJSON();
            first = false;
            current = current->next;
            count++;
        }
        ss << "]";
        return ss.str();
    }

private:
    /**
     * Remove oldest activity (at bottom of stack)
     * Called when stack exceeds maxSize
     * 
     * Time: O(n) - must traverse to second-to-last node
     */
    void trimOldest() {
        if (top == nullptr || top->next == nullptr) return;

        ActivityNode* current = top;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
        size--;
    }
};

#endif // STACK_H
