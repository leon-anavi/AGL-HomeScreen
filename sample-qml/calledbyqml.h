#ifndef CALLEDBYQML_H
#define CALLEDBYQML_H
#include <QObject>

class CalledByQml : public QObject
{
    Q_OBJECT

public:
    explicit CalledByQml(QObject *parent = 0);
    Q_INVOKABLE int consoleout();
    Q_INVOKABLE int showup();

signals:

public slots:
    void refresh() {
        qDebug("Called the C++ slot");
    }
};

#endif // CALLEDBYQML_H
