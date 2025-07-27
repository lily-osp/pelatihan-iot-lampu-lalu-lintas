// Simple IoT Teaching Platform JavaScript
document.addEventListener('DOMContentLoaded', function() {
    // Traffic light logic data with correct implementation from actual Arduino code
    const trafficLogic = {
        twoWay: {
            name: "Jalan Lurus 2 Arah",
            headers: ['State', 'Arah 1', 'Arah 2', 'Durasi (detik)', 'Keterangan'],
            rows: [
                [1, 'Hijau', 'Merah', 20, 'Arah 1 Hijau, Arah 2 Merah'],
                [2, 'Kuning', 'Merah', 3, 'Arah 1 Kuning, Arah 2 Merah'],
                [3, 'Merah', 'Merah', 2, 'Kedua Arah Merah (Transisi)'],
                [4, 'Merah', 'Hijau', 20, 'Arah 1 Merah, Arah 2 Hijau'],
                [5, 'Merah', 'Kuning', 3, 'Arah 1 Merah, Arah 2 Kuning'],
                [6, 'Merah', 'Merah', 2, 'Kedua Arah Merah (Transisi)']
            ],
            info: {
                totalCycle: "~50 detik",
                stateCount: "6 states",
                maxGreen: "20 detik",
                transitionTime: "2 detik"
            }
        },
        yShape: {
            name: "Pertigaan Y-Shape",
            headers: ['State', 'Jalur Utama', 'Cabang 1', 'Cabang 2', 'Durasi (detik)', 'Keterangan'],
            rows: [
                [1, 'Hijau', 'Merah', 'Merah', 30, 'Jalur Utama Hijau, Cabang 1 & 2 Merah'],
                [2, 'Kuning', 'Merah', 'Merah', 4, 'Jalur Utama Kuning, Cabang 1 & 2 Merah'],
                [3, 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)'],
                [4, 'Merah', 'Hijau', 'Merah', 25, 'Cabang 1 Hijau, Jalur Utama & Cabang 2 Merah'],
                [5, 'Merah', 'Kuning', 'Merah', 4, 'Cabang 1 Kuning, Jalur Utama & Cabang 2 Merah'],
                [6, 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)'],
                [7, 'Merah', 'Merah', 'Hijau', 25, 'Cabang 2 Hijau, Jalur Utama & Cabang 1 Merah'],
                [8, 'Merah', 'Merah', 'Kuning', 4, 'Cabang 2 Kuning, Jalur Utama & Cabang 1 Merah'],
                [9, 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)']
            ],
            info: {
                totalCycle: "~120 detik",
                stateCount: "9 states",
                maxGreen: "30 detik",
                transitionTime: "2 detik"
            }
        },
        tShape: {
            name: "Pertigaan T-Shape",
            headers: ['State', 'Main Road', 'Side Road', 'Left Turn', 'Durasi (detik)', 'Keterangan'],
            rows: [
                [1, 'Hijau', 'Merah', 'Merah', 35, 'Main Road Hijau, Side Road & Left Turn Merah'],
                [2, 'Kuning', 'Merah', 'Merah', 4, 'Main Road Kuning, Side Road & Left Turn Merah'],
                [3, 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)'],
                [4, 'Merah', 'Hijau', 'Merah', 20, 'Side Road Hijau, Main Road & Left Turn Merah'],
                [5, 'Merah', 'Kuning', 'Merah', 4, 'Side Road Kuning, Main Road & Left Turn Merah'],
                [6, 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)'],
                [7, 'Merah', 'Merah', 'Hijau', 15, 'Left Turn Hijau, Main Road & Side Road Merah'],
                [8, 'Merah', 'Merah', 'Kuning', 4, 'Left Turn Kuning, Main Road & Side Road Merah'],
                [9, 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)']
            ],
            info: {
                totalCycle: "~100 detik",
                stateCount: "9 states",
                maxGreen: "35 detik",
                transitionTime: "2 detik"
            }
        },
        fourWay: {
            name: "Perempatan Lengkap",
            headers: ['State', 'Utara', 'Selatan', 'Timur', 'Barat', 'Durasi (detik)', 'Keterangan'],
            rows: [
                [1, 'Hijau', 'Hijau', 'Merah', 'Merah', 30, 'Utara & Selatan Hijau, Timur & Barat Merah'],
                [2, 'Kuning', 'Kuning', 'Merah', 'Merah', 4, 'Utara & Selatan Kuning, Timur & Barat Merah'],
                [3, 'Merah', 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)'],
                [4, 'Merah', 'Merah', 'Hijau', 'Hijau', 25, 'Timur & Barat Hijau, Utara & Selatan Merah'],
                [5, 'Merah', 'Merah', 'Kuning', 'Kuning', 4, 'Timur & Barat Kuning, Utara & Selatan Merah'],
                [6, 'Merah', 'Merah', 'Merah', 'Merah', 2, 'Semua Merah (Transisi)'],
                [7, 'Hijau', 'Merah', 'Merah', 'Merah', 10, 'Individual: Utara Hijau (Belok Kiri)'],
                [8, 'Merah', 'Merah', 'Hijau', 'Merah', 10, 'Individual: Timur Hijau (Belok Kiri)'],
                [9, 'Merah', 'Hijau', 'Merah', 'Merah', 10, 'Individual: Selatan Hijau (Belok Kiri)'],
                [10, 'Merah', 'Merah', 'Merah', 'Hijau', 10, 'Individual: Barat Hijau (Belok Kiri)'],
                [11, 'Merah', 'Merah', 'Merah', 'Merah', 5, 'Emergency Mode: Semua Merah'],
                [12, 'Merah', 'Merah', 'Merah', 'Merah', 15, 'Pedestrian Mode: Semua Merah']
            ],
            info: {
                totalCycle: "~180 detik",
                stateCount: "12 states",
                maxGreen: "30 detik",
                transitionTime: "2 detik"
            }
        }
    };



    // Toast demo
    function showToast(type, message) {
        let toast = document.createElement('div');
        toast.className = `custom-toast toast-${type}`;
        toast.innerHTML = `<i class="material-icons-outlined">${type === 'success' ? 'check_circle' : 'error'}</i> ${message}`;
        document.body.appendChild(toast);
        setTimeout(() => toast.classList.add('show'), 100);
        setTimeout(() => { toast.classList.remove('show'); setTimeout(() => toast.remove(), 400); }, 3000);
    }

    // Initialize chart
    let trafficChart = null;
    function updateTrafficLogic(scenario) {
        const data = trafficLogic[scenario];
        const table = document.getElementById('logicTable');
        const thead = table.querySelector('thead');
        const tbody = table.querySelector('tbody');
        
        // Update table headers
        thead.innerHTML = `<tr>${data.headers.map(h => `<th style="padding:0.75rem; text-align:left; border-bottom:1px solid #e5e7eb; font-weight:600;">${h}</th>`).join('')}</tr>`;
        
        // Update table body
        tbody.innerHTML = data.rows.map(row => {
            const state = row[0];
            const lights = row.slice(1, -2); // All elements except first (state) and last two (duration, description)
            const duration = row[row.length - 2];
            const description = row[row.length - 1];
            
            return `<tr style="border-bottom:1px solid #f3f4f6;">
                <td style="padding:0.75rem; font-weight:600;">${state}</td>
                ${lights.map(light => {
                    let className = '';
                    if (light === 'Hijau') className = 'status-active';
                    else if (light === 'Kuning') className = 'status-masked';
                    else if (light === 'Merah') className = 'status-inactive';
                    return `<td class="${className}" style="padding:0.75rem;">${light}</td>`;
                }).join('')}
                <td style="padding:0.75rem; font-weight:600; text-align:center;">${duration}</td>
                <td style="padding:0.75rem; font-size:0.875rem; color:#6b7280;">${description}</td>
            </tr>`;
        }).join('');
        
        // Update chart
        const chartLabels = data.rows.map(row => `State ${row[0]}`);
        const chartData = data.rows.map(row => row[row.length - 2]); // Duration is second to last
        
        if (trafficChart) {
            trafficChart.data.labels = chartLabels;
            trafficChart.data.datasets[0].data = chartData;
            trafficChart.update();
        } else {
            const ctx = document.getElementById('trafficChart').getContext('2d');
            trafficChart = new Chart(ctx, {
                type: 'bar',
                data: {
                    labels: chartLabels,
                    datasets: [{
                        label: 'Durasi State (detik)',
                        data: chartData,
                        backgroundColor: data.rows.map((row, index) => {
                            const duration = row[row.length - 2];
                            if (duration >= 20) return '#166534'; // Green for long durations
                            else if (duration >= 10) return '#ca8a04'; // Yellow for medium
                            else return '#dc2626'; // Red for short
                        }),
                        borderColor: '#166534',
                        borderWidth: 1
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                        y: {
                            beginAtZero: true,
                            title: { display: true, text: 'Durasi (detik)' }
                        }
                    },
                    plugins: { 
                        legend: { display: false },
                        tooltip: {
                            callbacks: {
                                label: function(context) {
                                    return `Durasi: ${context.parsed.y} detik`;
                                }
                            }
                        }
                    }
                }
            });
        }
        
        // Update system info
        document.getElementById('totalCycle').textContent = data.info.totalCycle;
        document.getElementById('stateCount').textContent = data.info.stateCount;
        document.getElementById('maxGreen').textContent = data.info.maxGreen;
        document.getElementById('transitionTime').textContent = data.info.transitionTime;
    }

    // Button event listeners
    const btn2Way = document.getElementById('btn2Way');
    const btnYShape = document.getElementById('btnYShape');
    const btnTShape = document.getElementById('btnTShape');
    const btn4Way = document.getElementById('btn4Way');
    
    function updateButtonStates(activeButton) {
        // Reset all buttons
        [btn2Way, btnYShape, btnTShape, btn4Way].forEach(btn => {
            btn.style.background = '#737373';
        });
        // Set active button
        activeButton.style.background = '#166534';
    }
    
    btn2Way.addEventListener('click', function() {
        updateTrafficLogic('twoWay');
        updateButtonStates(btn2Way);
        showToast('success', 'Jalan Lurus 2 Arah dipilih!');
    });
    
    btnYShape.addEventListener('click', function() {
        updateTrafficLogic('yShape');
        updateButtonStates(btnYShape);
        showToast('success', 'Pertigaan Y-Shape dipilih!');
    });
    
    btnTShape.addEventListener('click', function() {
        updateTrafficLogic('tShape');
        updateButtonStates(btnTShape);
        showToast('success', 'Pertigaan T-Shape dipilih!');
    });
    
    btn4Way.addEventListener('click', function() {
        updateTrafficLogic('fourWay');
        updateButtonStates(btn4Way);
        showToast('success', 'Perempatan Lengkap dipilih!');
    });



    // Initialize everything
    updateTrafficLogic('twoWay');

    // Smooth scrolling for navigation links
    document.querySelectorAll('nav a').forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href');
            const targetSection = document.querySelector(targetId);
            if (targetSection) {
                targetSection.scrollIntoView({ behavior: 'smooth' });
            }
        });
    });

    // Show a demo toast on load
    setTimeout(() => showToast('success', 'Selamat datang di platform IoT Lampu Lalu Lintas!'), 500);
}); 