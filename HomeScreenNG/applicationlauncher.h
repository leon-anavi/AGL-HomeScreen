#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H

#include <QtCore/QObject>

class ApplicationLauncher : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationLauncher(QObject *parent = nullptr);

public slots:
    void launch(const QString &application);
};

#endif // APPLICATIONLAUNCHER_H
