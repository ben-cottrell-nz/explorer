#ifndef ABSTRACT_FILE_ENTRY_MODEL_HXX
#define ABSTRACT_FILE_ENTRY_MODEL_HXX

#include <QAbstractListModel>
struct FileEntry {
  QString name_;
  qint64 size_;
  bool is_directory_;
  bool is_checked_;
};

class AbstractFileEntryModel : public QAbstractListModel
{
  Q_OBJECT
 public:
  explicit AbstractFileEntryModel(QObject* parent = 0) {

  }
  enum DisplayRoles {
    FileName = Qt::UserRole + 1,
    FileSize,
    IsDirectory,
    IsChecked
  };
  QHash<int, QByteArray> roleNames() const;
  int rowCount(const QModelIndex& parent) const ;
  QVariant data(const QModelIndex& index, int role) const;
  void append(const FileEntry value);
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  Q_INVOKABLE void openDirectory(QString path, bool isRelative);
  Q_INVOKABLE bool openFile(QString path);
  Q_INVOKABLE QString currentLocation();
  Q_INVOKABLE void refresh();
  Q_INVOKABLE QString filenameAt(qint32 index);
  Q_INVOKABLE QString readableFileSize(quint64 byteSize);
  Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged);
  QString currentPath();
signals:
  void currentPathChanged();

private:
  QList<FileEntry> entries_;
  QString current_path_;
};

AbstractFileEntryModel* abstract_file_entry_model_instance();

#endif // ABSTRACT_FILE_ENTRY_MODEL_HXX
