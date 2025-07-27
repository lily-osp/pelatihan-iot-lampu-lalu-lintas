// Simple IoT Teaching Platform JavaScript
document.addEventListener('DOMContentLoaded', function() {
    // Section Navigation with enhanced functionality
    const navLinks = document.querySelectorAll('.nav-link');
    const sections = document.querySelectorAll('.section, #hero, #overview');
    
    // Function to show section with smooth transitions
    function showSection(sectionId) {
        // Don't block user interactions - remove pointer-events blocking
        const activeLink = document.querySelector('.nav-link.active');
        
        // Hide all sections with smooth transition
        sections.forEach(section => {
            if (section.classList.contains('active')) {
                section.style.opacity = '0';
                section.style.transform = 'translateY(20px)';
                
                setTimeout(() => {
                    section.classList.remove('active');
                    section.style.opacity = '';
                    section.style.transform = '';
                }, 300);
            }
        });
        
        // Remove active class from all nav links
        navLinks.forEach(link => {
            link.classList.remove('active');
        });
        
        // Show target section with smooth animation
        const targetSection = document.getElementById(sectionId);
        if (targetSection) {
            // Small delay to ensure smooth transition
            setTimeout(() => {
                targetSection.classList.add('active');
                
                // Trigger reflow for smooth animation
                targetSection.offsetHeight;
                
                // Add entrance animation
                targetSection.style.opacity = '1';
                targetSection.style.transform = 'translateY(0)';
            }, 350);
        }
        
        // Add active class to clicked nav link with smooth transition
        const newActiveLink = document.querySelector(`[data-section="${sectionId}"]`);
        if (newActiveLink) {
            setTimeout(() => {
                newActiveLink.classList.add('active');
            }, 100);
        }
        
        // Scroll to top smoothly
        window.scrollTo({
            top: 0,
            behavior: 'smooth'
        });
    }
    
    // Enhanced click event listeners with better debouncing
    let isTransitioning = false;
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            
            if (isTransitioning) return;
            
            const sectionId = this.getAttribute('data-section');
            const currentActiveSection = document.querySelector('.section.active, #hero.active, #overview.active');
            
            // Don't transition if clicking the same section
            if (currentActiveSection && currentActiveSection.id === sectionId) {
                return;
            }
            
            isTransitioning = true;
            showSection(sectionId);
            
            // Reset transition flag after animation completes (reduced time for better responsiveness)
            setTimeout(() => {
                isTransitioning = false;
            }, 500);
        });
    });
    
    // Initialize with hero section by default
    showSection('hero');
    
    // Add keyboard navigation support
    document.addEventListener('keydown', function(e) {
        if (e.key === 'Escape') {
            // Return to hero section on Escape
            showSection('hero');
        }
    });
    
    // Add smooth scroll for anchor links
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function (e) {
            const href = this.getAttribute('href');
            if (href === '#') return;
            
            const targetId = href.substring(1);
            const targetElement = document.getElementById(targetId);
            
            if (targetElement && !this.classList.contains('nav-link')) {
                e.preventDefault();
                targetElement.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    
    // Interactive traffic light demo animation
    function animateTrafficLight() {
        const lights = document.querySelectorAll('.traffic-light-demo .light');
        let currentLight = 0;
        
        function cycleLight() {
            // Remove active class from all lights
            lights.forEach(light => light.classList.remove('active'));
            
            // Add active class to current light
            lights[currentLight].classList.add('active');
            
            // Move to next light
            currentLight = (currentLight + 1) % lights.length;
        }
        
        // Start the animation
        cycleLight();
        setInterval(cycleLight, 3000); // Change every 3 seconds for more realistic timing
    }
    
    // Start traffic light animation when hero section is active
    function startTrafficLightAnimation() {
        const heroSection = document.getElementById('hero');
        if (heroSection && heroSection.classList.contains('active')) {
            animateTrafficLight();
        }
    }
    
    // Enhanced section switching with visual feedback
    const originalShowSection = showSection;
    showSection = function(sectionId) {
        // Show transition feedback (non-blocking)
        const transitionFeedback = document.getElementById('transitionFeedback');
        if (transitionFeedback) {
            transitionFeedback.classList.add('show');
        }
        

        
        // Update progress bar
        updateProgressBar(sectionId);
        
        // Update section counter
        updateSectionCounter(sectionId);
        
        // Call original function
        originalShowSection(sectionId);
        
        // Hide feedback elements after transition
        setTimeout(() => {
            if (transitionFeedback) {
                transitionFeedback.classList.remove('show');
            }
            
            // Start traffic light animation if hero section
            if (sectionId === 'hero') {
                startTrafficLightAnimation();
            }
        }, 500);
    };
    
    // Function to update progress bar
    function updateProgressBar(sectionId) {
        const progressBar = document.getElementById('progressBar');
        if (!progressBar) return;
        
        const sections = ['hero', 'overview', 'iot-fundamentals', 'microcontroller', 'project-details', 'technical', 'wokwi-simulations', 'practical-examples', 'troubleshooting', 'sources'];
        const currentIndex = sections.indexOf(sectionId);
        const progress = ((currentIndex + 1) / sections.length) * 100;
        
        progressBar.style.width = progress + '%';
    }
    
    // Function to update section counter
    function updateSectionCounter(sectionId) {
        const sectionCounter = document.getElementById('sectionCounter');
        const currentSectionSpan = document.getElementById('currentSection');
        const totalSectionsSpan = document.getElementById('totalSections');
        
        if (!sectionCounter || !currentSectionSpan || !totalSectionsSpan) return;
        
        const sectionNames = {
            'hero': 'Hero',
            'overview': 'Ringkasan',
            'iot-fundamentals': 'Dasar IoT',
            'microcontroller': 'Mikrokontroler',
            'project-details': 'Proyek',
            'technical': 'Teknis',
            'wokwi-simulations': 'Simulasi',
            'practical-examples': 'Contoh',
            'troubleshooting': 'Pemecahan',
            'sources': 'Sumber'
        };
        
        const sections = Object.keys(sectionNames);
        const currentIndex = sections.indexOf(sectionId);
        
        currentSectionSpan.textContent = sectionNames[sectionId] || 'Hero';
        totalSectionsSpan.textContent = sections.length;
        
        // Show section counter
        sectionCounter.classList.add('show');
        
        // Hide after 3 seconds
        setTimeout(() => {
            sectionCounter.classList.remove('show');
        }, 3000);
    }
    
    // Start initial traffic light animation
    startTrafficLightAnimation();
    
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
                [7, 'Hijau', 'Merah', 'Merah', 'Merah', 10, 'Individual: Utara Hijau (Belok Kanan)'],
                [8, 'Merah', 'Merah', 'Hijau', 'Merah', 10, 'Individual: Timur Hijau (Belok Kanan)'],
                [9, 'Merah', 'Hijau', 'Merah', 'Merah', 10, 'Individual: Selatan Hijau (Belok Kanan)'],
                [10, 'Merah', 'Merah', 'Merah', 'Hijau', 10, 'Individual: Barat Hijau (Belok Kanan)'],
                [11, 'Kuning*', 'Kuning*', 'Kuning*', 'Kuning*', 5, 'Emergency Mode: Blinking Yellow'],
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

    // Show a demo toast on load
    setTimeout(() => showToast('success', 'Selamat datang di platform IoT Lampu Lalu Lintas!'), 500);
}); 