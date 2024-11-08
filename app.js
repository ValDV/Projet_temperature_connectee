const socket = io();

const ctx = document.getElementById('temperatureChart').getContext('2d');
const temperatureData = {
    labels: [],
    datasets: [{
        label: 'Température (°C)',
        data: [],
        borderColor: 'rgba(255, 99, 132, 1)',
        backgroundColor: 'rgba(255, 99, 132, 0.2)',
        fill: true,
    }]
};

const temperatureChart = new Chart(ctx, {
    type: 'line',
    data: temperatureData,
    options: {
        scales: {
            x: { 
                type: 'time',
                time: {
                    unit: 'minute'
                },
                title: {
                    display: true,
                    text: 'Temps'
                }
            },
            y: {
                beginAtZero: false,
                title: {
                    display: true,
                    text: 'Température (°C)'
                }
            }
        }
    }
});

socket.on('temperature_update', (data) => {
    console.log("Données reçues : ", data);

    const { temperature, timestamp } = data;

    temperatureChart.data.labels.push(new Date(timestamp));
    temperatureChart.data.datasets[0].data.push(temperature);

    if (temperatureChart.data.labels.length > 10) {
        temperatureChart.data.labels.shift();
        temperatureChart.data.datasets[0].data.shift();
    }

    temperatureChart.update();
});
