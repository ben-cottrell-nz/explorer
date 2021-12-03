#include "place_dir_pairs_abstract_model.h"
#include <QStandardPaths>
#include <QMap>
#include <QStorageInfo>

PlaceDirPairsAbstractModel::PlaceDirPairsAbstractModel()
{
    using paths = QStandardPaths;
    QMap<QString, paths::StandardLocation> common_path_name_map = {
        {"Home", paths::HomeLocation},
        {"Documents", paths::DocumentsLocation},
        {"Pictures", paths::PicturesLocation},
        {"Music", paths::MusicLocation},
         {"Movies", paths::MoviesLocation},
        {"Downloads", paths::DownloadLocation},
        {"Desktop", paths::DesktopLocation}
    };
    QMapIterator<QString, paths::StandardLocation> map_iterator(
            common_path_name_map);
    while (map_iterator.hasNext()) {
        map_iterator.next();
        entries_.append(Entry{
                            Group::COMMON,
                            map_iterator.key(),
                            paths::writableLocation(paths::StandardLocation(
                                map_iterator.value()))
                        });
    }
    for (auto &s : QStorageInfo::mountedVolumes()) {
        entries_.append(Entry {
                            Group::DEVICES,
                            s.displayName(),
                            s.rootPath()
                        });
    }
}

QList<PlaceIdPair*> PlaceDirPairsAbstractModel::getGroupPlaces(int group)
{
    QList<PlaceIdPair*> list;
    for (auto i = entries_.begin(); i != entries_.end(); i++) {
        if (i->group_ == group) {
            PlaceIdPair* p = new PlaceIdPair;
            p->setParent(this);
            p->place_ = i->place_;
            p->id_ = i - entries_.begin();
            list.append(p);
        }
    }
    return list;
}

QHash<int, QByteArray> PlaceDirPairsAbstractModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Group] = "group";
    roles[Directory] = "directory";
    roles[Place] = "place";
    return roles;
}

int PlaceDirPairsAbstractModel::rowCount(const QModelIndex &parent) const
{
    return entries_.count();
}

QVariant PlaceDirPairsAbstractModel::data(const QModelIndex &index, int role) const
{
    Entry current_entry = entries_[index.row()];
    switch (role) {
    case DisplayRoles::Directory:
        return current_entry.directory_;
    case DisplayRoles::Group:
        return current_entry.group_;
    case DisplayRoles::Place:
        return current_entry.place_;
    };
    return QVariant();
}

Qt::ItemFlags PlaceDirPairsAbstractModel::flags(const QModelIndex &index) const
{
    return Qt::ItemFlag::ItemIsEnabled;
}

PlaceDirPairsAbstractModel::Entry &PlaceDirPairsAbstractModel::GetEntryItem(int id)
{
    return entries_[id];
}

PlaceDirPairsAbstractModel* place_dir_pairs_abstract_model_instance()
{
    static PlaceDirPairsAbstractModel instance;
    return &instance;
}

QString PlaceIdPair::getPlaceName()
{
    return place_;
}

int PlaceIdPair::getPlaceId()
{
    return id_;
}
