#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot pour gérer une nouvelle connexion
    void handleNewConnection();
    // Slot pour gérer les messages entrants d'un client
    void handleClientMessage();
    // Slot pour gérer la déconnexion d'un client
    void handleClientDisconnection();

private:
    Ui::MainWindow *ui;          // Interface utilisateur générée par Qt Designer
    QTcpServer *server;          // Serveur TCP pour écouter les connexions
    QVector<QTcpSocket*> clients; // Liste des sockets des clients connectés

    // Méthode pour diffuser un message à tous les clients
    void broadcastMessage(const QString &message, QTcpSocket *excludeClient = nullptr);
};

#endif // MAINWINDOW_H
