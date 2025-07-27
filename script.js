// Simple IoT Teaching Platform JavaScript
document.addEventListener('DOMContentLoaded', function() {
    // Traffic light logic data with correct implementation from actual code
    const trafficLogic = {
        simpangEmpat: {
            headers: ['State', 'Utara', 'Selatan', 'Timur', 'Barat', 'Durasi (detik)', 'Status'],
            rows: [
                [1, 'Hijau', 'Hijau', 'Merah', 'Merah', 25, 'enabled'],
                [2, 'Kuning', 'Kuning', 'Merah', 'Merah', 3, 'activating'],
                [3, 'Merah', 'Merah', 'Merah', 'Merah', 2, 'static'],
                [4, 'Merah', 'Merah', 'Hijau', 'Hijau', 20, 'active'],
                [5, 'Merah', 'Merah', 'Kuning', 'Kuning', 3, 'masked'],
                [6, 'Merah', 'Merah', 'Merah', 'Merah', 2, 'failed']
            ]
        },
        simpangTiga: {
            headers: ['State', 'Jalur Utama', 'Cabang 1', 'Cabang 2', 'Durasi (detik)', 'Status'],
            rows: [
                [1, 'Hijau', 'Merah', 'Merah', 30, 'enabled'],
                [2, 'Kuning', 'Merah', 'Merah', 3, 'activating'],
                [3, 'Merah', 'Hijau', 'Merah', 15, 'active'],
                [4, 'Merah', 'Kuning', 'Merah', 3, 'masked'],
                [5, 'Merah', 'Merah', 'Hijau', 12, 'active'],
                [6, 'Merah', 'Merah', 'Kuning', 3, 'masked'],
                [7, 'Merah', 'Merah', 'Merah', 2, 'failed']
            ]
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
        thead.innerHTML = `<tr>${data.headers.map(h => `<th>${h}</th>`).join('')}</tr>`;
        tbody.innerHTML = data.rows.map(row => {
            const state = row[0];
            const lights = row.slice(1, -2); // All elements except first (state) and last two (duration, status)
            const duration = row[row.length - 2];
            const status = row[row.length - 1];
            return `<tr>
                <td>${state}</td>
                ${lights.map(light => `<td class="${light === 'Hijau' ? 'status-active' : light === 'Kuning' ? 'status-masked' : light === 'Merah' ? 'status-inactive' : ''}">${light}</td>`).join('')}
                <td>${duration}</td>
                <td class="status-${status}">${status.charAt(0).toUpperCase() + status.slice(1)}</td>
            </tr>`;
        }).join('');
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
                        backgroundColor: '#166534',
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
                    plugins: { legend: { display: false } }
                }
            });
        }
    }

    // Button event listeners
    const btnPerempatan = document.getElementById('btnPerempatan');
    const btnPertigaan = document.getElementById('btnPertigaan');
    btnPerempatan.addEventListener('click', function() {
        updateTrafficLogic('simpangEmpat');
        btnPerempatan.style.background = '#166534';
        btnPertigaan.style.background = '#737373';
        showToast('success', 'Simpang Empat dipilih!');
    });
    btnPertigaan.addEventListener('click', function() {
        updateTrafficLogic('simpangTiga');
        btnPertigaan.style.background = '#166534';
        btnPerempatan.style.background = '#737373';
        showToast('error', 'Simpang Tiga dipilih!');
    });



    // Initialize everything
    updateTrafficLogic('simpangEmpat');

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