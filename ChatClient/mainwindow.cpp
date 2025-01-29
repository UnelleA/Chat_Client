#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> // Pour afficher des messages d'erreur

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), socket(new QTcpSocket(this)) {
    ui->setupUi(this);

    // Connexion des boutons aux slots appropriés
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    // Connexion de la socket aux slots pour gérer les événements réseau
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::receiveMessage);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &MainWindow::handleSocketError);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Slot : Établir une connexion au serveur
void MainWindow::connectToServer() {
    QString ip = ui->ipLineEdit->text();        // Récupérer l'adresse IP entrée
    quint16 port = ui->portLineEdit->text().toUShort(); // Récupérer le port entré

    // Vérifier si l'utilisateur a bien rempli les champs
    if (ip.isEmpty() || port == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse IP et un port valides.");
        return;
    }

    // Connecter la socket au serveur
    socket->connectToHost(ip, port);

    // Attendre la connexion pendant 3 secondes
    if (socket->waitForConnected(3000)) {
        ui->messagesTextEdit->append("Connecté au serveur.");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter au serveur.");
    }
}

// Slot : Envoyer un message au serveur
void MainWindow::sendMessage() {
    QString message = ui->messageLineEdit->text(); // Récupérer le message à envoyer

    // Vérifier si le message n'est pas vide
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le message ne peut pas être vide.");
        return;
    }

    // Envoyer le message au serveur
    socket->write(message.toUtf8());
    socket->flush();

    // Afficher le message dans la zone de texte locale
    ui->messagesTextEdit->append("Moi : " + message);

    // Effacer le champ de saisie du message
    ui->messageLineEdit->clear();
}

// Slot : Recevoir un message du serveur
void MainWindow::receiveMessage() {
    // Lire les données reçues
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);

    // Afficher le message dans la zone de texte
    ui->messagesTextEdit->append("Serveur : " + message);
}

// Slot : Gérer les erreurs de la socket
void MainWindow::handleSocketError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    QMessageBox::critical(this, "Erreur de connexion", socket->errorString());
}
