"""
Patient Management System - Python Server
==========================================

This Flask server acts as a bridge between the web frontend and the C++ backend.
It provides REST API endpoints that call the C++ functions via PyBind11.

DATA FLOW:
    Frontend (JS) -> HTTP Request -> Flask Server -> PyBind11 -> C++ Backend
                                                                      |
    Frontend (JS) <- HTTP Response <- Flask Server <- PyBind11 <------+

FALLBACK MODE:
    If the C++ module is not compiled, the server uses a pure Python implementation
    of the data structures for demonstration purposes.

NEW FEATURES:
    - Persistent file storage (JSON)
    - Auto-save on every change
    - Sample data loading
    - Data export
    - Input validation
"""

from flask import Flask, request, jsonify, send_file
from flask_cors import CORS
import json
import os
import sys
from datetime import datetime

# File paths for data persistence
DATA_DIR = os.path.join(os.path.dirname(__file__), 'data')
PATIENTS_FILE = os.path.join(DATA_DIR, 'patients.json')
QUEUE_FILE = os.path.join(DATA_DIR, 'queue.json')

# Ensure data directory exists
os.makedirs(DATA_DIR, exist_ok=True)

# Try to import the C++ module, fall back to Python implementation
try:
    import patient_system
    USING_CPP = True
    print("✓ Using C++ backend via PyBind11")
except ImportError:
    USING_CPP = False
    print("⚠ C++ module not found. Using Python fallback implementation.")
    print("  To compile C++ module, see README.md for instructions.")

app = Flask(__name__)
CORS(app)  # Enable Cross-Origin Resource Sharing for frontend

# ============================================================================
# PYTHON FALLBACK IMPLEMENTATION
# ============================================================================
# These classes mirror the C++ data structures for when PyBind11 is not available

if not USING_CPP:
    
    class Patient:
        """Patient record structure"""
        def __init__(self, patient_id, name, age, gender, disease, 
                     contact_number, appointment_date, visit_notes):
            self.patient_id = patient_id
            self.name = name
            self.age = age
            self.gender = gender
            self.disease = disease
            self.contact_number = contact_number
            self.appointment_date = appointment_date
            self.visit_notes = visit_notes
        
        def to_dict(self):
            return {
                "patientId": self.patient_id,
                "name": self.name,
                "age": self.age,
                "gender": self.gender,
                "disease": self.disease,
                "contactNumber": self.contact_number,
                "appointmentDate": self.appointment_date,
                "visitNotes": self.visit_notes
            }
        
        @staticmethod
        def from_dict(data):
            return Patient(
                data.get('patientId', 0),
                data.get('name', ''),
                data.get('age', 0),
                data.get('gender', ''),
                data.get('disease', ''),
                data.get('contactNumber', ''),
                data.get('appointmentDate', ''),
                data.get('visitNotes', '')
            )
    
    class PatientNode:
        """Linked List Node"""
        def __init__(self, patient):
            self.data = patient
            self.next = None
    
    class PatientLinkedList:
        """
        Singly Linked List for Patient Storage
        Time: Insert O(1), Delete O(n), Search O(n)
        """
        def __init__(self):
            self.head = None
            self.size = 0
        
        def add(self, patient):
            new_node = PatientNode(patient)
            new_node.next = self.head
            self.head = new_node
            self.size += 1
        
        def delete(self, patient_id):
            if self.head is None:
                return False
            if self.head.data.patient_id == patient_id:
                self.head = self.head.next
                self.size -= 1
                return True
            current = self.head
            while current.next:
                if current.next.data.patient_id == patient_id:
                    current.next = current.next.next
                    self.size -= 1
                    return True
                current = current.next
            return False
        
        def get(self, patient_id):
            current = self.head
            while current:
                if current.data.patient_id == patient_id:
                    return current.data
                current = current.next
            return None
        
        def update(self, patient_id, patient):
            current = self.head
            while current:
                if current.data.patient_id == patient_id:
                    current.data = patient
                    return True
                current = current.next
            return False
        
        def get_all(self):
            patients = []
            current = self.head
            while current:
                patients.append(current.data.to_dict())
                current = current.next
            return patients
        
        def exists(self, patient_id):
            return self.get(patient_id) is not None
        
        def clear(self):
            """Clear all patients from the list"""
            self.head = None
            self.size = 0
    
    class BSTNode:
        """Binary Search Tree Node"""
        def __init__(self, patient_id, patient):
            self.patient_id = patient_id
            self.patient = patient
            self.left = None
            self.right = None
    
    class PatientBST:
        """
        Binary Search Tree for Fast Patient ID Search
        Time: Search O(log n) average, O(n) worst
        """
        def __init__(self):
            self.root = None
        
        def insert(self, patient_id, patient):
            if self.root is None:
                self.root = BSTNode(patient_id, patient)
            else:
                self._insert_recursive(self.root, patient_id, patient)
        
        def _insert_recursive(self, node, patient_id, patient):
            if patient_id < node.patient_id:
                if node.left is None:
                    node.left = BSTNode(patient_id, patient)
                else:
                    self._insert_recursive(node.left, patient_id, patient)
            elif patient_id > node.patient_id:
                if node.right is None:
                    node.right = BSTNode(patient_id, patient)
                else:
                    self._insert_recursive(node.right, patient_id, patient)
        
        def search(self, patient_id):
            return self._search_recursive(self.root, patient_id)
        
        def _search_recursive(self, node, patient_id):
            if node is None or node.patient_id == patient_id:
                return node.patient if node else None
            if patient_id < node.patient_id:
                return self._search_recursive(node.left, patient_id)
            return self._search_recursive(node.right, patient_id)
        
        def delete(self, patient_id):
            self.root = self._delete_recursive(self.root, patient_id)
        
        def _delete_recursive(self, node, patient_id):
            if node is None:
                return None
            if patient_id < node.patient_id:
                node.left = self._delete_recursive(node.left, patient_id)
            elif patient_id > node.patient_id:
                node.right = self._delete_recursive(node.right, patient_id)
            else:
                if node.left is None:
                    return node.right
                if node.right is None:
                    return node.left
                min_node = self._find_min(node.right)
                node.patient_id = min_node.patient_id
                node.patient = min_node.patient
                node.right = self._delete_recursive(node.right, min_node.patient_id)
            return node
        
        def _find_min(self, node):
            while node.left:
                node = node.left
            return node
        
        def inorder(self):
            result = []
            self._inorder_recursive(self.root, result)
            return result
        
        def _inorder_recursive(self, node, result):
            if node:
                self._inorder_recursive(node.left, result)
                result.append(node.patient.to_dict())
                self._inorder_recursive(node.right, result)
        
        def clear(self):
            """Clear the BST"""
            self.root = None
    
    class Activity:
        """Activity record for stack"""
        def __init__(self, action, patient_id, patient_name, details=""):
            self.action = action
            self.patient_id = patient_id
            self.patient_name = patient_name
            self.details = details
            self.timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
        def to_dict(self):
            return {
                "action": self.action,
                "patientId": self.patient_id,
                "patientName": self.patient_name,
                "details": self.details,
                "timestamp": self.timestamp
            }
    
    class ActivityStack:
        """
        Stack for Recent Activities (LIFO)
        Time: Push O(1), Pop O(1)
        """
        def __init__(self, max_size=100):
            self.items = []
            self.max_size = max_size
        
        def push(self, activity):
            self.items.append(activity)
            if len(self.items) > self.max_size:
                self.items.pop(0)
        
        def pop(self):
            return self.items.pop() if self.items else None
        
        def peek(self):
            return self.items[-1] if self.items else None
        
        def get_all(self):
            return [a.to_dict() for a in reversed(self.items)]
        
        def get_recent(self, n):
            return [a.to_dict() for a in reversed(self.items[-n:])]
        
        @property
        def size(self):
            return len(self.items)
    
    class Appointment:
        """Appointment for queue"""
        def __init__(self, patient_id, patient_name, appointment_time, position=0):
            self.patient_id = patient_id
            self.patient_name = patient_name
            self.appointment_time = appointment_time
            self.registration_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            self.position = position
        
        def to_dict(self):
            return {
                "patientId": self.patient_id,
                "patientName": self.patient_name,
                "appointmentTime": self.appointment_time,
                "registrationTime": self.registration_time,
                "position": self.position
            }
    
    class AppointmentQueue:
        """
        Queue for Appointment Scheduling (FIFO)
        Time: Enqueue O(1), Dequeue O(1)
        """
        def __init__(self):
            self.items = []
        
        def enqueue(self, appointment):
            appointment.position = len(self.items) + 1
            self.items.append(appointment)
        
        def dequeue(self):
            if not self.items:
                return None
            item = self.items.pop(0)
            self._update_positions()
            return item
        
        def peek(self):
            return self.items[0] if self.items else None
        
        def is_in_queue(self, patient_id):
            return any(a.patient_id == patient_id for a in self.items)
        
        def get_all(self):
            return [a.to_dict() for a in self.items]
        
        def _update_positions(self):
            for i, item in enumerate(self.items):
                item.position = i + 1
        
        def clear(self):
            """Clear the queue"""
            self.items = []
        
        @property
        def size(self):
            return len(self.items)
    
    class PatientManagerPython:
        """Python implementation of PatientManager with File Persistence"""
        def __init__(self):
            self.patient_list = PatientLinkedList()
            self.patient_bst = PatientBST()
            self.activity_stack = ActivityStack()
            self.appointment_queue = AppointmentQueue()
            self.next_id = 1001
            
            # Load saved data on startup
            self._load_from_file()
        
        # ==================== FILE PERSISTENCE ====================
        
        def _save_to_file(self):
            """Save all patient data to JSON file"""
            try:
                data = {
                    "patients": self.patient_list.get_all(),
                    "nextId": self.next_id,
                    "savedAt": datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                }
                with open(PATIENTS_FILE, 'w', encoding='utf-8') as f:
                    json.dump(data, f, indent=2, ensure_ascii=False)
                print(f"  📁 Data saved to {PATIENTS_FILE}")
            except Exception as e:
                print(f"  ⚠ Error saving data: {e}")
        
        def _load_from_file(self):
            """Load patient data from JSON file"""
            if not os.path.exists(PATIENTS_FILE):
                print("  📁 No saved data found. Starting fresh.")
                return
            
            try:
                with open(PATIENTS_FILE, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                
                patients = data.get('patients', [])
                self.next_id = data.get('nextId', 1001)
                
                # Rebuild data structures from file
                for p_data in patients:
                    patient = Patient.from_dict(p_data)
                    self.patient_list.add(patient)
                    self.patient_bst.insert(patient.patient_id, patient)
                
                print(f"  ✓ Loaded {len(patients)} patients from {PATIENTS_FILE}")
                self.activity_stack.push(Activity("LOAD", 0, "System", f"Loaded {len(patients)} patients from file"))
                
            except Exception as e:
                print(f"  ⚠ Error loading data: {e}")
        
        def export_data(self):
            """Export all data as JSON string"""
            return json.dumps({
                "patients": self.patient_list.get_all(),
                "queue": self.appointment_queue.get_all(),
                "statistics": {
                    "totalPatients": self.patient_list.size,
                    "queueSize": self.appointment_queue.size,
                    "nextId": self.next_id
                },
                "exportedAt": datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            }, indent=2, ensure_ascii=False)
        
        def clear_all_data(self):
            """Clear all data and delete saved files"""
            self.patient_list.clear()
            self.patient_bst.clear()
            self.appointment_queue.clear()
            self.next_id = 1001
            
            if os.path.exists(PATIENTS_FILE):
                os.remove(PATIENTS_FILE)
            
            self.activity_stack.push(Activity("CLEAR", 0, "System", "All data cleared"))
            return json.dumps({"success": True, "message": "All data cleared successfully"})
        
        def load_sample_data(self):
            """Load sample patient data for testing"""
            sample_patients = [
                {"name": "Ahmed Khan", "age": 45, "gender": "Male", "disease": "Diabetes", 
                 "contact": "03001234567", "date": "2024-01-15", "notes": "Regular checkup"},
                {"name": "Fatima Ali", "age": 32, "gender": "Female", "disease": "Migraine", 
                 "contact": "03009876543", "date": "2024-01-16", "notes": "Recurring headaches"},
                {"name": "Muhammad Hassan", "age": 28, "gender": "Male", "disease": "Flu", 
                 "contact": "03005551234", "date": "2024-01-17", "notes": "Seasonal flu symptoms"},
                {"name": "Ayesha Siddiqui", "age": 55, "gender": "Female", "disease": "Hypertension", 
                 "contact": "03007778899", "date": "2024-01-18", "notes": "Blood pressure monitoring"},
                {"name": "Omar Farooq", "age": 40, "gender": "Male", "disease": "Back Pain", 
                 "contact": "03003334455", "date": "2024-01-19", "notes": "Lower back pain"},
            ]
            
            added = 0
            for p in sample_patients:
                patient_id = self.next_id
                self.next_id += 1
                patient = Patient(patient_id, p["name"], p["age"], p["gender"], 
                                  p["disease"], p["contact"], p["date"], p["notes"])
                self.patient_list.add(patient)
                self.patient_bst.insert(patient_id, patient)
                added += 1
            
            self._save_to_file()
            self.activity_stack.push(Activity("SAMPLE", 0, "System", f"Loaded {added} sample patients"))
            
            return json.dumps({
                "success": True, 
                "message": f"Loaded {added} sample patients",
                "count": added
            })
        
        # ==================== PATIENT OPERATIONS ====================
        
        def add_patient(self, name, age, gender, disease, contact_number, 
                        appointment_date, visit_notes):
            # Input validation
            if not name or len(name.strip()) < 2:
                return json.dumps({"success": False, "message": "Name is required (min 2 characters)"})
            if age < 0 or age > 150:
                return json.dumps({"success": False, "message": "Age must be between 0 and 150"})
            if not gender:
                return json.dumps({"success": False, "message": "Gender is required"})
            if not disease:
                return json.dumps({"success": False, "message": "Disease/Condition is required"})
            
            patient_id = self.next_id
            self.next_id += 1
            patient = Patient(patient_id, name.strip(), age, gender, disease,
                              contact_number, appointment_date, visit_notes)
            self.patient_list.add(patient)
            self.patient_bst.insert(patient_id, patient)
            self.activity_stack.push(Activity("ADD", patient_id, name, "New patient registered"))
            
            # Auto-save to file
            self._save_to_file()
            
            return json.dumps({
                "success": True,
                "message": "Patient added successfully",
                "patient": patient.to_dict()
            })
        
        def add_patient_with_id(self, patient_id, name, age, gender, disease,
                                 contact_number, appointment_date, visit_notes):
            if self.patient_list.exists(patient_id):
                return json.dumps({"success": False, "message": "Patient ID already exists"})
            patient = Patient(patient_id, name, age, gender, disease,
                              contact_number, appointment_date, visit_notes)
            self.patient_list.add(patient)
            self.patient_bst.insert(patient_id, patient)
            self.activity_stack.push(Activity("ADD", patient_id, name, "Patient registered"))
            if patient_id >= self.next_id:
                self.next_id = patient_id + 1
            
            self._save_to_file()
            
            return json.dumps({
                "success": True,
                "message": "Patient added successfully",
                "patient": patient.to_dict()
            })
        
        def search_patient(self, patient_id):
            patient = self.patient_bst.search(patient_id)
            if patient is None:
                return json.dumps({"success": False, "message": f"Patient with ID {patient_id} not found"})
            self.activity_stack.push(Activity("VIEW", patient_id, patient.name, "Patient record viewed"))
            return json.dumps({"success": True, "patient": patient.to_dict()})
        
        def delete_patient(self, patient_id):
            patient = self.patient_bst.search(patient_id)
            if patient is None:
                return json.dumps({"success": False, "message": f"Patient with ID {patient_id} not found"})
            patient_name = patient.name
            self.patient_bst.delete(patient_id)
            self.patient_list.delete(patient_id)
            self.activity_stack.push(Activity("DELETE", patient_id, patient_name, "Patient record deleted"))
            
            self._save_to_file()
            
            return json.dumps({"success": True, "message": f"Patient {patient_name} deleted successfully"})
        
        def update_patient(self, patient_id, name, age, gender, disease,
                            contact_number, appointment_date, visit_notes):
            if not self.patient_list.exists(patient_id):
                return json.dumps({"success": False, "message": f"Patient with ID {patient_id} not found"})
            
            # Input validation
            if not name or len(name.strip()) < 2:
                return json.dumps({"success": False, "message": "Name is required (min 2 characters)"})
            
            patient = Patient(patient_id, name.strip(), age, gender, disease,
                              contact_number, appointment_date, visit_notes)
            self.patient_list.update(patient_id, patient)
            self.patient_bst.delete(patient_id)
            self.patient_bst.insert(patient_id, patient)
            self.activity_stack.push(Activity("UPDATE", patient_id, name, "Patient record updated"))
            
            self._save_to_file()
            
            return json.dumps({
                "success": True,
                "message": "Patient updated successfully",
                "patient": patient.to_dict()
            })
        
        def get_all_patients(self):
            return json.dumps(self.patient_list.get_all())
        
        def get_patients_sorted(self):
            return json.dumps(self.patient_bst.inorder())
        
        def search_by_name(self, name_query):
            """Search patients by name (partial match)"""
            results = []
            current = self.patient_list.head
            query_lower = name_query.lower()
            while current:
                if query_lower in current.data.name.lower():
                    results.append(current.data.to_dict())
                current = current.next
            return json.dumps({
                "success": True,
                "count": len(results),
                "patients": results
            })
        
        # ==================== QUEUE OPERATIONS ====================
        
        def add_to_queue(self, patient_id):
            patient = self.patient_bst.search(patient_id)
            if patient is None:
                return json.dumps({"success": False, "message": f"Patient with ID {patient_id} not found"})
            if self.appointment_queue.is_in_queue(patient_id):
                return json.dumps({"success": False, "message": f"{patient.name} is already in the queue"})
            appt = Appointment(patient_id, patient.name, patient.appointment_date)
            self.appointment_queue.enqueue(appt)
            self.activity_stack.push(Activity("QUEUE", patient_id, patient.name, "Added to appointment queue"))
            return json.dumps({
                "success": True,
                "message": f"{patient.name} added to queue",
                "position": self.appointment_queue.size,
                "queueSize": self.appointment_queue.size
            })
        
        def process_next_appointment(self):
            if self.appointment_queue.size == 0:
                return json.dumps({"success": False, "message": "No patients in queue"})
            appt = self.appointment_queue.dequeue()
            self.activity_stack.push(Activity("PROCESS", appt.patient_id, appt.patient_name, "Appointment processed"))
            return json.dumps({
                "success": True,
                "message": f"Processing appointment for {appt.patient_name}",
                "appointment": appt.to_dict(),
                "remainingInQueue": self.appointment_queue.size
            })
        
        def get_waiting_list(self):
            return json.dumps({
                "success": True,
                "queueSize": self.appointment_queue.size,
                "waitingList": self.appointment_queue.get_all()
            })
        
        def peek_next_appointment(self):
            if self.appointment_queue.size == 0:
                return json.dumps({"success": False, "message": "No patients in queue"})
            appt = self.appointment_queue.peek()
            return json.dumps({"success": True, "nextAppointment": appt.to_dict()})
        
        # ==================== STACK OPERATIONS ====================
        
        def get_recent_activities(self, count=10):
            return json.dumps({
                "success": True,
                "count": self.activity_stack.size,
                "activities": self.activity_stack.get_recent(count)
            })
        
        def get_all_activities(self):
            return json.dumps({
                "success": True,
                "count": self.activity_stack.size,
                "activities": self.activity_stack.get_all()
            })
        
        def pop_last_activity(self):
            if self.activity_stack.size == 0:
                return json.dumps({"success": False, "message": "No activities to undo"})
            act = self.activity_stack.pop()
            return json.dumps({"success": True, "poppedActivity": act.to_dict()})
        
        # ==================== STATISTICS ====================
        
        def get_statistics(self):
            # Count by gender
            male_count = female_count = other_count = 0
            current = self.patient_list.head
            while current:
                g = current.data.gender.lower()
                if g == 'male':
                    male_count += 1
                elif g == 'female':
                    female_count += 1
                else:
                    other_count += 1
                current = current.next
            
            return json.dumps({
                "totalPatients": self.patient_list.size,
                "queueSize": self.appointment_queue.size,
                "activityCount": self.activity_stack.size,
                "bstHeight": 0,
                "nextPatientId": self.next_id,
                "genderDistribution": {
                    "male": male_count,
                    "female": female_count,
                    "other": other_count
                },
                "dataFile": PATIENTS_FILE if os.path.exists(PATIENTS_FILE) else None
            })

# ============================================================================
# INITIALIZE PATIENT MANAGER
# ============================================================================

if USING_CPP:
    manager = patient_system.PatientManager()
else:
    manager = PatientManagerPython()

# ============================================================================
# API ROUTES
# ============================================================================

@app.route('/')
def index():
    """API information endpoint"""
    return jsonify({
        "name": "Patient Management System API",
        "version": "2.0.0",
        "backend": "C++ with PyBind11" if USING_CPP else "Python (Fallback)",
        "features": ["File Storage", "Sample Data", "Export", "Search by Name"],
        "endpoints": {
            "patients": "/api/patients",
            "patient": "/api/patient/<id>",
            "queue": "/api/queue",
            "activities": "/api/activities",
            "statistics": "/api/statistics",
            "export": "/api/export",
            "sample": "/api/sample-data",
            "clear": "/api/clear"
        }
    })

# ==================== PATIENT ENDPOINTS ====================

@app.route('/api/patients', methods=['GET'])
def get_patients():
    """Get all patients"""
    result = manager.get_all_patients()
    return jsonify(json.loads(result) if isinstance(result, str) else result)

@app.route('/api/patients/sorted', methods=['GET'])
def get_patients_sorted():
    """Get patients sorted by ID (uses BST in-order traversal)"""
    result = manager.get_patients_sorted()
    return jsonify(json.loads(result) if isinstance(result, str) else result)

@app.route('/api/patients/search', methods=['GET'])
def search_patients_by_name():
    """Search patients by name"""
    name = request.args.get('name', '')
    if not name:
        return jsonify({"success": False, "message": "Name query is required"})
    result = manager.search_by_name(name)
    return jsonify(json.loads(result))

@app.route('/api/patient', methods=['POST'])
def add_patient():
    """Add a new patient"""
    data = request.json
    if not data:
        return jsonify({"success": False, "message": "Request body is required"})
    
    result = manager.add_patient(
        data.get('name', ''),
        int(data.get('age', 0)),
        data.get('gender', ''),
        data.get('disease', ''),
        data.get('contactNumber', ''),
        data.get('appointmentDate', ''),
        data.get('visitNotes', '')
    )
    return jsonify(json.loads(result))

@app.route('/api/patient/<int:patient_id>', methods=['GET'])
def get_patient(patient_id):
    """Search for a patient by ID"""
    result = manager.search_patient(patient_id)
    return jsonify(json.loads(result))

@app.route('/api/patient/<int:patient_id>', methods=['PUT'])
def update_patient(patient_id):
    """Update patient information"""
    data = request.json
    if not data:
        return jsonify({"success": False, "message": "Request body is required"})
    
    result = manager.update_patient(
        patient_id,
        data.get('name', ''),
        int(data.get('age', 0)),
        data.get('gender', ''),
        data.get('disease', ''),
        data.get('contactNumber', ''),
        data.get('appointmentDate', ''),
        data.get('visitNotes', '')
    )
    return jsonify(json.loads(result))

@app.route('/api/patient/<int:patient_id>', methods=['DELETE'])
def delete_patient(patient_id):
    """Delete a patient"""
    result = manager.delete_patient(patient_id)
    return jsonify(json.loads(result))

# ==================== QUEUE ENDPOINTS ====================

@app.route('/api/queue', methods=['GET'])
def get_queue():
    """Get appointment queue (waiting list)"""
    result = manager.get_waiting_list()
    return jsonify(json.loads(result))

@app.route('/api/queue', methods=['POST'])
def add_to_queue():
    """Add patient to appointment queue"""
    data = request.json
    patient_id = int(data.get('patientId', 0))
    result = manager.add_to_queue(patient_id)
    return jsonify(json.loads(result))

@app.route('/api/queue/next', methods=['GET'])
def peek_next():
    """Peek at next appointment"""
    result = manager.peek_next_appointment()
    return jsonify(json.loads(result))

@app.route('/api/queue/process', methods=['POST'])
def process_next():
    """Process next appointment (dequeue)"""
    result = manager.process_next_appointment()
    return jsonify(json.loads(result))

# ==================== ACTIVITY ENDPOINTS ====================

@app.route('/api/activities', methods=['GET'])
def get_activities():
    """Get recent activities"""
    count = request.args.get('count', 10, type=int)
    result = manager.get_recent_activities(count)
    return jsonify(json.loads(result))

@app.route('/api/activities/all', methods=['GET'])
def get_all_activities():
    """Get all activities"""
    result = manager.get_all_activities()
    return jsonify(json.loads(result))

@app.route('/api/activities/pop', methods=['POST'])
def pop_activity():
    """Pop last activity from stack"""
    result = manager.pop_last_activity()
    return jsonify(json.loads(result))

# ==================== STATISTICS ENDPOINT ====================

@app.route('/api/statistics', methods=['GET'])
def get_statistics():
    """Get system statistics"""
    result = manager.get_statistics()
    return jsonify(json.loads(result))

# ==================== NEW FEATURE ENDPOINTS ====================

@app.route('/api/sample-data', methods=['POST'])
def load_sample_data():
    """Load sample patient data for testing"""
    result = manager.load_sample_data()
    return jsonify(json.loads(result))

@app.route('/api/export', methods=['GET'])
def export_data():
    """Export all data as JSON"""
    data = manager.export_data()
    return app.response_class(
        response=data,
        status=200,
        mimetype='application/json',
        headers={'Content-Disposition': 'attachment; filename=patients_export.json'}
    )

@app.route('/api/clear', methods=['POST'])
def clear_all_data():
    """Clear all data (use with caution!)"""
    result = manager.clear_all_data()
    return jsonify(json.loads(result))

# ============================================================================
# MAIN
# ============================================================================

if __name__ == '__main__':
    print("\n" + "="*60)
    print("    PATIENT MANAGEMENT SYSTEM - Backend Server v2.0")
    print("="*60)
    print(f"\n  Backend: {'C++ with PyBind11' if USING_CPP else 'Python (Fallback)'}")
    print("  Server:  http://localhost:5000")
    print("  API:     http://localhost:5000/api")
    print(f"  Data:    {DATA_DIR}")
    print("\n  NEW FEATURES:")
    print("  ✓ Persistent file storage (auto-save)")
    print("  ✓ Sample data loading")
    print("  ✓ Data export")
    print("  ✓ Search by name")
    print("  ✓ Input validation")
    print("  ✓ Gender statistics")
    print("\n" + "="*60 + "\n")
    
    app.run(debug=True, port=5000)
