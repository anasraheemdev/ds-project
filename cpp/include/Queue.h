#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <sstream>
#include <ctime>

/**
 * Queue Implementation for Appointment Scheduling
 * ================================================
 * 
 * WHY QUEUE (FIFO - First In, First Out)?
 * ---------------------------------------
 * 1. Fairness: Patients who arrive first get served first
 *    This is the fundamental principle of appointment systems
 * 
 * 2. Sequential Processing: Doctors process patients in order
 *    of their arrival - no skipping or prioritization (basic version)
 * 
 * 3. Waiting Room Model: Real hospital waiting rooms work on FIFO
 *    The first patient to register is the first to be called
 * 
 * 4. Predictable Wait Times: Patients can estimate their wait
 *    based on their position in queue
 * 
 * REAL-WORLD ANALOGY:
 * -------------------
 * Think of a line at a ticket counter - the person who arrives 
 * first gets served first, and new people join at the back.
 * 
 * TIME COMPLEXITY:
 * ----------------
 * - Enqueue:      O(1) - add to rear
 * - Dequeue:      O(1) - remove from front
 * - Peek/Front:   O(1) - view front element
 * - Display All:  O(n) - show waiting list
 * 
 * SPACE COMPLEXITY:
 * -----------------
 * - O(n) where n = number of patients in queue
 */

// Appointment class (Refactored from struct)
class Appointment {
public: // Keeping members public or adding getters/setters. Public for easier access inside Queue for now.
    // Changing to Class with private members to follow instructions strictly.
private:
    int patientId;
    std::string patientName;
    std::string appointmentTime;  // Scheduled time
    std::string registrationTime; // When they joined queue
    int position;                 // Position in queue

public:
    Appointment() : patientId(0), position(0) {}

    Appointment(int id, const std::string& name, const std::string& apptTime)
        : patientId(id), patientName(name), appointmentTime(apptTime), position(0) {
        // Record registration time
        time_t now = time(nullptr);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        registrationTime = std::string(buf);
    }
    
    // Getters and Setters
    int getPatientId() const { return patientId; }
    std::string getPatientName() const { return patientName; }
    std::string getAppointmentTime() const { return appointmentTime; }
    std::string getRegistrationTime() const { return registrationTime; }
    int getPosition() const { return position; }
    
    void setPosition(int p) { position = p; }

    std::string toJSON() const {
        std::stringstream ss;
        ss << "{";
        ss << "\"patientId\":" << patientId << ",";
        ss << "\"patientName\":\"" << patientName << "\",";
        ss << "\"appointmentTime\":\"" << appointmentTime << "\",";
        ss << "\"registrationTime\":\"" << registrationTime << "\",";
        ss << "\"position\":" << position;
        ss << "}";
        return ss.str();
    }
    
    // Friend class to allow direct access if needed, OR just use setters.
    // Queue needs to update position.
};

class AppointmentQueue {
private:
    // Inner class for linked queue node
    class QueueNode {
    public:
        Appointment data;
        QueueNode* next;

        QueueNode(const Appointment& appt) : data(appt), next(nullptr) {}
    };

    QueueNode* front;  // Pointer to front of queue (for dequeue)
    QueueNode* rear;   // Pointer to rear of queue (for enqueue)
    int size;          // Number of appointments in queue

public:
    /**
     * Constructor - Initialize empty queue
     */
    AppointmentQueue() : front(nullptr), rear(nullptr), size(0) {}

    /**
     * Destructor - Free all memory
     */
    ~AppointmentQueue() {
        while (front != nullptr) {
            QueueNode* toDelete = front;
            front = front->next;
            delete toDelete;
        }
    }

    /**
     * Enqueue - Add patient to appointment queue (at rear)
     * 
     * @param appointment The appointment to add
     * 
     * Time: O(1) | Space: O(1)
     */
    void enqueue(const Appointment& appointment) {
        QueueNode* newNode = new QueueNode(appointment);
        // data is a copy, need to set position on the stored copy
        newNode->data.setPosition(size + 1);  // Set position

        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    /**
     * Dequeue - Remove and return patient from front (next to be served)
     * 
     * @return The appointment being processed
     * 
     * Time: O(1) | Space: O(1)
     */
    Appointment dequeue() {
        if (isEmpty()) {
            return Appointment(); // Return empty appointment
        }

        QueueNode* toDelete = front;
        Appointment result = front->data;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr; // Queue is now empty
        }

        delete toDelete;
        size--;

        // Update positions for remaining patients
        updatePositions();

        return result;
    }

    /**
     * Peek at front appointment without removing
     * 
     * @return Reference to front appointment
     * 
     * Time: O(1) | Space: O(1)
     */
    Appointment peekFront() const {
        if (isEmpty()) {
            return Appointment();
        }
        return front->data;
    }

    /**
     * Check if queue is empty
     */
    bool isEmpty() const {
        return front == nullptr;
    }

    /**
     * Get current queue size (number of waiting patients)
     */
    int getSize() const {
        return size;
    }

    /**
     * Get all appointments in queue as JSON array (front to rear)
     * 
     * Time: O(n) | Space: O(n)
     */
    std::string getWaitingListJSON() const {
        std::stringstream ss;
        ss << "[";
        QueueNode* current = front;
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
     * Check if patient is already in queue
     * 
     * Time: O(n) | Space: O(1)
     */
    bool isInQueue(int patientId) const {
        QueueNode* current = front;
        while (current != nullptr) {
            if (current->data.getPatientId() == patientId) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    /**
     * Get patient's position in queue
     * 
     * @return Position (1-indexed) or -1 if not found
     * 
     * Time: O(n) | Space: O(1)
     */
    int getPosition(int patientId) const {
        QueueNode* current = front;
        int pos = 1;
        while (current != nullptr) {
            if (current->data.getPatientId() == patientId) {
                return pos;
            }
            current = current->next;
            pos++;
        }
        return -1;
    }

private:
    /**
     * Update positions of all patients in queue
     * Called after dequeue operation
     * 
     * Time: O(n)
     */
    void updatePositions() {
        QueueNode* current = front;
        int pos = 1;
        while (current != nullptr) {
            current->data.setPosition(pos++);
            current = current->next;
        }
    }
};

#endif // QUEUE_H
