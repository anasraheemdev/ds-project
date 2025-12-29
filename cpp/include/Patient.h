#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <sstream>

/**
 * Patient Class
 * -------------
 * Represents a single patient record with all required medical information.
 * 
 * Why a Class (OOP)?
 * - Encapsulation: Data fields are private, accessed only via public methods
 * - Data Protection: Prevents invalid modifications to internal state
 * - Abstraction: Hides internal implementation details
 * - Interface: Provides a clear contract (methods) for interacting with the object
 */
class Patient {
private:
    int patientId;
    std::string name;
    int age;
    std::string gender;
    std::string disease;
    std::string contactNumber;
    std::string appointmentDate;
    std::string visitNotes;

public:
    // Default constructor
    Patient() : patientId(0), age(0) {}

    // Parameterized constructor
    Patient(int id, const std::string& n, int a, const std::string& g,
            const std::string& d, const std::string& c, 
            const std::string& appt, const std::string& notes)
        : patientId(id), name(n), age(a), gender(g),
          disease(d), contactNumber(c), appointmentDate(appt), visitNotes(notes) {}

    // Getters and Setters
    int getPatientId() const { return patientId; }
    void setPatientId(int id) { patientId = id; }

    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }

    int getAge() const { return age; }
    void setAge(int a) { age = a; }

    std::string getGender() const { return gender; }
    void setGender(const std::string& g) { gender = g; }

    std::string getDisease() const { return disease; }
    void setDisease(const std::string& d) { disease = d; }

    std::string getContactNumber() const { return contactNumber; }
    void setContactNumber(const std::string& c) { contactNumber = c; }

    std::string getAppointmentDate() const { return appointmentDate; }
    void setAppointmentDate(const std::string& appt) { appointmentDate = appt; }

    std::string getVisitNotes() const { return visitNotes; }
    void setVisitNotes(const std::string& notes) { visitNotes = notes; }

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
    // Kept private as it is a utility for internal use only
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
