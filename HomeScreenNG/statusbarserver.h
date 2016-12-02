#ifndef STATUSBARSERVER_H
#define STATUSBARSERVER_H

#include <QtCore/QObject>

class StatusBarServer : public QObject
{
    Q_OBJECT
public:
    enum {
        SupportedCount = 3,
    };
    explicit StatusBarServer(QObject *parent = nullptr);
    ~StatusBarServer();

    Q_INVOKABLE QList<int> getAvailablePlaceholders() const;
    Q_INVOKABLE QString getStatusIcon(int placeholderIndex) const;
    Q_INVOKABLE QString getStatusText(int placeholderIndex) const;

public slots:
    void setStatusIcon(int placeholderIndex, const QString &icon);
    void setStatusText(int placeholderIndex, const QString &text);

signals:
    void statusIconChanged(int placeholderIndex, const QString &icon);
    void statusTextChanged(int placeholderIndex, const QString &text);

private:
    class Private;
    Private *d;
};

#endif // STATUSBARSERVER_H
