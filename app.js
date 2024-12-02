const socket = io();

const ctx = document.getElementById('temperatureChart').getContext('2d');
const temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
        datasets: [{
            label: 'Température (°C)',
            data: [],
            borderColor: '#ff6347',
            backgroundColor: 'rgba(255, 99, 71, 0.2)',
            fill: true,
            tension: 0.4,
            pointRadius: 3,
            pointHoverRadius: 5
        }]
    },
    options: {
        responsive: true,
        scales: {
            x: {
                type: 'time',
                time: {
                    unit: 'minute',
                    displayFormats: {
                        minute: 'HH:mm'
                    }
                },
                title: {
                    display: true,
                    text: 'Temps'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Température (°C)'
                },
                beginAtZero: false
            }
        }
    }
});

function updateCurrentTemperature(temperature) {
    document.getElementById('currentTemperature').textContent = `${temperature} °C`;
}

function addTemperatureToChart(data) {
    temperatureChart.data.labels.push(data.timestamp);
    temperatureChart.data.datasets[0].data.push({
        x: data.timestamp,
        y: data.temperature
    });
    temperatureChart.update();
}

socket.on('temperature_update', (data) => {
    console.log('Mise à jour reçue :', data);
    updateCurrentTemperature(data.temperature);
    addTemperatureToChart(data);
});

function updateChartWithRange(startDate, endDate) {
    socket.emit('get_temperature_range', startDate, endDate);
}

socket.on('temperature_range_update', (data) => {
    console.log('Données pour la plage reçues :', data);

    temperatureChart.data.labels = [];
    temperatureChart.data.datasets[0].data = [];

    data.forEach((entry) => {
        addTemperatureToChart(entry);
    });

    temperatureChart.update();
});

document.getElementById('updateChart').addEventListener('click', () => {
    const startDate = document.getElementById('startDate').value;
    const endDate = document.getElementById('endDate').value;

    if (!startDate || !endDate) {
        alert('Veuillez sélectionner les dates de début et de fin.');
        return;
    }

    const start = new Date(startDate).toISOString();
    const end = new Date(endDate).toISOString();

    updateChartWithRange(start, end);
});
