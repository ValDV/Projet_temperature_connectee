const express = require('express');
const socketIo = require('socket.io');
const sqlite3 = require('sqlite3').verbose();
const http = require('http');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

const db = new sqlite3.Database('./temperature_data.db');

db.run("CREATE TABLE IF NOT EXISTS temperature_readings (timestamp TEXT, temperature REAL)");

function generateRandomTemperature() {
  return (Math.random() * 12 + 18).toFixed(2);
}

setInterval(() => {
  const temperature = generateRandomTemperature();
  const timestamp = new Date().toISOString();

  db.run("INSERT INTO temperature_readings (timestamp, temperature) VALUES (?, ?)", [timestamp, temperature]);

  io.emit('temperature_update', { temperature, timestamp });

  console.log(`Température générée : ${temperature} °C à ${timestamp}`);
}, 5000);

app.use(express.static('public'));

io.on('connection', (socket) => {
  console.log('Client connecté au WebSocket');
  
  db.all("SELECT * FROM temperature_readings ORDER BY timestamp DESC LIMIT 1", (err, rows) => {
    if (rows.length > 0) {
      socket.emit('temperature_update', rows[0]);
    }
  });
});

const port = 3000;
server.listen(port, () => {
  console.log(`Serveur de test démarré sur http://192.168.65.211:${port}`);
});
