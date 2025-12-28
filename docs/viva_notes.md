# Viva Preparation Notes
## Patient Management System - Data Structures Project

---

## 1. Data Structure Justifications

### 1.1 Why Singly Linked List for Patient Records?

**Purpose:** Store all patient records dynamically

**Justification:**
1. **Dynamic Size:** Unlike arrays, linked lists don't require pre-defined size
2. **Efficient Insertion:** O(1) time to add new patient at head
3. **Memory Efficiency:** Only allocates memory for actual patients
4. **No Shifting:** Deletion doesn't require shifting elements

**Trade-off:** Linear search O(n) - compensated by using BST for fast searches

```
Head → [Patient1] → [Patient2] → [Patient3] → NULL
```

---

### 1.2 Why Binary Search Tree (BST) for Patient Search?

**Purpose:** Enable O(log n) average-case search by Patient ID

**Justification:**
1. **Efficient Search:** Binary search eliminates half the search space at each step
2. **Sorted Output:** In-order traversal gives patients sorted by ID
3. **Dynamic Updates:** Can insert/delete without restructuring entire tree

**BST Property:**
- Left subtree: keys < node key
- Right subtree: keys > node key

```
        [1005]
       /      \
    [1002]   [1008]
    /    \       \
[1001] [1003]  [1010]
```

**In-order traversal:** 1001 → 1002 → 1003 → 1005 → 1008 → 1010 (sorted!)

---

### 1.3 Why Queue (FIFO) for Appointments?

**Purpose:** Manage patient appointments fairly

**Justification:**
1. **Fairness:** First patient to arrive gets served first
2. **Real-world modeling:** Mirrors actual hospital waiting rooms
3. **Predictability:** Patients can estimate wait time based on position

**FIFO Principle:**
```
ENQUEUE →  [P3] [P2] [P1]  → DEQUEUE
           Rear       Front
```

---

### 1.4 Why Stack (LIFO) for Activity History?

**Purpose:** Track recent patient activities

**Justification:**
1. **Recency:** Most recent action is most relevant
2. **Undo Support:** Natural support for "undo last action"
3. **Audit Trail:** Like browser history - latest first

**LIFO Principle:**
```
PUSH →  [View Patient]     ← TOP (Most Recent)
        [Add Patient]
        [Delete Patient]   ← BOTTOM (Oldest)
```

---

## 2. Time Complexity Analysis

| Operation | Data Structure | Time Complexity | Explanation |
|-----------|---------------|-----------------|-------------|
| Add Patient | Linked List | O(1) | Insert at head |
| Delete Patient | Linked List | O(n) | Must find patient first |
| Search by ID | BST | O(log n) avg | Binary search |
| Search by ID | BST | O(n) worst | Skewed tree |
| Enqueue | Queue | O(1) | Add at rear pointer |
| Dequeue | Queue | O(1) | Remove from front pointer |
| Push Activity | Stack | O(1) | Add at top |
| Pop Activity | Stack | O(1) | Remove from top |
| Display All | Linked List | O(n) | Must traverse all |
| Sorted Display | BST | O(n) | In-order traversal |

---

## 3. Space Complexity Analysis

| Data Structure | Space Complexity | Explanation |
|----------------|------------------|-------------|
| Linked List | O(n) | n nodes, each with data + next pointer |
| BST | O(n) | n nodes, each with data + left/right pointers |
| Queue | O(k) | k = patients in queue |
| Stack | O(m) | m = activities stored (capped at 100) |
| **Total** | **O(n + k + m)** | Linear in total elements |

---

## 4. System Flow Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                      USER INTERFACE                          │
│   ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│   │ Add Form │  │ Search   │  │  Queue   │  │ Stack    │   │
│   └────┬─────┘  └────┬─────┘  └────┬─────┘  └────┬─────┘   │
└────────┼─────────────┼─────────────┼─────────────┼──────────┘
         │             │             │             │
         ▼             ▼             ▼             ▼
┌─────────────────────────────────────────────────────────────┐
│                    JAVASCRIPT (AJAX)                         │
│                    Fetch API Calls                           │
└────────────────────────┬────────────────────────────────────┘
                         │ HTTP Requests
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                    FLASK SERVER (Python)                     │
│                    REST API Endpoints                        │
└────────────────────────┬────────────────────────────────────┘
                         │ PyBind11 Calls
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                    C++ BACKEND                               │
│  ┌──────────────┐  ┌─────┐  ┌───────┐  ┌───────┐           │
│  │ Linked List  │  │ BST │  │ Queue │  │ Stack │           │
│  │  (Storage)   │  │(Idx)│  │(Appts)│  │(Acts) │           │
│  └──────────────┘  └─────┘  └───────┘  └───────┘           │
└─────────────────────────────────────────────────────────────┘
```

---

## 5. Key Code Explanations

### 5.1 Linked List Node Structure
```cpp
struct PatientNode {
    Patient data;      // Patient information
    PatientNode* next; // Pointer to next node
};
```

### 5.2 BST Search Algorithm
```cpp
Patient* search(int patientId) {
    if (node == nullptr || node->patientId == patientId)
        return node;
    if (patientId < node->patientId)
        return search(node->left, patientId);  // Go left
    return search(node->right, patientId);     // Go right
}
```

### 5.3 Queue Enqueue Operation
```cpp
void enqueue(Appointment appt) {
    QueueNode* newNode = new QueueNode(appt);
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}
```

### 5.4 Stack Push Operation
```cpp
void push(Activity activity) {
    ActivityNode* newNode = new ActivityNode(activity);
    newNode->next = top;
    top = newNode;
}
```

---

## 6. Common Viva Questions

### Q1: Why not use array instead of linked list?
**A:** Arrays have fixed size and require O(n) shifts for insertions/deletions. Linked lists provide dynamic sizing and O(1) insertion.

### Q2: Why is BST search O(log n)?
**A:** At each comparison, we eliminate half the remaining nodes. After log₂(n) comparisons, we find the target.

### Q3: What if BST becomes skewed?
**A:** Worst case becomes O(n). Solutions: use self-balancing trees like AVL or Red-Black trees (beyond scope of this project).

### Q4: Why use both Linked List AND BST?
**A:** Linked List stores patient data; BST stores pointers for fast ID-based search. This is an index structure - common in databases.

### Q5: Why is Queue FIFO important for appointments?
**A:** Ensures fairness - patients are served in order of arrival. Random access would be unfair.

### Q6: Why not use Hash Map for O(1) search?
**A:** Project constraint. Also, BST provides sorted traversal which Hash Map cannot.

---

## 7. Constraints Followed

| Constraint | Status | Implementation |
|------------|--------|----------------|
| No Hash Map | ✅ | Using BST for indexing |
| No unordered_map | ✅ | Custom linked structures |
| No Database | ✅ | In-memory storage |
| Focus on DS concepts | ✅ | All 4 DS implemented |
| Well-commented code | ✅ | Extensive comments |
| Modular design | ✅ | Separate header files |
