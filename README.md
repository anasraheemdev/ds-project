# Patient Management System
## Data Structures Implementation Project

A comprehensive Patient Management System demonstrating classical Data Structures concepts using **C++** backend with **PyBind11** bindings and a web-based frontend.

---

## 📁 Project Structure

```
DS Project/
├── cpp/
│   ├── include/
│   │   ├── Patient.h          # Patient data structure
│   │   ├── LinkedList.h       # Singly Linked List implementation
│   │   ├── Stack.h            # Stack for activity tracking
│   │   ├── Queue.h            # Queue for appointments
│   │   ├── BST.h              # Binary Search Tree for searches
│   │   └── PatientManager.h   # Main system controller
│   └── bindings.cpp           # PyBind11 bindings
├── python/
│   └── server.py              # Flask server (with Python fallback)
├── frontend/
│   ├── index.html             # Main UI
│   ├── style.css              # Styling
│   └── script.js              # Frontend logic
├── docs/
│   └── viva_notes.md          # Viva preparation notes
├── CMakeLists.txt             # CMake build configuration
└── README.md                  # This file
```

---

## 🚀 Quick Start (Windows)

### Prerequisites
- Python 3.8 or higher
- pip (Python package manager)

### Step 1: Install Python Dependencies

```powershell
cd "c:\Users\Anas Raheem\Desktop\DS Project"
pip install flask flask-cors
```

### Step 2: Start the Backend Server

```powershell
cd "c:\Users\Anas Raheem\Desktop\DS Project\python"
python server.py
```

The server will start at `http://localhost:5000`

> **Note:** If PyBind11 module is not compiled, the server automatically uses a Python fallback implementation.

### Step 3: Open the Frontend

Open `frontend/index.html` in your web browser, or run:

```powershell
cd "c:\Users\Anas Raheem\Desktop\DS Project\frontend"
python -m http.server 8080
```

Then visit `http://localhost:8080`

---

## 🔧 Compiling C++ with PyBind11 (Optional)

If you want to use the actual C++ backend:

### Prerequisites
1. **MinGW-w64** or **Visual Studio** with C++ build tools
2. **PyBind11**: `pip install pybind11`
3. **CMake**: Download from cmake.org

### Build Steps (with CMake)

```powershell
cd "c:\Users\Anas Raheem\Desktop\DS Project"
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Build Steps (Manual with g++)

```powershell
cd "c:\Users\Anas Raheem\Desktop\DS Project\cpp"
g++ -O3 -Wall -shared -std=c++17 -fPIC `
    $(python -m pybind11 --includes) `
    bindings.cpp -o ../python/patient_system.pyd `
    -I./include
```

---

## 📊 Data Structures Used

| Data Structure | Purpose | Time Complexity |
|----------------|---------|-----------------|
| **Singly Linked List** | Store patient records | Insert: O(1), Delete/Search: O(n) |
| **Binary Search Tree** | Fast patient ID search | Search: O(log n) avg |
| **Queue (FIFO)** | Appointment scheduling | Enqueue/Dequeue: O(1) |
| **Stack (LIFO)** | Recent activity tracking | Push/Pop: O(1) |

---

## 🌐 API Endpoints

### Patient Operations
- `GET /api/patients` - Get all patients
- `GET /api/patients/sorted` - Get patients sorted by ID (BST in-order)
- `POST /api/patient` - Add new patient
- `GET /api/patient/<id>` - Search patient by ID
- `PUT /api/patient/<id>` - Update patient
- `DELETE /api/patient/<id>` - Delete patient

### Queue Operations
- `GET /api/queue` - Get waiting list
- `POST /api/queue` - Add patient to queue
- `POST /api/queue/process` - Process next appointment

### Activity Operations
- `GET /api/activities` - Get recent activities
- `GET /api/activities/all` - Get all activities

### Statistics
- `GET /api/statistics` - Get system statistics

---

## ✨ Features

### Core Features (Data Structures)
- ✅ Add, Update, Delete patients (Linked List)
- ✅ Fast patient search by ID (BST)
- ✅ Appointment queue management (FIFO Queue)
- ✅ Activity history tracking (LIFO Stack)
- ✅ Sorted patient display (BST in-order traversal)

### New Features (Bonus)
- ✅ **Persistent File Storage** - Data auto-saved to `data/patients.json`
- ✅ **Sample Data Loading** - Load 5 test patients instantly
- ✅ **Data Export** - Download all data as JSON file
- ✅ **Search by Name** - Find patients by partial name match
- ✅ **Input Validation** - Form validation with clear error messages
- ✅ **Gender Statistics** - View patient gender distribution
- ✅ Modern responsive web interface
- ✅ Real-time statistics dashboard

---

## 📝 Academic Notes

For viva preparation and DS justifications, see [docs/viva_notes.md](docs/viva_notes.md)

---

## 🔒 Constraints Followed

- ❌ No Hash Maps / unordered_map
- ❌ No Database
- ❌ No irrelevant STL containers
- ✅ Pure Data Structures implementation
- ✅ Well-commented code
- ✅ Modular design
