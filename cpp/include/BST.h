#ifndef BST_H
#define BST_H

#include "Patient.h"
#include <string>
#include <sstream>

/**
 * Binary Search Tree Implementation for Patient ID Search
 * ========================================================
 * 
 * WHY BST?
 * --------
 * 1. Efficient Search: O(log n) average case search time
 *    Much faster than linear search in linked list for large datasets
 * 
 * 2. Sorted Data: In-order traversal gives patients sorted by ID
 *    Useful for generating sorted reports
 * 
 * 3. Dynamic: Can insert and delete without restructuring entire data
 *    Unlike sorted arrays which require shifting elements
 * 
 * 4. Memory Efficient: Only stores what's needed, no pre-allocation
 * 
 * BST PROPERTY:
 * -------------
 * For every node:
 * - Left subtree contains only nodes with keys LESS than node's key
 * - Right subtree contains only nodes with keys GREATER than node's key
 * 
 * This property enables binary search - at each step, we eliminate 
 * half of the remaining search space.
 * 
 * REAL-WORLD ANALOGY:
 * -------------------
 * Think of a dictionary - you don't search page by page. You open 
 * roughly in the middle, check if your word comes before or after, 
 * then repeat in the appropriate half.
 * 
 * TIME COMPLEXITY:
 * ----------------
 * - Insert:              O(log n) average, O(n) worst (skewed tree)
 * - Search:              O(log n) average, O(n) worst (skewed tree)
 * - Delete:              O(log n) average, O(n) worst
 * - In-order Traversal:  O(n)
 * 
 * SPACE COMPLEXITY:
 * -----------------
 * - O(n) for n patients
 * - Recursive operations use O(h) stack space where h = tree height
 */

class PatientBST {
private:
    /**
     * BST Node Implementation
     * -----------------------
     * Encapsulated as a private inner struct/class.
     * Hidden from the outside world, accessible only by PatientBST.
     */
    struct BSTNode {
        int patientId;      // Key for BST ordering
        Patient* patient;   // Pointer to patient data (stored in linked list)
        BSTNode* left;      // Left child (smaller IDs)
        BSTNode* right;     // Right child (larger IDs)

        BSTNode(int id, Patient* p) 
            : patientId(id), patient(p), left(nullptr), right(nullptr) {}
    };

    BSTNode* root;  // Root of the BST
    int size;       // Number of nodes

public:
    /**
     * Constructor - Initialize empty BST
     */
    PatientBST() : root(nullptr), size(0) {}

    /**
     * Destructor - Free all nodes
     */
    ~PatientBST() {
        destroyTree(root);
    }

    /**
     * Insert patient ID into BST
     * 
     * @param patientId The patient ID to insert
     * @param patient Pointer to the patient data
     * @return true if inserted, false if ID already exists
     * 
     * Time: O(log n) average, O(n) worst | Space: O(h) for recursion
     */
    bool insert(int patientId, Patient* patient) {
        if (search(patientId) != nullptr) {
            return false; // ID already exists
        }
        root = insertRecursive(root, patientId, patient);
        size++;
        return true;
    }

    /**
     * Search for patient by ID
     * 
     * @param patientId The ID to search for
     * @return Pointer to patient if found, nullptr otherwise
     * 
     * Time: O(log n) average, O(n) worst | Space: O(h) for recursion
     */
    Patient* search(int patientId) const {
        BSTNode* node = searchRecursive(root, patientId);
        if (node != nullptr) {
            return node->patient;
        }
        return nullptr;
    }

    /**
     * Delete patient ID from BST
     * 
     * @param patientId The ID to delete
     * @return true if deleted, false if not found
     * 
     * Time: O(log n) average | Space: O(h) for recursion
     */
    bool remove(int patientId) {
        if (search(patientId) == nullptr) {
            return false; // ID not found
        }
        root = deleteRecursive(root, patientId);
        size--;
        return true;
    }

    /**
     * In-order traversal - returns patients sorted by ID
     * Visits: Left subtree -> Node -> Right subtree
     * 
     * @return JSON array of patient IDs in sorted order
     * 
     * Time: O(n) | Space: O(n) for output + O(h) for recursion
     */
    std::string inorderTraversalJSON() const {
        std::stringstream ss;
        ss << "[";
        bool first = true;
        inorderHelper(root, ss, first);
        ss << "]";
        return ss.str();
    }

    /**
     * Get all patients in sorted order (by ID) as JSON
     */
    std::string getSortedPatientsJSON() const {
        std::stringstream ss;
        ss << "[";
        bool first = true;
        inorderPatientsHelper(root, ss, first);
        ss << "]";
        return ss.str();
    }

    /**
     * Get tree size
     */
    int getSize() const {
        return size;
    }

    /**
     * Check if tree is empty
     */
    bool isEmpty() const {
        return root == nullptr;
    }

    /**
     * Get tree height (for complexity analysis)
     * 
     * Time: O(n) | Space: O(h)
     */
    int getHeight() const {
        return heightHelper(root);
    }

    /**
     * Update patient pointer (when patient data is updated)
     */
    void updatePatientPointer(int patientId, Patient* newPatient) {
        BSTNode* node = searchRecursive(root, patientId);
        if (node != nullptr) {
            node->patient = newPatient;
        }
    }

private:
    /**
     * Recursive insert helper
     */
    BSTNode* insertRecursive(BSTNode* node, int patientId, Patient* patient) {
        // Base case: empty position found
        if (node == nullptr) {
            return new BSTNode(patientId, patient);
        }

        // Recursively find correct position
        if (patientId < node->patientId) {
            node->left = insertRecursive(node->left, patientId, patient);
        } else if (patientId > node->patientId) {
            node->right = insertRecursive(node->right, patientId, patient);
        }
        // If equal, do nothing (duplicate)

        return node;
    }

    /**
     * Recursive search helper
     */
    BSTNode* searchRecursive(BSTNode* node, int patientId) const {
        // Base cases: empty or found
        if (node == nullptr || node->patientId == patientId) {
            return node;
        }

        // Search left or right based on comparison
        if (patientId < node->patientId) {
            return searchRecursive(node->left, patientId);
        }
        return searchRecursive(node->right, patientId);
    }

    /**
     * Recursive delete helper
     */
    BSTNode* deleteRecursive(BSTNode* node, int patientId) {
        if (node == nullptr) return nullptr;

        // Find the node to delete
        if (patientId < node->patientId) {
            node->left = deleteRecursive(node->left, patientId);
        } else if (patientId > node->patientId) {
            node->right = deleteRecursive(node->right, patientId);
        } else {
            // Found node to delete

            // Case 1: Leaf node
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            // Case 2: One child
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }

            // Case 3: Two children
            // Find in-order successor (smallest in right subtree)
            BSTNode* successor = findMin(node->right);
            node->patientId = successor->patientId;
            node->patient = successor->patient;
            node->right = deleteRecursive(node->right, successor->patientId);
        }

        return node;
    }

    /**
     * Find minimum node (leftmost node)
     */
    BSTNode* findMin(BSTNode* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    /**
     * In-order traversal helper (IDs only)
     */
    void inorderHelper(BSTNode* node, std::stringstream& ss, bool& first) const {
        if (node == nullptr) return;

        inorderHelper(node->left, ss, first);
        
        if (!first) ss << ",";
        ss << node->patientId;
        first = false;

        inorderHelper(node->right, ss, first);
    }

    /**
     * In-order traversal helper (full patient data)
     */
    void inorderPatientsHelper(BSTNode* node, std::stringstream& ss, bool& first) const {
        if (node == nullptr) return;

        inorderPatientsHelper(node->left, ss, first);
        
        if (node->patient != nullptr) {
            if (!first) ss << ",";
            ss << node->patient->toJSON();
            first = false;
        }

        inorderPatientsHelper(node->right, ss, first);
    }

    /**
     * Height helper
     */
    int heightHelper(BSTNode* node) const {
        if (node == nullptr) return 0;
        int leftHeight = heightHelper(node->left);
        int rightHeight = heightHelper(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    /**
     * Destroy tree helper (post-order deletion)
     */
    void destroyTree(BSTNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
};

#endif // BST_H
