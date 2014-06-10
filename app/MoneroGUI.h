#ifndef MONEROGUI_H
#define MONEROGUI_H

#include <QObject>

#include <QCoreApplication>
#include <QGuiApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "Models/MoneroModel.h"
#include "Models/WalletModel.h"
#include "Models/MinerModel.h"

#include "WalletSettings.h"

#include "WalletHandler.h"
#include "DaemonHandler.h"



class MoneroInterface;
class MinerInterface;
class WalletInterface;

/* TODO : Fix this class with more clean methods a states */
class MoneroGUI : public QObject
{
    Q_OBJECT
public:
    MoneroGUI(QGuiApplication& pApp);
    ~MoneroGUI();

    int start();

    int startWizard();
    int startMainWindow();

signals:
    void applicationQuit(int pReturnCode);

private:

    void initModels();
    void initInterfaces();

    bool createComponent(QQmlComponent& pComponent) {

        pComponent.create();
        if ( !pComponent.isReady() ) {
            qDebug() << "Component not ready";
            qDebug() << "Error " << pComponent.errors();
            return false;
        }

        return true;

    }

    bool initQmlElement(QQmlEngine& pEngine, const QUrl& pQmlFile) {

        QQmlComponent* lComponent = new QQmlComponent(&pEngine, pQmlFile);

        if ( !createComponent(*lComponent) ) {
            return false;
        }

        return true;

    }

    bool initMainWindow(QQmlEngine& pEngine) {

        return initQmlElement(pEngine, QUrl("qrc:/qml/main.qml"));
    }

    bool initWizard(QQmlEngine& pEngine) {

        return initQmlElement(pEngine, QUrl("qrc:/qml/wizard.qml"));
    }

    bool initSplashScreen(QQmlEngine& pEngine) {

        return initQmlElement(pEngine, QUrl("qrc:/qml/SplashScreen.qml"));
    }

    bool isReady() const {
        return settings.areSettingsAcceptable();
    }


    int startSplashScreen() {
        if ( initSplashScreen(engine) ) {

            splashscreen_running = true;
            app.exec();
            return 0;
        }

        return 1;
    }

    void stopSplashScreen();

private:

    void dialogError(int pErrorCode);

private:
    QGuiApplication& app;

    MoneroInterface* monero_interface;
    MinerInterface* miner_interface;
    WalletInterface* wallet_interface;

    MoneroModel monero_model;
    MinerModel miner_model;
    WalletModel wallet_model;

    WalletSettings settings;

    WalletHandler wallet_handler;

    QQmlEngine engine;

    bool splashscreen_running;

    int exit_status;



};

#endif // MONEROGUI_H
