#ifndef PATIENT_MANAGER_H
#define PATIENT_MANAGER_H

#include "Patient.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "BST.h"
#include <string>
#include <sstream>

/**
 * Patient Manager - Main System Controller
 * =========================================
 * 
 * This class integrates all data structures and provides a unified 
 * interface for the patient management system.
 * 
 * DATA STRUCTURE COORDINATION:
 * ----------------------------
 * 1. LinkedList: Primary storage for patient records
 * 2. BST: Secondary index for fast search by ID
 * 3. Queue: Appointment scheduling (FIFO)
 * 4. Stack: Activity tracking (LIFO)
 * 
 * When a patient is added:
 * - Patient is stored in LinkedList
 * - Patient ID is indexed in BST (pointing to LinkedList data)
 * - Activity is logged to Stack
 * 
 * This demonstrates how multiple data structures can work together
 * in a real-world application, each serving its optimal purpose.
 */
class PatientManager {
private:
    PatientLinkedList patientList;  // Primary patient storage
    PatientBST patientIndex;        // BST index for fast search
    AppointmentQueue appointmentQueue;  // Appointment scheduling
    ActivityStack activityStack;    // Recent activity tracking
    int nextId;                     // Auto-increment ID counter

public:
    /**
     * Constructor - Initialize system
     */
    PatientManager() : nextId(1001) {}

    // ==================== PATIENT OPERATIONS ====================

    /**
     * Add new patient to the system
     * 
     * @return JSON response with success status and patient data
     */
    std::string addPatient(const std::string& name, int age, const std::string& gender,
                           const std::string& disease, const std::string& contactNumber,
                           const std::string& appointmentDate, const std::string& visitNotes) {
        // Create patient with auto-generated ID
        int patientId = nextId++;
        Patient patient(patientId, name, age, gender, disease, 
                        contactNumber, appointmentDate, visitNotes);
        
        // Add to linked list
        patientList.addPatient(patient);
        
        // Get pointer to stored patient and add to BST
        Patient* storedPatient = patientList.getPatientById(patientId);
        patientIndex.insert(patientId, storedPatient);
        
        // Log activity
        Activity activity("ADD", patientId, name, "New patient registered");
        activityStack.push(activity);
        
        // Return success response
        std::stringstream ss;
        ss << "{\"success\":true,\"message\":\"Patient added successfully\",";
        ss << "\"patient\":" << patient.toJSON() << "}";
        return ss.str();
    }

    /**
     * Add patient with specific ID (for data loading)
     */
    std::string addPatientWithId(int patientId, const std::string& name, int age, 
                                  const std::string& gender, const std::string& disease, 
                                  const std::string& contactNumber,
                                  const std::string& appointmentDate, 
                                  const std::string& visitNotes) {
        // Check if ID already exists
        if (patientList.exists(patientId)) {
            return "{\"success\":false,\"message\":\"Patient ID already exists\"}";
        }
        
        Patient patient(patientId, name, age, gender, disease, 
                        contactNumber, appointmentDate, visitNotes);
        
        patientList.addPatient(patient);
        Patient* storedPatient = patientList.getPatientById(patientId);
        patientIndex.insert(patientId, storedPatient);
        
        Activity activity("ADD", patientId, name, "Patient registered");
        activityStack.push(activity);
        
        // Update nextId if needed
        if (patientId >= nextId) {
            nextId = patientId + 1;
        }
        
        std::stringstream ss;
        ss << "{\"success\":true,\"message\":\"Patient added successfully\",";
        ss << "\"patient\":" << patient.toJSON() << "}";
        return ss.str();
    }

    /**
     * Search patient by ID using BST (fast search)
     * 
     * @return JSON response with patient data or error
     */
    std::string searchPatient(int patientId) {
        // Use BST for O(log n) search
        Patient* patient = patientIndex.search(patientId);
        
        if (patient == nullptr) {
            return "{\"success\":false,\"message\":\"Patient not found\"}";
        }
        
        // Log activity
        Activity activity("VIEW", patientId, patient->name, "Patient record viewed");
        activityStack.push(activity);
        
        std::stringstream ss;
        ss << "{\"success\":true,\"patient\":" << patient->toJSON() << "}";
        return ss.str();
    }

    /**
     * Delete patient from system
     * 
     * @return JSON response with success status
     */
    std::string deletePatient(int patientId) {
        // First check if patient exists
        Patient* patient = patientIndex.search(patientId);
        
        if (patient == nullptr) {
            return "{\"success\":false,\"message\":\"Patient not found\"}";
        }
        
        std::string patientName = patient->name;
        
        // Remove from BST first
        patientIndex.remove(patientId);
        
        // Remove from linked list
        patientList.deletePatient(patientId);
        
        // Log activity
        Activity activity("DELETE", patientId, patientName, "Patient record deleted");
        activityStack.push(activity);
        
        return "{\"success\":true,\"message\":\"Patient deleted successfully\"}";
    }

    /**
     * Update patient information
     * 
     * @return JSON response with updated patient data
     */
    std::string updatePatient(int patientId, const std::string& name, int age, 
                               const std::string& gender, const std::string& disease, 
                               const std::string& contactNumber,
                               const std::string& appointmentDate, 
                               const std::string& visitNotes) {
        // Check if patient exists
        Patient* existingPatient = patientIndex.search(patientId);
        
        if (existingPatient == nullptr) {
            return "{\"success\":false,\"message\":\"Patient not found\"}";
        }
        
        // Create updated patient data
        Patient updatedPatient(patientId, name, age, gender, disease, 
                               contactNumber, appointmentDate, visitNotes);
        
        // Update in linked list
        patientList.updatePatient(patientId, updatedPatient);
        
        // Get updated pointer (same memory location, values updated)
        Patient* updated = patientList.getPatientById(patientId);
        patientIndex.updatePatientPointer(patientId, updated);
        
        // Log activity
        Activity activity("UPDATE", patientId, name, "Patient record updated");
        activityStack.push(activity);
        
        std::stringstream ss;
        ss << "{\"success\":true,\"message\":\"Patient updated successfully\",";
        ss << "\"patient\":" << updated->toJSON() << "}";
        return ss.str();
    }

    /**
     * Get all patients (from linked list)
     */
    std::string getAllPatients() {
        return patientList.getAllPatientsJSON();
    }

    /**
     * Get all patients sorted by ID (using BST in-order traversal)
     */
    std::string getPatientsSortedById() {
        return patientIndex.getSortedPatientsJSON();
    }

    // ==================== QUEUE OPERATIONS ====================

    /**
     * Add patient to appointment queue
     */
    std::string addToQueue(int patientId) {
        // Check if patient exists
        Patient* patient = patientIndex.search(patientId);
        
        if (patient == nullptr) {
            return "{\"success\":false,\"message\":\"Patient not found\"}";
        }
        
        // Check if already in queue
        if (appointmentQueue.isInQueue(patientId)) {
            return "{\"success\":false,\"message\":\"Patient already in queue\"}";
        }
        
        // Add to queue
        Appointment appt(patientId, patient->name, patient->appointmentDate);
        appointmentQueue.enqueue(appt);
        
        // Log activity
        Activity activity("QUEUE", patientId, patient->name, "Added to appointment queue");
        activityStack.push(activity);
        
        std::stringstream ss;
        ss << "{\"success\":true,\"message\":\"Patient added to queue\",";
        ss << "\"position\":" << appointmentQueue.getSize() << ",";
        ss << "\"queueSize\":" << appointmentQueue.getSize() << "}";
        return ss.str();
    }

    /**
     * Process next appointment (dequeue)
     */
    std::string processNextAppointment() {
        if (appointmentQueue.isEmpty()) {
            return "{\"success\":false,\"message\":\"No appointments in queue\"}";
        }
        
        Appointment appt = appointmentQueue.dequeue();
        
        // Log activity
        Activity activity("PROCESS", appt.patientId, appt.patientName, 
                          "Appointment processed");
        activityStack.push(activity);
        
        std::stringstream ss;
        ss << "{\"success\":true,\"message\":\"Appointment processed\",";
        ss << "\"appointment\":" << appt.toJSON() << ",";
        ss << "\"remainingInQueue\":" << appointmentQueue.getSize() << "}";
        return ss.str();
    }

    /**
     * Get waiting list
     */
    std::string getWaitingList() {
        std::stringstream ss;
        ss << "{\"success\":true,\"queueSize\":" << appointmentQueue.getSize() << ",";
        ss << "\"waitingList\":" << appointmentQueue.getWaitingListJSON() << "}";
        return ss.str();
    }

    /**
     * Peek at next appointment without processing
     */
    std::string peekNextAppointment() {
        if (appointmentQueue.isEmpty()) {
            return "{\"success\":false,\"message\":\"No appointments in queue\"}";
        }
        
        Appointment appt = appointmentQueue.peekFront();
        std::stringstream ss;
        ss << "{\"success\":true,\"nextAppointment\":" << appt.toJSON() << "}";
        return ss.str();
    }

    // ==================== STACK OPERATIONS ====================

    /**
     * Get recent activities
     */
    std::string getRecentActivities(int count = 10) {
        std::stringstream ss;
        ss << "{\"success\":true,\"count\":" << activityStack.getSize() << ",";
        ss << "\"activities\":" << activityStack.getRecentActivitiesJSON(count) << "}";
        return ss.str();
    }

    /**
     * Get all activities
     */
    std::string getAllActivities() {
        std::stringstream ss;
        ss << "{\"success\":true,\"count\":" << activityStack.getSize() << ",";
        ss << "\"activities\":" << activityStack.getAllActivitiesJSON() << "}";
        return ss.str();
    }

    /**
     * Pop last activity (for undo feature)
     */
    std::string popLastActivity() {
        if (activityStack.isEmpty()) {
            return "{\"success\":false,\"message\":\"No activities to undo\"}";
        }
        
        Activity act = activityStack.pop();
        std::stringstream ss;
        ss << "{\"success\":true,\"poppedActivity\":" << act.toJSON() << "}";
        return ss.str();
    }

    // ==================== STATISTICS ====================

    /**
     * Get system statistics
     */
    std::string getStatistics() {
        std::stringstream ss;
        ss << "{";
        ss << "\"totalPatients\":" << patientList.getSize() << ",";
        ss << "\"queueSize\":" << appointmentQueue.getSize() << ",";
        ss << "\"activityCount\":" << activityStack.getSize() << ",";
        ss << "\"bstHeight\":" << patientIndex.getHeight() << ",";
        ss << "\"nextPatientId\":" << nextId;
        ss << "}";
        return ss.str();
    }
};

#endif // PATIENT_MANAGER_H
