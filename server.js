const express = require('express');
const socketIo = require('socket.io');
const mysql = require('mysql2');
const http = require('http');
const fs = require('fs');
const mqtt = require('mqtt');

const config = JSON.parse(fs.readFileSync('config.json', 'utf8'));

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

const db = mysql.createConnection({
    host: config.db_host,
    user: config.db_user,
    password: config.db_password,
    database: config.db_database
});

db.connect((err) => {
    if (err) {
        console.error('Erreur de connexion à la base de données :', err);
        return;
    }
    console.log('Connexion à la base de données réussie.');
});

const mqttClient = mqtt.connect(`mqtt://${config.mqtt_host}:${config.mqtt_port}`);

mqttClient.on('connect', () => {
    console.log('Connecté au broker MQTT');
    mqttClient.subscribe(config.mqtt_topic, (err) => {
        if (err) {
            console.error('Erreur lors de la souscription au topic MQTT :', err);
        }
    });
});

mqttClient.on('message', (topic, message) => {
    console.log(`Message reçu sur ${topic} : ${message}`);
    try {
        const data = JSON.parse(message);

        const query = `INSERT INTO temperature_readings (timestamp, temperature) VALUES (?, ?)`;
        const timestamp = new Date();
        db.query(query, [timestamp, data.value], (err) => {
            if (err) {
                console.error("Erreur lors de l'insertion des données :", err);
                return;
            }
            console.log('Données insérées dans la base MySQL.');
        });

        io.emit('temperature_update', { temperature: data.value, timestamp });
    } catch (err) {
        console.error('Erreur lors du traitement des données MQTT :', err);
    }
});

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
    console.log(`Serveur démarré sur http://192.168.65.211:${port}`);
});