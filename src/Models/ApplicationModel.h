#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>

class ApplicationModel : public QObject
{
    Q_OBJECT
public:
    ApplicationModel();
    ~ApplicationModel();

    Q_INVOKABLE void requestLaunchWizard() {
        emit onLaunchWizardRequested();
    }

    Q_INVOKABLE void requestApplicationQuit() {
        emit onApplicationQuitRequested();
    }

    Q_INVOKABLE void notifyWizardSuccess() {
        emit onWizardSuccess();
    }

signals:
    void onLaunchWizardRequested();
    void onApplicationQuitRequested();
    void onWizardSuccess();
};

#endif // APPLICATIONMODEL_H
