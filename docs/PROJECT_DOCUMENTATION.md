# Patient Management System
## Data Structures & Algorithms Project Documentation

**Submitted By:** Anas Raheem  
**Date:** December 28, 2025  
**GitHub Repository:** https://github.com/anasraheemdev/ds-project

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Technologies Used](#2-technologies-used)
3. [Data Structures Implemented](#3-data-structures-implemented)
4. [System Architecture](#4-system-architecture)
5. [Project Structure](#5-project-structure)
6. [Data Structures Justification](#6-data-structures-justification)
7. [Time & Space Complexity Analysis](#7-time--space-complexity-analysis)
8. [API Documentation](#8-api-documentation)
9. [User Interface](#9-user-interface)
10. [How to Run](#10-how-to-run)
11. [Features Implemented](#11-features-implemented)
12. [Code Samples](#12-code-samples)
13. [Testing & Verification](#13-testing--verification)
14. [Conclusion](#14-conclusion)

---

## 1. Project Overview

### 1.1 Problem Statement

Design and implement a **Patient Management System** that demonstrates the practical application of classical Data Structures concepts. The system must manage patient records, appointment scheduling, and activity tracking using only fundamental data structures without relying on hash maps or database systems.

### 1.2 Objectives

1. Implement core data structures (Linked List, Stack, Queue, BST) from scratch
2. Create a functional patient management system with CRUD operations
3. Demonstrate the integration of C++ backend with web frontend via PyBind11
4. Provide persistent data storage using file handling
5. Build a user-friendly web interface

### 1.3 Constraints

- ❌ No Hash Maps or unordered_map
- ❌ No Database systems
- ❌ No irrelevant STL containers
- ✅ Focus strictly on Data Structures concepts
- ✅ Well-commented and modular code

---

## 2. Technologies Used

| Layer | Technology | Purpose |
|-------|------------|---------|
| Backend (Core) | C++ 17 | Data structure implementations |
| Backend (Binding) | PyBind11 | Bridge between C++ and Python |
| Backend (Server) | Python Flask | REST API server |
| Frontend | HTML5, CSS3, JavaScript | User interface |
| Storage | JSON Files | Persistent data storage |

### 2.1 Why This Architecture?

```
┌──────────────────────────────────────────────────────────────────┐
│                        USER INTERFACE                             │
│                    (HTML + CSS + JavaScript)                      │
└─────────────────────────────┬────────────────────────────────────┘
                              │ HTTP Requests (JSON)
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│                      FLASK REST API                               │
│                    (Python Backend)                               │
└─────────────────────────────┬────────────────────────────────────┘
                              │ PyBind11 Calls
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│                    C++ DATA STRUCTURES                            │
│     ┌──────────┐  ┌─────┐  ┌───────┐  ┌───────┐                 │
│     │ Linked   │  │ BST │  │ Queue │  │ Stack │                 │
│     │  List    │  │     │  │       │  │       │                 │
│     └──────────┘  └─────┘  └───────┘  └───────┘                 │
└──────────────────────────────────────────────────────────────────┘
```

---

## 3. Data Structures Implemented

### 3.1 Singly Linked List

**Purpose:** Store all patient records dynamically

**File:** `cpp/include/LinkedList.h`

```cpp
struct PatientNode {
    Patient data;       // Patient information
    PatientNode* next;  // Pointer to next node
};

class PatientLinkedList {
private:
    PatientNode* head;  // Pointer to first node
    int size;           // Track number of patients
    
public:
    bool addPatient(const Patient& patient);    // O(1)
    bool deletePatient(int patientId);          // O(n)
    bool updatePatient(int patientId, ...);     // O(n)
    Patient* getPatientById(int patientId);     // O(n)
    std::string getAllPatientsJSON();           // O(n)
};
```

**Visual Representation:**
```
Head → [Patient 1001] → [Patient 1002] → [Patient 1003] → NULL
        ↓                 ↓                 ↓
     {Ahmed, 45}       {Fatima, 32}      {Omar, 28}
```

---

### 3.2 Binary Search Tree (BST)

**Purpose:** Enable O(log n) average-case search by Patient ID

**File:** `cpp/include/BST.h`

```cpp
struct BSTNode {
    int patientId;      // Key for BST ordering
    Patient* patient;   // Pointer to patient data
    BSTNode* left;      // Left child (smaller IDs)
    BSTNode* right;     // Right child (larger IDs)
};

class PatientBST {
private:
    BSTNode* root;
    
public:
    bool insert(int patientId, Patient* patient);  // O(log n)
    Patient* search(int patientId);                // O(log n)
    bool remove(int patientId);                    // O(log n)
    std::string inorderTraversalJSON();            // O(n)
};
```

**Visual Representation:**
```
            [1005]
           /      \
       [1002]    [1008]
       /    \        \
   [1001] [1003]   [1010]

In-order: 1001 → 1002 → 1003 → 1005 → 1008 → 1010 (Sorted!)
```

---

### 3.3 Queue (FIFO)

**Purpose:** Manage patient appointment scheduling

**File:** `cpp/include/Queue.h`

```cpp
struct QueueNode {
    Appointment data;
    QueueNode* next;
};

class AppointmentQueue {
private:
    QueueNode* front;  // For dequeue (remove)
    QueueNode* rear;   // For enqueue (add)
    int size;
    
public:
    void enqueue(const Appointment& appt);  // O(1)
    Appointment dequeue();                   // O(1)
    Appointment peekFront();                 // O(1)
    std::string getWaitingListJSON();        // O(n)
};
```

**Visual Representation:**
```
ENQUEUE →  [Patient 3] → [Patient 2] → [Patient 1]  → DEQUEUE
           (Last In)                    (First Out)
              ↑                              ↑
            Rear                           Front
```

---

### 3.4 Stack (LIFO)

**Purpose:** Track recent patient activities

**File:** `cpp/include/Stack.h`

```cpp
struct ActivityNode {
    Activity data;
    ActivityNode* next;
};

class ActivityStack {
private:
    ActivityNode* top;
    int size;
    int maxSize;  // Limit to prevent memory issues
    
public:
    void push(const Activity& activity);    // O(1)
    Activity pop();                          // O(1)
    Activity peek();                         // O(1)
    std::string getAllActivitiesJSON();      // O(n)
};
```

**Visual Representation:**
```
      ┌─────────────────┐ ← TOP (Most Recent)
      │ VIEW Patient    │
      ├─────────────────┤
      │ ADD Patient     │
      ├─────────────────┤
      │ DELETE Patient  │
      └─────────────────┘ ← BOTTOM (Oldest)
```

---

## 4. System Architecture

### 4.1 Data Flow Diagram

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   Browser   │────▶│  JavaScript │────▶│ Flask API   │────▶│ C++ Backend │
│   (User)    │     │  (Fetch)    │     │ (Python)    │     │ (DS Logic)  │
└─────────────┘     └─────────────┘     └─────────────┘     └─────────────┘
      ▲                                                            │
      │                                                            │
      └──────────────────── JSON Response ◀────────────────────────┘
```

### 4.2 Component Interaction

1. **User** interacts with HTML forms
2. **JavaScript** captures form data and makes API calls
3. **Flask Server** receives requests and routes to appropriate handlers
4. **PyBind11** bridges Python calls to C++ functions (or uses Python fallback)
5. **C++ Data Structures** perform the actual operations
6. **JSON Response** is sent back to update the UI

---

## 5. Project Structure

```
DS Project/
│
├── cpp/                          # C++ Backend
│   ├── include/
│   │   ├── Patient.h             # Patient data structure
│   │   ├── LinkedList.h          # Linked List implementation
│   │   ├── Stack.h               # Stack implementation
│   │   ├── Queue.h               # Queue implementation
│   │   ├── BST.h                 # BST implementation
│   │   └── PatientManager.h      # Main controller
│   └── bindings.cpp              # PyBind11 bindings
│
├── python/                       # Python Backend
│   ├── server.py                 # Flask REST API
│   └── data/
│       └── patients.json         # Persistent storage
│
├── frontend/                     # Web UI
│   ├── index.html                # Main page
│   ├── style.css                 # Styling
│   └── script.js                 # Frontend logic
│
├── docs/
│   └── viva_notes.md             # Viva preparation
│
├── CMakeLists.txt                # Build configuration
└── README.md                     # Quick start guide
```

---

## 6. Data Structures Justification

### 6.1 Why Linked List for Patient Storage?

| Aspect | Linked List | Array |
|--------|-------------|-------|
| Size | Dynamic | Fixed |
| Insertion | O(1) at head | O(n) shift required |
| Memory | Only allocated as needed | May waste space |
| Implementation | More complex | Simpler |

**Decision:** Linked List is preferred because:
- Number of patients is unknown at compile time
- Frequent insertions/deletions expected
- Memory efficiency for sparse data

---

### 6.2 Why BST for Patient Search?

| Aspect | BST | Linear Search |
|--------|-----|---------------|
| Search Time | O(log n) average | O(n) |
| Sorted Output | In-order traversal | Requires sorting |
| Dynamic Updates | Efficient | N/A |

**Decision:** BST provides:
- Faster search for large datasets
- Natural sorting via in-order traversal
- Efficient insertion/deletion

---

### 6.3 Why Queue for Appointments?

**FIFO (First In, First Out) Principle:**
- Patients who arrive first get served first
- Ensures fairness in appointment scheduling
- Mirrors real-world hospital waiting room behavior

---

### 6.4 Why Stack for Activities?

**LIFO (Last In, First Out) Principle:**
- Most recent action is most relevant
- Natural support for "undo" functionality
- Like browser history - latest action first

---

## 7. Time & Space Complexity Analysis

### 7.1 Time Complexity

| Operation | Data Structure | Average Case | Worst Case |
|-----------|---------------|--------------|------------|
| Add Patient | Linked List | O(1) | O(1) |
| Delete Patient | Linked List | O(n) | O(n) |
| Search by ID | BST | O(log n) | O(n)* |
| Enqueue | Queue | O(1) | O(1) |
| Dequeue | Queue | O(1) | O(1) |
| Push Activity | Stack | O(1) | O(1) |
| Pop Activity | Stack | O(1) | O(1) |
| Display All | All | O(n) | O(n) |

*BST worst case occurs with skewed tree (all nodes on one side)

### 7.2 Space Complexity

| Data Structure | Space | Explanation |
|----------------|-------|-------------|
| Linked List | O(n) | n nodes + pointers |
| BST | O(n) | n nodes + left/right pointers |
| Queue | O(k) | k = current appointments |
| Stack | O(m) | m = activities (capped at 100) |
| **Total** | **O(n + k + m)** | Linear in total elements |

---

## 8. API Documentation

### 8.1 Patient Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/patients` | Get all patients |
| GET | `/api/patients/sorted` | Get patients sorted by ID |
| GET | `/api/patients/search?name=xyz` | Search by name |
| POST | `/api/patient` | Add new patient |
| GET | `/api/patient/<id>` | Get patient by ID |
| PUT | `/api/patient/<id>` | Update patient |
| DELETE | `/api/patient/<id>` | Delete patient |

### 8.2 Queue Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/queue` | Get waiting list |
| POST | `/api/queue` | Add to queue |
| POST | `/api/queue/process` | Process next appointment |
| GET | `/api/queue/next` | Peek next appointment |

### 8.3 Other Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/activities` | Get recent activities |
| GET | `/api/statistics` | Get system statistics |
| POST | `/api/sample-data` | Load sample patients |
| GET | `/api/export` | Export all data as JSON |
| POST | `/api/clear` | Clear all data |

---

## 9. User Interface

### 9.1 Main Dashboard

The UI consists of three main panels:

1. **Left Panel:** Forms for adding patients, searching, and quick actions
2. **Center Panel:** Patient records table with action buttons
3. **Right Panel:** Appointment queue and activity history

### 9.2 Key UI Features

- **Real-time Statistics:** Header shows patient count, queue size, activity count
- **Toast Notifications:** Success/error messages for all operations
- **Responsive Design:** Works on desktop and mobile devices
- **Modern Styling:** Clean, professional appearance

---

## 10. How to Run

### 10.1 Prerequisites

- Python 3.8 or higher
- pip (Python package manager)

### 10.2 Installation Steps

```powershell
# Step 1: Navigate to project directory
cd "c:\Users\Anas Raheem\Desktop\DS Project"

# Step 2: Install Python dependencies
pip install flask flask-cors

# Step 3: Start the backend server
cd python
python server.py

# Step 4: Open the frontend
# Simply open frontend/index.html in a web browser
```

### 10.3 Verify Installation

1. Server should show: `Running on http://127.0.0.1:5000`
2. Open `frontend/index.html` in browser
3. Click "Load Sample Data" to test

---

## 11. Features Implemented

### 11.1 Core Features (Required)

| Feature | Status | Data Structure Used |
|---------|--------|---------------------|
| Add Patient | ✅ | Linked List + BST |
| Delete Patient | ✅ | Linked List + BST |
| Update Patient | ✅ | Linked List |
| Search by ID | ✅ | BST (O(log n)) |
| Display All Patients | ✅ | Linked List |
| Sorted Display | ✅ | BST In-order |
| Add to Queue | ✅ | Queue |
| Process Appointment | ✅ | Queue |
| Track Activities | ✅ | Stack |

### 11.2 Bonus Features (Optional)

| Feature | Status | Description |
|---------|--------|-------------|
| File Storage | ✅ | Auto-save to JSON |
| Sample Data | ✅ | Load 5 test patients |
| Data Export | ✅ | Download as JSON |
| Search by Name | ✅ | Partial match search |
| Input Validation | ✅ | Form validation |
| Gender Statistics | ✅ | Distribution display |

---

## 12. Code Samples

### 12.1 Adding a Patient (Linked List Insert)

```cpp
bool PatientLinkedList::addPatient(const Patient& patient) {
    // Create new node
    PatientNode* newNode = new PatientNode(patient);
    
    // Insert at head (O(1) operation)
    newNode->next = head;
    head = newNode;
    size++;
    
    return true;
}
```

### 12.2 BST Search Algorithm

```cpp
Patient* PatientBST::search(int patientId) {
    return searchRecursive(root, patientId);
}

BSTNode* searchRecursive(BSTNode* node, int patientId) {
    // Base cases
    if (node == nullptr || node->patientId == patientId)
        return node;
    
    // Binary search: go left or right
    if (patientId < node->patientId)
        return searchRecursive(node->left, patientId);
    else
        return searchRecursive(node->right, patientId);
}
```

### 12.3 Queue Enqueue Operation

```cpp
void AppointmentQueue::enqueue(const Appointment& appointment) {
    QueueNode* newNode = new QueueNode(appointment);
    
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;  // Add at rear
        rear = newNode;
    }
    size++;
}
```

### 12.4 Stack Push Operation

```cpp
void ActivityStack::push(const Activity& activity) {
    ActivityNode* newNode = new ActivityNode(activity);
    
    newNode->next = top;  // Point to previous top
    top = newNode;        // New node becomes top
    size++;
    
    // Trim if exceeds max size
    if (size > maxSize) {
        trimOldest();
    }
}
```

---

## 13. Testing & Verification

### 13.1 Test Cases Executed

| Test Case | Expected Result | Actual Result | Status |
|-----------|-----------------|---------------|--------|
| Add patient | Patient added with auto-ID | Patient added (ID: 1001) | ✅ Pass |
| Search existing ID | Patient found | Patient data returned | ✅ Pass |
| Search non-existing ID | Error message | "Patient not found" | ✅ Pass |
| Delete patient | Patient removed | List updated | ✅ Pass |
| Add to queue | Patient queued | Position assigned | ✅ Pass |
| Process queue | FIFO order | First patient processed | ✅ Pass |
| Activity tracking | Recent actions shown | Stack updated correctly | ✅ Pass |
| Data persistence | Data saved/loaded | JSON file created | ✅ Pass |

### 13.2 Edge Cases Tested

- Empty list operations
- Duplicate ID prevention
- Invalid input validation
- Queue with single patient
- BST with skewed insertions

---

## 14. Conclusion

### 14.1 Learning Outcomes

1. **Practical DS Implementation:** Gained hands-on experience implementing Linked List, Stack, Queue, and BST from scratch
2. **System Integration:** Learned to integrate C++ with Python and web technologies
3. **API Design:** Understood RESTful API design principles
4. **Full-Stack Development:** Built a complete application from backend to frontend

### 14.2 Future Improvements

1. Implement AVL/Red-Black Tree for balanced BST
2. Add priority queue for emergency patients
3. Implement user authentication
4. Add database support for production use
5. Mobile app development

### 14.3 Summary

This project successfully demonstrates the practical application of fundamental Data Structures in a real-world scenario. By implementing a Patient Management System, we showed how:

- **Linked Lists** efficiently store dynamic data
- **BST** enables fast searching
- **Queues** ensure fair appointment scheduling
- **Stacks** track system activities

The project meets all specified requirements and includes additional bonus features for enhanced functionality.

---

## Appendix

### A. GitHub Repository
https://github.com/anasraheemdev/ds-project

### B. References
1. Data Structures and Algorithms - Course Material
2. PyBind11 Documentation - https://pybind11.readthedocs.io/
3. Flask Documentation - https://flask.palletsprojects.com/

---

*Document prepared for academic submission*  
*December 2025*
