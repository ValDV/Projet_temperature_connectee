Le projet consiste à créer une sonde de température connectée qui sera capable de mesurer la température via une thermistance,
se connecter à un réseau WiFi et de publier les données au format JSON sur un broker MQTT.
Un backend Js récupérera les données et les stockera en base de données pour les afficher en temps réel sur une page web.
La sonde sera configurable via liaison série avec une interface Qt.

Etapes :
- Partie Arduino : assemblage du projet et code pour mesurer la température et envoyer les données;
- Partie Web : back qui récupère les données et les stocke, interface web qui les affiche en temps réel sur un graphique;
- Partie Qt : interface Qt qui permet de configurer la sonde et notamment le wifi.
