#ifndef PLACE_DIR_PAIRS_H
#define PLACE_DIR_PAIRS_H

#include <QAbstractListModel>

/*
 * This defines an interface for the place dir pairs
 * abstract model.
 *
 * An directory is associated with a place name for each
 * group, which could be Devices, Common or Internet.
*/

class PlaceIdPair : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE QString getPlaceName();
    Q_INVOKABLE int getPlaceId();
    QString place_;
    int id_;
};

class PlaceDirPairsAbstractModel : public QAbstractListModel
{
Q_OBJECT
public:
    explicit PlaceDirPairsAbstractModel();
    enum Group {
        DEVICES,
        COMMON,
        INTERNET
    };

    enum DisplayRoles {
        Group,
        Place,
        Directory
    };
    struct Entry {
        int group_;
        QString place_;
        QString directory_;
    };

    Q_INVOKABLE QList<PlaceIdPair*> getGroupPlaces(int group);
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex& parent) const ;
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    Entry& GetEntryItem(int id);
private:
    QList<Entry> entries_;
    int selected_place_id_;
};

PlaceDirPairsAbstractModel* place_dir_pairs_abstract_model_instance();

#endif // PLACE_DIR_PAIRS_H
