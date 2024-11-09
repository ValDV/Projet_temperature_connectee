const express = require('express');
const socketIo = require('socket.io');
const mysql = require('mysql2');
const http = require('http');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

const db = mysql.createConnection({
    host: '127.0.0.1',
    user: 'user_temp',
    password: 'debian',
    database: 'temperature'
});

db.connect((err) => {
    if (err) {
        console.error('Erreur de connexion à la base de données :', err);
        return;
    }
    console.log('Connexion à la base de données réussie.');
});

function generateRandomTemperature() {
    return (Math.random() * 4 + 18).toFixed(2);
}

setInterval(() => {
    const temperature = generateRandomTemperature();
    const timestamp = new Date();

    db.query(
        "INSERT INTO temperature_readings (timestamp, temperature) VALUES (?, ?)",
        [timestamp, temperature],
        (err) => {
            if (err) {
                console.error("Erreur lors de l'insertion des données :", err);
            }
        }
    );

    io.emit('temperature_update', { temperature, timestamp });
    console.log(`Température générée : ${temperature} °C à ${timestamp}`);
}, 60000);

app.use(express.static('public'));

io.on('connection', (socket) => {
    console.log('Client connecté au WebSocket');

    db.query("SELECT * FROM temperature_readings ORDER BY timestamp DESC LIMIT 1", (err, rows) => {
        if (err) {
            console.error('Erreur lors de la récupération des données :', err);
            return;
        }
        if (rows.length > 0) {
            socket.emit('temperature_update', rows[0]);
        }
    });

    socket.on('get_temperature_range', (startDate, endDate) => {
        db.query(
            "SELECT * FROM temperature_readings WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp ASC",
            [startDate, endDate],
            (err, rows) => {
                if (err) {
                    console.error('Erreur lors de la récupération des données par plage :', err);
                    return;
                }
                socket.emit('temperature_range_update', rows);
            }
        );
    });
});

const port = 3000;
server.listen(port, () => {
    console.log(`Serveur de test démarré sur http://192.168.65.211:${port}`);
});
