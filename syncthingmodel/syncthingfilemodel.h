#ifndef DATA_SYNCTHINGFILEMODEL_H
#define DATA_SYNCTHINGFILEMODEL_H

#include "./syncthingmodel.h"

#include <syncthingconnector/syncthingconnection.h>

#include <QFuture>
#include <QFutureWatcher>

#include <map>
#include <memory>

namespace Data {

class LIB_SYNCTHING_MODEL_EXPORT SyncthingFileModel : public SyncthingModel {
    Q_OBJECT
public:
    enum SyncthingFileModelRole {
        NameRole = SyncthingModelUserRole + 1,
        SizeRole,
        ModificationTimeRole,
        PathRole,
        Actions,
        ActionNames,
        ActionIcons
    };

    explicit SyncthingFileModel(SyncthingConnection &connection, const SyncthingDir &dir, QObject *parent = nullptr);
    ~SyncthingFileModel() override;

public Q_SLOTS:
    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(const QString &path) const;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    void triggerAction(const QString &action, const QModelIndex &index);

public:
    QString path(const QModelIndex &path) const;

Q_SIGNALS:
    void fetchQueueEmpty();

private Q_SLOTS:
    void handleConfigInvalidated() override;
    void handleNewConfigAvailable() override;
    void handleForkAwesomeIconsChanged() override;
    void handleBrightColorsChanged() override;
    void handleLocalLookupFinished();

private:
    void processFetchQueue(const QString &lastItemPath = QString());

private:
    using SyncthingItems = std::vector<std::unique_ptr<SyncthingItem>>;
    using LocalLookupRes = std::shared_ptr<std::map<QString, SyncthingItem>>;
    struct QueryResult : SyncthingConnection::QueryResult {
        QString forPath;
        QFuture<LocalLookupRes> localLookup;
        QPersistentModelIndex refreshedIndex;
        QueryResult &operator=(SyncthingConnection::QueryResult &&);
    };

    SyncthingConnection &m_connection;
    QString m_dirId;
    QString m_localPath;
    QStringList m_fetchQueue;
    QueryResult m_pendingRequest;
    QFutureWatcher<LocalLookupRes> m_localItemLookup;
    std::unique_ptr<SyncthingItem> m_root;
};

} // namespace Data

#endif // DATA_SYNCTHINGFILEMODEL_H
