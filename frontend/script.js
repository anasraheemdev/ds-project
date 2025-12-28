/**
 * Patient Management System - Frontend JavaScript
 * ================================================
 * 
 * This file handles all frontend logic:
 * - API communication with Python backend
 * - DOM manipulation for UI updates
 * - Form handling and validation
 * - Toast notifications
 * 
 * DATA FLOW:
 * User Action -> JavaScript -> Fetch API -> Flask Server -> C++ Backend
 *                                                              |
 * DOM Update <- JavaScript <- JSON Response <- Flask Server <--+
 */

// ============================================
// CONFIGURATION
// ============================================

const API_BASE_URL = 'http://localhost:5000/api';

// ============================================
// API HELPER FUNCTIONS
// ============================================

/**
 * Make API request with error handling
 * @param {string} endpoint - API endpoint
 * @param {object} options - Fetch options
 * @returns {Promise<object>} - JSON response
 */
async function apiRequest(endpoint, options = {}) {
    try {
        const response = await fetch(`${API_BASE_URL}${endpoint}`, {
            headers: {
                'Content-Type': 'application/json',
            },
            ...options
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        return await response.json();
    } catch (error) {
        console.error('API Error:', error);
        showToast(`Error: ${error.message}`, 'error');
        throw error;
    }
}

// ============================================
// TOAST NOTIFICATIONS
// ============================================

/**
 * Show toast notification
 * @param {string} message - Message to display
 * @param {string} type - Type: 'success', 'error', 'warning'
 */
function showToast(message, type = 'success') {
    const container = document.getElementById('toastContainer');
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    toast.textContent = message;

    container.appendChild(toast);

    // Auto-remove after 3 seconds
    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transform = 'translateX(100%)';
        setTimeout(() => toast.remove(), 300);
    }, 3000);
}

// ============================================
// PATIENT OPERATIONS
// ============================================

/**
 * Load all patients from linked list
 */
async function loadPatients() {
    try {
        const patients = await apiRequest('/patients');
        renderPatientTable(patients);
        updateStats();
    } catch (error) {
        console.error('Failed to load patients:', error);
    }
}

/**
 * Load patients sorted by ID (uses BST in-order traversal)
 */
async function loadPatientsSorted() {
    try {
        const patients = await apiRequest('/patients/sorted');
        renderPatientTable(patients);
        showToast('Patients sorted by ID (BST in-order traversal)', 'success');
    } catch (error) {
        console.error('Failed to load sorted patients:', error);
    }
}

/**
 * Render patient table
 * @param {Array} patients - Array of patient objects
 */
function renderPatientTable(patients) {
    const tbody = document.getElementById('patientTableBody');
    const emptyState = document.getElementById('emptyState');
    const table = document.getElementById('patientTable');

    if (!patients || patients.length === 0) {
        tbody.innerHTML = '';
        emptyState.classList.remove('hidden');
        table.classList.add('hidden');
        return;
    }

    emptyState.classList.add('hidden');
    table.classList.remove('hidden');

    tbody.innerHTML = patients.map(patient => `
        <tr>
            <td><strong>${patient.patientId}</strong></td>
            <td>${escapeHtml(patient.name)}</td>
            <td>${patient.age}</td>
            <td>${escapeHtml(patient.gender)}</td>
            <td>${escapeHtml(patient.disease)}</td>
            <td>${escapeHtml(patient.contactNumber)}</td>
            <td>${escapeHtml(patient.appointmentDate)}</td>
            <td class="actions">
                <button class="btn btn-small btn-ghost" onclick="editPatient(${patient.patientId})" title="Edit">
                    ✏️
                </button>
                <button class="btn btn-small btn-ghost" onclick="addToQueue(${patient.patientId})" title="Add to Queue">
                    ⏰
                </button>
                <button class="btn btn-small btn-ghost text-danger" onclick="deletePatient(${patient.patientId})" title="Delete">
                    🗑️
                </button>
            </td>
        </tr>
    `).join('');
}

/**
 * Add new patient
 */
async function addPatient(formData) {
    try {
        const result = await apiRequest('/patient', {
            method: 'POST',
            body: JSON.stringify(formData)
        });

        if (result.success) {
            showToast(`Patient ${result.patient.name} added with ID ${result.patient.patientId}`, 'success');
            loadPatients();
            loadActivities();
            document.getElementById('addPatientForm').reset();
        } else {
            showToast(result.message, 'error');
        }
    } catch (error) {
        console.error('Failed to add patient:', error);
    }
}

/**
 * Search patient by ID
 */
async function searchPatient(patientId) {
    try {
        const result = await apiRequest(`/patient/${patientId}`);
        const resultDiv = document.getElementById('searchResult');

        if (result.success) {
            const p = result.patient;
            resultDiv.innerHTML = `
                <h3>Patient Found (BST Search)</h3>
                <p><strong>ID:</strong> ${p.patientId}</p>
                <p><strong>Name:</strong> ${escapeHtml(p.name)}</p>
                <p><strong>Age:</strong> ${p.age} | <strong>Gender:</strong> ${escapeHtml(p.gender)}</p>
                <p><strong>Disease:</strong> ${escapeHtml(p.disease)}</p>
                <p><strong>Contact:</strong> ${escapeHtml(p.contactNumber)}</p>
                <p><strong>Appointment:</strong> ${escapeHtml(p.appointmentDate)}</p>
                ${p.visitNotes ? `<p><strong>Notes:</strong> ${escapeHtml(p.visitNotes)}</p>` : ''}
            `;
            resultDiv.classList.remove('hidden', 'error');
            loadActivities();
        } else {
            resultDiv.innerHTML = `<p>${result.message}</p>`;
            resultDiv.classList.remove('hidden');
            resultDiv.classList.add('error');
        }
    } catch (error) {
        console.error('Search failed:', error);
    }
}

/**
 * Delete patient
 */
async function deletePatient(patientId) {
    if (!confirm(`Are you sure you want to delete patient ${patientId}?`)) {
        return;
    }

    try {
        const result = await apiRequest(`/patient/${patientId}`, {
            method: 'DELETE'
        });

        if (result.success) {
            showToast('Patient deleted successfully', 'success');
            loadPatients();
            loadActivities();
        } else {
            showToast(result.message, 'error');
        }
    } catch (error) {
        console.error('Failed to delete patient:', error);
    }
}

/**
 * Open edit modal for patient
 */
async function editPatient(patientId) {
    try {
        const result = await apiRequest(`/patient/${patientId}`);

        if (result.success) {
            const p = result.patient;
            document.getElementById('editPatientId').value = p.patientId;
            document.getElementById('editName').value = p.name;
            document.getElementById('editAge').value = p.age;
            document.getElementById('editGender').value = p.gender;
            document.getElementById('editDisease').value = p.disease;
            document.getElementById('editContactNumber').value = p.contactNumber;
            document.getElementById('editAppointmentDate').value = p.appointmentDate;
            document.getElementById('editVisitNotes').value = p.visitNotes || '';

            document.getElementById('editModal').classList.remove('hidden');
        } else {
            showToast(result.message, 'error');
        }
    } catch (error) {
        console.error('Failed to load patient for edit:', error);
    }
}

/**
 * Close edit modal
 */
function closeEditModal() {
    document.getElementById('editModal').classList.add('hidden');
}

/**
 * Update patient
 */
async function updatePatient(patientId, formData) {
    try {
        const result = await apiRequest(`/patient/${patientId}`, {
            method: 'PUT',
            body: JSON.stringify(formData)
        });

        if (result.success) {
            showToast('Patient updated successfully', 'success');
            closeEditModal();
            loadPatients();
            loadActivities();
        } else {
            showToast(result.message, 'error');
        }
    } catch (error) {
        console.error('Failed to update patient:', error);
    }
}

// ============================================
// QUEUE OPERATIONS
// ============================================

/**
 * Load appointment queue
 */
async function loadQueue() {
    try {
        const result = await apiRequest('/queue');
        renderQueue(result.waitingList || []);
    } catch (error) {
        console.error('Failed to load queue:', error);
    }
}

/**
 * Render queue UI
 */
function renderQueue(queue) {
    const queueList = document.getElementById('queueList');
    const queueEmpty = document.getElementById('queueEmpty');

    if (!queue || queue.length === 0) {
        queueList.innerHTML = '';
        queueEmpty.style.display = 'block';
        return;
    }

    queueEmpty.style.display = 'none';
    queueList.innerHTML = queue.map(item => `
        <div class="queue-item">
            <div class="queue-position">${item.position}</div>
            <div class="queue-info">
                <div class="queue-name">${escapeHtml(item.patientName)}</div>
                <div class="queue-details">ID: ${item.patientId} | ${escapeHtml(item.appointmentTime)}</div>
            </div>
        </div>
    `).join('');
}

/**
 * Add patient to queue
 */
async function addToQueue(patientId) {
    try {
        const result = await apiRequest('/queue', {
            method: 'POST',
            body: JSON.stringify({ patientId })
        });

        if (result.success) {
            showToast(`Patient added to queue at position ${result.position}`, 'success');
            loadQueue();
            loadActivities();
            updateStats();
        } else {
            showToast(result.message, 'warning');
        }
    } catch (error) {
        console.error('Failed to add to queue:', error);
    }
}

/**
 * Process next appointment
 */
async function processNextAppointment() {
    try {
        const result = await apiRequest('/queue/process', {
            method: 'POST'
        });

        if (result.success) {
            const appt = result.appointment;
            showToast(`Processing appointment for ${appt.patientName} (ID: ${appt.patientId})`, 'success');
            loadQueue();
            loadActivities();
            updateStats();
        } else {
            showToast(result.message, 'warning');
        }
    } catch (error) {
        console.error('Failed to process appointment:', error);
    }
}

// ============================================
// ACTIVITY STACK OPERATIONS
// ============================================

/**
 * Load recent activities
 */
async function loadActivities() {
    try {
        const result = await apiRequest('/activities?count=10');
        renderActivities(result.activities || []);
    } catch (error) {
        console.error('Failed to load activities:', error);
    }
}

/**
 * Render activities UI
 */
function renderActivities(activities) {
    const activityList = document.getElementById('activityList');
    const activityEmpty = document.getElementById('activityEmpty');

    if (!activities || activities.length === 0) {
        activityList.innerHTML = '';
        activityEmpty.style.display = 'block';
        return;
    }

    activityEmpty.style.display = 'none';

    const iconMap = {
        'ADD': { icon: '➕', class: 'add' },
        'DELETE': { icon: '🗑️', class: 'delete' },
        'UPDATE': { icon: '✏️', class: 'update' },
        'VIEW': { icon: '👁️', class: 'view' },
        'QUEUE': { icon: '⏰', class: 'queue' },
        'PROCESS': { icon: '✅', class: 'process' },
        'LOAD': { icon: '📂', class: 'add' },
        'SAMPLE': { icon: '📥', class: 'add' },
        'CLEAR': { icon: '🗑️', class: 'delete' }
    };

    activityList.innerHTML = activities.map(act => {
        const iconInfo = iconMap[act.action] || { icon: '📋', class: 'view' };
        return `
            <div class="activity-item">
                <div class="activity-icon ${iconInfo.class}">${iconInfo.icon}</div>
                <div class="activity-content">
                    <div class="activity-action">${act.action}</div>
                    <div class="activity-patient">${escapeHtml(act.patientName)} (ID: ${act.patientId})</div>
                    <div class="activity-time">${escapeHtml(act.timestamp)}</div>
                </div>
            </div>
        `;
    }).join('');
}

// ============================================
// STATISTICS
// ============================================

/**
 * Update header statistics
 */
async function updateStats() {
    try {
        const stats = await apiRequest('/statistics');
        document.getElementById('totalPatients').textContent = stats.totalPatients || 0;
        document.getElementById('queueCount').textContent = stats.queueSize || 0;
        document.getElementById('activityCount').textContent = stats.activityCount || 0;
    } catch (error) {
        console.error('Failed to update stats:', error);
    }
}

// ============================================
// NEW FEATURES: SAMPLE DATA, EXPORT, CLEAR
// ============================================

/**
 * Load sample patient data for testing
 */
async function loadSampleData() {
    if (!confirm('This will add 5 sample patients. Continue?')) {
        return;
    }

    try {
        const result = await apiRequest('/sample-data', { method: 'POST' });

        if (result.success) {
            showToast(`Loaded ${result.count} sample patients!`, 'success');
            loadPatients();
            loadActivities();
            updateStats();
        } else {
            showToast(result.message, 'error');
        }
    } catch (error) {
        console.error('Failed to load sample data:', error);
    }
}

/**
 * Export all data as JSON file download
 */
async function exportData() {
    try {
        const response = await fetch(`${API_BASE_URL}/export`);
        const blob = await response.blob();

        // Create download link
        const url = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = 'patients_export.json';
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        window.URL.revokeObjectURL(url);

        showToast('Data exported successfully!', 'success');
    } catch (error) {
        console.error('Failed to export data:', error);
        showToast('Failed to export data', 'error');
    }
}

/**
 * Clear all data (with confirmation)
 */
async function clearAllData() {
    if (!confirm('⚠️ WARNING: This will DELETE ALL patient data permanently!\n\nAre you sure?')) {
        return;
    }

    if (!confirm('This action CANNOT be undone. Type YES to confirm.') || !confirm('Final confirmation - Delete all data?')) {
        showToast('Data clearing cancelled', 'warning');
        return;
    }

    try {
        const result = await apiRequest('/clear', { method: 'POST' });

        if (result.success) {
            showToast('All data cleared!', 'success');
            loadPatients();
            loadQueue();
            loadActivities();
            updateStats();
        } else {
            showToast(result.message, 'error');
        }
    } catch (error) {
        console.error('Failed to clear data:', error);
    }
}

/**
 * Search patients by name
 */
async function searchByName(name) {
    try {
        const result = await apiRequest(`/patients/search?name=${encodeURIComponent(name)}`);
        const resultDiv = document.getElementById('nameSearchResult');

        if (result.success && result.count > 0) {
            let html = `<h3>Found ${result.count} patient(s)</h3>`;
            result.patients.forEach(p => {
                html += `
                    <div style="padding: 0.5rem; border-bottom: 1px solid #eee;">
                        <strong>ID: ${p.patientId}</strong> - ${escapeHtml(p.name)}<br>
                        <small>${escapeHtml(p.disease)} | ${escapeHtml(p.gender)}, ${p.age}y</small>
                    </div>
                `;
            });
            resultDiv.innerHTML = html;
            resultDiv.classList.remove('hidden', 'error');
        } else {
            resultDiv.innerHTML = `<p>No patients found matching "${escapeHtml(name)}"</p>`;
            resultDiv.classList.remove('hidden');
            resultDiv.classList.add('error');
        }
    } catch (error) {
        console.error('Search by name failed:', error);
    }
}

// ============================================
// UTILITY FUNCTIONS
// ============================================

/**
 * Escape HTML to prevent XSS
 */
function escapeHtml(text) {
    if (!text) return '';
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

/**
 * Get form data as object
 */
function getFormData(formId) {
    const form = document.getElementById(formId);
    const formData = new FormData(form);
    const data = {};
    formData.forEach((value, key) => {
        data[key] = value;
    });
    return data;
}

// ============================================
// EVENT LISTENERS
// ============================================

document.addEventListener('DOMContentLoaded', () => {
    // Set default appointment date to today
    const today = new Date().toISOString().split('T')[0];
    document.getElementById('appointmentDate').value = today;

    // Add Patient Form
    document.getElementById('addPatientForm').addEventListener('submit', (e) => {
        e.preventDefault();
        const data = getFormData('addPatientForm');
        addPatient(data);
    });

    // Search Patient Form
    document.getElementById('searchPatientForm').addEventListener('submit', (e) => {
        e.preventDefault();
        const searchId = document.getElementById('searchId').value;
        searchPatient(parseInt(searchId));
    });

    // Search by Name Form
    document.getElementById('searchByNameForm').addEventListener('submit', (e) => {
        e.preventDefault();
        const searchName = document.getElementById('searchName').value;
        searchByName(searchName);
    });

    // Edit Patient Form
    document.getElementById('editPatientForm').addEventListener('submit', (e) => {
        e.preventDefault();
        const patientId = document.getElementById('editPatientId').value;
        const data = {
            name: document.getElementById('editName').value,
            age: document.getElementById('editAge').value,
            gender: document.getElementById('editGender').value,
            disease: document.getElementById('editDisease').value,
            contactNumber: document.getElementById('editContactNumber').value,
            appointmentDate: document.getElementById('editAppointmentDate').value,
            visitNotes: document.getElementById('editVisitNotes').value
        };
        updatePatient(parseInt(patientId), data);
    });

    // Close modal on Escape key
    document.addEventListener('keydown', (e) => {
        if (e.key === 'Escape') {
            closeEditModal();
        }
    });

    // Initial data load
    loadPatients();
    loadQueue();
    loadActivities();
    updateStats();

    // Refresh data every 30 seconds
    setInterval(() => {
        loadPatients();
        loadQueue();
        loadActivities();
        updateStats();
    }, 30000);
});

// ============================================
// EXPOSE FUNCTIONS TO GLOBAL SCOPE
// ============================================
window.loadPatients = loadPatients;
window.loadPatientsSorted = loadPatientsSorted;
window.editPatient = editPatient;
window.deletePatient = deletePatient;
window.addToQueue = addToQueue;
window.processNextAppointment = processNextAppointment;
window.closeEditModal = closeEditModal;
window.loadSampleData = loadSampleData;
window.exportData = exportData;
window.clearAllData = clearAllData;
window.searchByName = searchByName;
