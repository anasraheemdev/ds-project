#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "Patient.h"
#include <string>
#include <sstream>

/**
 * Singly Linked List Implementation for Patient Records
 * ======================================================
 * 
 * WHY LINKED LIST?
 * ----------------
 * 1. Dynamic Size: Unlike arrays, linked lists can grow/shrink at runtime
 *    without needing to know the maximum number of patients in advance.
 * 
 * 2. Efficient Insertion: O(1) time for adding new patient at head
 *    (No need to shift elements like in arrays)
 * 
 * 3. Efficient Deletion: O(1) time once the node is found
 *    (Just update pointers, no shifting required)
 * 
 * 4. No Memory Waste: Memory is allocated only for actual patients
 *    (Arrays might allocate more space than needed)
 * 
 * TIME COMPLEXITY:
 * ----------------
 * - Insert at head: O(1)
 * - Insert at tail: O(n) - must traverse to end
 * - Delete by ID:   O(n) - must search for the ID first
 * - Search by ID:   O(n) - linear search required
 * - Display All:    O(n) - must visit each node
 * 
 * SPACE COMPLEXITY:
 * -----------------
 * - O(n) where n = number of patients
 * - Each node uses extra memory for the 'next' pointer
 */

class PatientLinkedList {
private:
    // Inner class for the linked list node
    class PatientNode {
    public:
        Patient data;       // Patient information
        PatientNode* next;  // Pointer to next node
        
        // Constructor
        PatientNode(const Patient& p) : data(p), next(nullptr) {}
    };

    PatientNode* head;  // Pointer to first node
    int size;           // Track number of patients

public:
    /**
     * Constructor - Initialize empty list
     */
    PatientLinkedList() : head(nullptr), size(0) {}

    /**
     * Destructor - Free all allocated memory
     * Prevents memory leaks
     */
    ~PatientLinkedList() {
        PatientNode* current = head;
        while (current != nullptr) {
            PatientNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    /**
     * Add new patient to the list (at head for O(1) insertion)
     * 
     * @param patient The patient to add
     * @return true if successful
     * 
     * Time: O(1) | Space: O(1)
     */
    bool addPatient(const Patient& patient) {
        PatientNode* newNode = new PatientNode(patient);
        newNode->next = head;
        head = newNode;
        size++;
        return true;
    }

    /**
     * Delete patient by ID
     * 
     * @param patientId The ID of patient to delete
     * @return true if found and deleted, false otherwise
     * 
     * Time: O(n) - must search for the patient
     * Space: O(1)
     */
    bool deletePatient(int patientId) {
        if (head == nullptr) return false;

        // Special case: deleting head
        if (head->data.getPatientId() == patientId) {
            PatientNode* toDelete = head;
            head = head->next;
            delete toDelete;
            size--;
            return true;
        }

        // Search for patient
        PatientNode* current = head;
        while (current->next != nullptr) {
            if (current->next->data.getPatientId() == patientId) {
                PatientNode* toDelete = current->next;
                current->next = current->next->next;
                delete toDelete;
                size--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    /**
     * Update patient information
     * 
     * @param patientId The ID of patient to update
     * @param updatedPatient New patient data (ID remains same)
     * @return true if found and updated, false otherwise
     * 
     * Time: O(n) | Space: O(1)
     */
    bool updatePatient(int patientId, const Patient& updatedPatient) {
        PatientNode* current = head;
        while (current != nullptr) {
            if (current->data.getPatientId() == patientId) {
                // Keep the same ID, update other fields
                current->data.setName(updatedPatient.getName());
                current->data.setAge(updatedPatient.getAge());
                current->data.setGender(updatedPatient.getGender());
                current->data.setDisease(updatedPatient.getDisease());
                current->data.setContactNumber(updatedPatient.getContactNumber());
                current->data.setAppointmentDate(updatedPatient.getAppointmentDate());
                current->data.setVisitNotes(updatedPatient.getVisitNotes());
                return true;
            }
            current = current->next;
        }
        return false;
    }

    /**
     * Get patient by ID
     * 
     * @param patientId The ID to search for
     * @return Pointer to patient if found, nullptr otherwise
     * 
     * Time: O(n) | Space: O(1)
     */
    Patient* getPatientById(int patientId) {
        PatientNode* current = head;
        while (current != nullptr) {
            if (current->data.getPatientId() == patientId) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    /**
     * Get all patients as JSON array string
     * 
     * Time: O(n) | Space: O(n) for output string
     */
    std::string getAllPatientsJSON() const {
        std::stringstream ss;
        ss << "[";
        PatientNode* current = head;
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
     * Get current number of patients
     */
    int getSize() const {
        return size;
    }

    /**
     * Check if list is empty
     */
    bool isEmpty() const {
        return head == nullptr;
    }

    /**
     * Check if patient ID exists
     */
    bool exists(int patientId) const {
        PatientNode* current = head;
        while (current != nullptr) {
            if (current->data.getPatientId() == patientId) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

#endif // LINKED_LIST_H
