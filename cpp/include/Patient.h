#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <sstream>

/**
 * Patient Structure
 * -----------------
 * Represents a single patient record with all required medical information.
 * 
 * Why a struct?
 * - Simple data container with public access
 * - Easy to serialize to JSON for frontend communication
 * - Efficient memory layout for linked list storage
 * 
 * Fields:
 * - patientId: Unique identifier (used as BST key)
 * - name: Patient's full name
 * - age: Patient's age in years
 * - gender: Patient's gender
 * - disease: Primary diagnosis/condition
 * - contactNumber: Phone number for communication
 * - appointmentDate: Scheduled appointment date
 * - visitNotes: Doctor's notes from visits
 */
struct Patient {
    int patientId;
    std::string name;
    int age;
    std::string gender;
    std::string disease;
    std::string contactNumber;
    std::string appointmentDate;
    std::string visitNotes;

    // Default constructor
    Patient() : patientId(0), age(0) {}

    // Parameterized constructor
    Patient(int id, const std::string& n, int a, const std::string& g,
            const std::string& d, const std::string& c, 
            const std::string& appt, const std::string& notes)
        : patientId(id), name(n), age(a), gender(g),
          disease(d), contactNumber(c), appointmentDate(appt), visitNotes(notes) {}

    /**
     * Convert patient to JSON string format
     * Used for communication with Python/Frontend
     * 
     * Time Complexity: O(1) - fixed number of fields
     * Space Complexity: O(1) - output string size is bounded
     */
    std::string toJSON() const {
        std::stringstream ss;
        ss << "{";
        ss << "\"patientId\":" << patientId << ",";
        ss << "\"name\":\"" << escapeJSON(name) << "\",";
        ss << "\"age\":" << age << ",";
        ss << "\"gender\":\"" << escapeJSON(gender) << "\",";
        ss << "\"disease\":\"" << escapeJSON(disease) << "\",";
        ss << "\"contactNumber\":\"" << escapeJSON(contactNumber) << "\",";
        ss << "\"appointmentDate\":\"" << escapeJSON(appointmentDate) << "\",";
        ss << "\"visitNotes\":\"" << escapeJSON(visitNotes) << "\"";
        ss << "}";
        return ss.str();
    }

private:
    // Helper function to escape special characters in JSON strings
    static std::string escapeJSON(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c;
            }
        }
        return result;
    }
};

#endif // PATIENT_H
