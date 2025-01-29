# Chat_Client
# 💬 Application de Chat TCP avec Qt

## 📝 Description

Ce projet consiste à développer une **application de chat en temps réel** utilisant les **sockets TCP** avec **Qt**. L'application permet à **plusieurs utilisateurs** de communiquer via un **serveur central** qui distribue les messages aux clients connectés.

L'application est composée de **deux logiciels distincts** :
- **Le Client** : Permet aux utilisateurs d'envoyer et de recevoir des messages.
- **Le Serveur** : Gère les connexions des clients et distribue les messages envoyés.

---

## 🛠️ Technologies utilisées

- **Qt Framework** : Pour la création de l'interface graphique et la gestion des événements.
- **C++** : Langage utilisé pour implémenter la logique réseau.
- **QTcpSocket** : Pour gérer les communications en **TCP** côté client.
- **QTcpServer** : Pour gérer les connexions entrantes côté serveur.
- **Qt Designer** : Pour concevoir l'interface utilisateur.
