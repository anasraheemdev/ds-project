/**
 * PyBind11 Bindings for Patient Management System
 * ================================================
 * 
 * This file creates Python bindings for the C++ Patient Management System.
 * PyBind11 allows C++ code to be called directly from Python.
 * 
 * COMPILATION (Windows with MinGW):
 * g++ -O3 -Wall -shared -std=c++17 -fPIC \
 *     $(python -m pybind11 --includes) \
 *     bindings.cpp -o patient_system$(python-config --extension-suffix) \
 *     -I./include
 * 
 * USAGE IN PYTHON:
 * import patient_system
 * manager = patient_system.PatientManager()
 * result = manager.add_patient("John Doe", 30, "Male", "Flu", "1234567890", "2024-01-15", "Initial visit")
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "include/PatientManager.h"

namespace py = pybind11;

/**
 * Python Module Definition
 * 
 * PYBIND11_MODULE creates a Python module named "patient_system"
 * The 'm' parameter is the module handle used to add functions/classes
 */
PYBIND11_MODULE(patient_system, m) {
    m.doc() = "Patient Management System - Data Structures Implementation";
    
    // Expose PatientManager class to Python
    py::class_<PatientManager>(m, "PatientManager")
        .def(py::init<>())  // Constructor
        
        // ==================== PATIENT OPERATIONS ====================
        .def("add_patient", &PatientManager::addPatient,
             "Add a new patient to the system",
             py::arg("name"),
             py::arg("age"),
             py::arg("gender"),
             py::arg("disease"),
             py::arg("contact_number"),
             py::arg("appointment_date"),
             py::arg("visit_notes"))
        
        .def("add_patient_with_id", &PatientManager::addPatientWithId,
             "Add a patient with specific ID",
             py::arg("patient_id"),
             py::arg("name"),
             py::arg("age"),
             py::arg("gender"),
             py::arg("disease"),
             py::arg("contact_number"),
             py::arg("appointment_date"),
             py::arg("visit_notes"))
        
        .def("search_patient", &PatientManager::searchPatient,
             "Search for a patient by ID (uses BST)",
             py::arg("patient_id"))
        
        .def("delete_patient", &PatientManager::deletePatient,
             "Delete a patient from the system",
             py::arg("patient_id"))
        
        .def("update_patient", &PatientManager::updatePatient,
             "Update patient information",
             py::arg("patient_id"),
             py::arg("name"),
             py::arg("age"),
             py::arg("gender"),
             py::arg("disease"),
             py::arg("contact_number"),
             py::arg("appointment_date"),
             py::arg("visit_notes"))
        
        .def("get_all_patients", &PatientManager::getAllPatients,
             "Get all patients (from linked list)")
        
        .def("get_patients_sorted", &PatientManager::getPatientsSortedById,
             "Get patients sorted by ID (using BST in-order traversal)")
        
        // ==================== QUEUE OPERATIONS ====================
        .def("add_to_queue", &PatientManager::addToQueue,
             "Add patient to appointment queue",
             py::arg("patient_id"))
        
        .def("process_next_appointment", &PatientManager::processNextAppointment,
             "Process next appointment in queue (FIFO)")
        
        .def("get_waiting_list", &PatientManager::getWaitingList,
             "Get all patients in appointment queue")
        
        .def("peek_next_appointment", &PatientManager::peekNextAppointment,
             "Peek at next appointment without processing")
        
        // ==================== STACK OPERATIONS ====================
        .def("get_recent_activities", &PatientManager::getRecentActivities,
             "Get recent activities",
             py::arg("count") = 10)
        
        .def("get_all_activities", &PatientManager::getAllActivities,
             "Get all activities")
        
        .def("pop_last_activity", &PatientManager::popLastActivity,
             "Pop last activity from stack")
        
        // ==================== STATISTICS ====================
        .def("get_statistics", &PatientManager::getStatistics,
             "Get system statistics");
}
