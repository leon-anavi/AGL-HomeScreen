#ifndef STATUSBARMODEL_H
#define STATUSBARMODEL_H

#include <QtCore/QAbstractListModel>

class StatusBarModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit StatusBarModel(QObject *parent = nullptr);
    ~StatusBarModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    class Private;
    Private *d;
};

#endif // STATUSBARMODEL_H
