#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> // Pour afficher des messages d'erreur

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), server(new QTcpServer(this)) {
    ui->setupUi(this);

    // Lancer le serveur sur mon port
    if (!server->listen(QHostAddress::Any, 149)) {
        QMessageBox::critical(this, "Erreur", "Le serveur n'a pas pu démarrer : " + server->errorString());
        close(); // Quitter si le serveur ne peut pas démarrer
        return;
    }

    // Connexion du signal newConnection du serveur à notre slot
    connect(server, &QTcpServer::newConnection, this, &MainWindow::handleNewConnection);

    //ui->messagesTextEdit->append("Serveur démarré sur le port 12345.");
}

MainWindow::~MainWindow() {
    delete ui;
}

// Slot : Gérer une nouvelle connexion client
void MainWindow::handleNewConnection() {
    QTcpSocket *clientSocket = server->nextPendingConnection(); // Récupérer le socket du nouveau client
    clients.append(clientSocket); // Ajouter à la liste des clients connectés

    // Afficher l'adresse du client connecté
    QString clientInfo = QString("Client connecté : %1:%2")
                             .arg(clientSocket->peerAddress().toString())
                             .arg(clientSocket->peerPort());
    ui->clientsTextEdit->append(clientInfo);

    // Connecter les signaux du client aux slots appropriés
    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::handleClientMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::handleClientDisconnection);

    // Informer tous les clients de la nouvelle connexion
    broadcastMessage(clientInfo);
}

// Slot : Gérer les messages entrants d'un client
void MainWindow::handleClientMessage() {
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender()); // Identifier le socket qui a envoyé le message
    if (!senderSocket) return;

    // Lire le message envoyé
    QByteArray data = senderSocket->readAll();
    QString message = QString::fromUtf8(data);

    // Afficher le message reçu dans la zone de texte
    QString displayMessage = QString("Message de %1:%2 - %3")
                                 .arg(senderSocket->peerAddress().toString())
                                 .arg(senderSocket->peerPort())
                                 .arg(message);
    ui->messagesTextEdit->append(displayMessage);

    // Diffuser le message à tous les autres clients
    broadcastMessage(message, senderSocket);
}

// Slot : Gérer la déconnexion d'un client
void MainWindow::handleClientDisconnection() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender()); // Identifier le client déconnecté
    if (!clientSocket) return;

    // Supprimer le client de la liste
    clients.removeOne(clientSocket);

    // Afficher un message indiquant la déconnexion
    QString clientInfo = QString("Client déconnecté : %1:%2")
                             .arg(clientSocket->peerAddress().toString())
                             .arg(clientSocket->peerPort());
    ui->clientsTextEdit->append(clientInfo);

    // Informer les autres clients de la déconnexion
    broadcastMessage(clientInfo);

    // Nettoyer le socket
    clientSocket->deleteLater();
}

// Méthode : Diffuser un message à tous les clients (sauf optionnellement un)
void MainWindow::broadcastMessage(const QString &message, QTcpSocket *excludeClient) {
    for (QTcpSocket *client : clients) {
        if (client != excludeClient) {
            client->write(message.toUtf8());
            client->flush();
        }
    }
}
