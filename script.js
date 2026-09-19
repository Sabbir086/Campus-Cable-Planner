const API_BASE = "http://localhost:18080/api"; // Assuming a similar Crow backend

// Embedded Data for Frontend Simulation (Since we don't have a backend ready for this one yet)
const nodes = [
    "", "Engineering Building", "YKSG-2", "RASG-1", "Food Court", "Auditorium", 
    "AB-4", "Annex Building", "Admission Building", "Hall Accommodation", 
    "ID Card Section", "Gate-4", "Gate-3", "Gate-2", "AB-1", "Teachers Home", 
    "Gate-8", "Mosque", "AB-3", "Green Garden", "Gate-1", "Transport Building"
];

const edges = [
    {id: 1, u: 6, v: 7, length: 120, cost: 12000, cap: 100, load: 25},
    {id: 2, u: 6, v: 5, length: 80, cost: 8000, cap: 100, load: 45},
    {id: 3, u: 5, v: 4, length: 60, cost: 6000, cap: 100, load: 60},
    {id: 4, u: 4, v: 3, length: 90, cost: 9000, cap: 100, load: 30},
    {id: 5, u: 3, v: 2, length: 70, cost: 7000, cap: 100, load: 70},
    {id: 6, u: 2, v: 1, length: 100, cost: 10000, cap: 100, load: 80},
    {id: 7, u: 3, v: 14, length: 110, cost: 11000, cap: 100, load: 35},
    {id: 8, u: 6, v: 14, length: 75, cost: 7500, cap: 100, load: 20},
    {id: 9, u: 6, v: 11, length: 50, cost: 5000, cap: 100, load: 75},
    {id: 10, u: 6, v: 8, length: 95, cost: 9500, cap: 100, load: 40},
    {id: 11, u: 9, v: 8, length: 65, cost: 6500, cap: 100, load: 25},
    {id: 12, u: 9, v: 12, length: 85, cost: 8500, cap: 100, load: 55},
    {id: 13, u: 8, v: 12, length: 55, cost: 5500, cap: 100, load: 65},
    {id: 14, u: 8, v: 10, length: 45, cost: 4500, cap: 100, load: 30},
    {id: 15, u: 10, v: 13, length: 70, cost: 7000, cap: 100, load: 50},
    {id: 16, u: 14, v: 15, length: 90, cost: 9000, cap: 100, load: 45},
    {id: 17, u: 15, v: 16, length: 60, cost: 6000, cap: 100, load: 70},
    {id: 18, u: 14, v: 17, length: 50, cost: 5000, cap: 100, load: 35},
    {id: 19, u: 17, v: 18, length: 65, cost: 6500, cap: 100, load: 50},
    {id: 20, u: 18, v: 3, length: 80, cost: 8000, cap: 100, load: 25},
    {id: 21, u: 3, v: 19, length: 75, cost: 7500, cap: 100, load: 40},
    {id: 22, u: 3, v: 20, length: 100, cost: 10000, cap: 100, load: 65},
    {id: 23, u: 14, v: 21, length: 120, cost: 12000, cap: 100, load: 30}
];

// --- LOGIN (Updated to FUBAO / FUBAO123) ---
document.getElementById('loginForm').addEventListener('submit', function(e) {
    e.preventDefault();
    const user = document.getElementById('username').value;
    const pass = document.getElementById('password').value;
    
    // Updated Credentials
    if(user === 'FUBAO' && pass === 'FUBAO123'){
        document.getElementById('loginPage').classList.add('hidden');
        document.getElementById('app').classList.remove('hidden');
        initApp();
    } else {
        document.getElementById('loginMessage').innerText = "Invalid credentials!";
    }
});

// --- NAVIGATION ---
document.querySelectorAll('.nav-btn, .quick-btn').forEach(btn => {
    btn.addEventListener('click', (e) => {
        const section = e.currentTarget.getAttribute('data-section');
        if(!section) return;

        document.querySelectorAll('.nav-btn').forEach(b => b.classList.remove('active'));
        document.querySelectorAll('.section').forEach(s => s.classList.remove('active-section'));

        const navBtn = document.querySelector(`.nav-btn[data-section="${section}"]`);
        if(navBtn) navBtn.classList.add('active');

        const secElement = document.getElementById(section);
        if(secElement) secElement.classList.add('active-section');
        
        if (e.currentTarget.classList.contains('nav-btn')) {
            document.getElementById('pageTitle').innerText = e.currentTarget.innerText.replace(/[^a-zA-Z ]/g, "").trim();
        }
    });
});

document.getElementById('logoutBtn').addEventListener('click', () => {
    document.getElementById('app').classList.add('hidden');
    document.getElementById('loginPage').classList.remove('hidden');
    document.getElementById('loginForm').reset();
});

// --- DEVELOPER INFO POPUP ---
const devBtn = document.getElementById('devBtn');
const devPopup = document.getElementById('devPopup');

devBtn.addEventListener('click', () => {
    devPopup.style.display = 'block';
});

devBtn.addEventListener('mouseleave', () => {
    setTimeout(() => {
        if(!devPopup.matches(':hover')) {
             devPopup.style.display = 'none';
        }
    }, 100);
});

devPopup.addEventListener('mouseleave', () => {
    devPopup.style.display = 'none';
});

document.addEventListener('click', (e) => {
    if (!devBtn.contains(e.target) && !devPopup.contains(e.target)) {
        devPopup.style.display = 'none';
    }
});

// --- APP INIT ---
function initApp() {
    // Populate Selects
    const pathStart = document.getElementById('pathStart');
    const pathEnd = document.getElementById('pathEnd');
    const failSelect = document.getElementById('failSelect');

    let nodeOptions = '<option value="">Select Node</option>';
    for(let i=1; i<nodes.length; i++){
        nodeOptions += `<option value="${i}">${nodes[i]}</option>`;
    }
    pathStart.innerHTML = nodeOptions;
    pathEnd.innerHTML = nodeOptions;

    let edgeOptions = '<option value="">Select Connection</option>';
    edges.forEach((e, i) => {
        edgeOptions += `<option value="${i}">${nodes[e.u]} -> ${nodes[e.v]}</option>`;
    });
    failSelect.innerHTML = edgeOptions;

    loadCables();
}

function loadCables(filter = '') {
    const tbody = document.getElementById('connectionsBody');
    tbody.innerHTML = '';
    edges.forEach(e => {
        const src = nodes[e.u];
        const dest = nodes[e.v];
        if(filter === '' || src.toLowerCase().includes(filter.toLowerCase()) || dest.toLowerCase().includes(filter.toLowerCase())) {
            tbody.innerHTML += `
                <tr>
                    <td>${e.id}</td>
                    <td>${src}</td>
                    <td>${dest}</td>
                    <td>${e.length}</td>
                    <td>${e.cap}</td>
                    <td class="${e.load > 80 ? 'due' : 'paid'}">${e.load}</td>
                </tr>
            `;
        }
    });
}

document.getElementById('searchNodeBtn').addEventListener('click', () => {
    loadCables(document.getElementById('searchNodeInput').value);
});

// Simplified JS Dijkstra for UI Demo
document.getElementById('findPathBtn').addEventListener('click', () => {
    const s = parseInt(document.getElementById('pathStart').value);
    const e = parseInt(document.getElementById('pathEnd').value);
    const res = document.getElementById('pathResult');
    
    if(!s || !e) {
        res.innerHTML = '<p style="color:red;">Please select both nodes.</p>';
        res.classList.remove('hidden');
        return;
    }

    res.classList.remove('hidden');
    res.innerHTML = `
        <h3>Shortest Path Calculation</h3>
        <p>Start: <strong>${nodes[s]}</strong></p>
        <p>End: <strong>${nodes[e]}</strong></p>
        <br>
        <p style="color:#2b6cb0;"><i>(For full graph calculation results, refer to the C++ backend.)</i></p>
        <div class="detail-item" style="margin-top:15px;">
             <span>Note</span>
             <strong>This is a UI placeholder. Full logic exists in main.cpp.</strong>
        </div>
    `;
});

document.getElementById('simFailBtn').addEventListener('click', () => {
    const idx = document.getElementById('failSelect').value;
    const res = document.getElementById('failResult');
    
    if(idx === '') return;
    
    const edge = edges[idx];
    res.classList.remove('hidden');
    res.innerHTML = `
        <div class="record-details">
            <h3>Failure Simulation</h3>
            <div class="detail-grid">
                <div class="detail-item"><span>Failed Link</span><strong>${nodes[edge.u]} -> ${nodes[edge.v]}</strong></div>
                <div class="detail-item"><span>Status</span><strong style="color:red;">CRITICAL / WARNING</strong></div>
            </div>
            <p style="margin-top:15px; color:#6b7280;">Run C++ terminal for full network connectivity checks.</p>
        </div>
    `;
});
