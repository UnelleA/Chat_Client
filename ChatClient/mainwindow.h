#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot pour établir une connexion au serveur
    void connectToServer();
    // Slot pour envoyer un message au serveur
    void sendMessage();
    // Slot pour gérer la réception des messages du serveur
    void receiveMessage();
    // Slot pour gérer les erreurs de la socket
    void handleSocketError(QAbstractSocket::SocketError socketError);

private:
    Ui::MainWindow *ui;       // Interface utilisateur générée par Qt Designer
    QTcpSocket *socket;       // Socket TCP utilisée pour communiquer avec le serveur
};

#endif // MAINWINDOW_H
