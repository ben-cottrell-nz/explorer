#include "abstract_file_entry_model.h"
#include <QDir>
#include <QDebug>
#if defined Q_OS_ANDROID
#include <QJniEnvironment>
#include <QJniObject>
#include <QCoreApplication>
#elif defined Q_OS_LINUX || defined Q_OS_WINDOWS
#include <QDesktopServices>
#include <QUrl>
#endif
#include <QMimeDatabase>
#include "ui_data.h"
#include <QList>

QHash<int, QByteArray> AbstractFileEntryModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[FileName] = "fileName";
  roles[FileSize] = "fileSize";
  roles[IsDirectory] = "isDirectory";
  roles[IsChecked] = "isChecked";
  roles[IsRenaming] = "isRenaming";
  return roles;
}
//Based on code from https://stackoverflow.com/a/47854799/3924272
qint64 dirSize(QString dirPath) {
  qint64 size = 0;
  QDir dir(dirPath);
  //calculate total size of current directories' files
  QDir::Filters fileFilters = QDir::Files|QDir::System|QDir::Hidden;
  for(QString filePath : dir.entryList(fileFilters)) {
    QFileInfo fi(dir, filePath);
    size+= fi.size();
  }
  //add size of child directories recursively
  QDir::Filters dirFilters = QDir::Dirs|QDir::NoDotAndDotDot|QDir::System|QDir::Hidden;
  for(QString childDirPath : dir.entryList(dirFilters))
    size+= dirSize(dirPath + QDir::separator() + childDirPath);
  return size;
}
int AbstractFileEntryModel::rowCount(const QModelIndex& parent = QModelIndex()) const {
  return entries_.size();
}
QVariant AbstractFileEntryModel::data(const QModelIndex& index, int role = Qt::DisplayRole) const {
  FileEntry currentEntry = entries_[index.row()];
  switch (role) {
    case FileName:
      return currentEntry.name_;
    case FileSize:
      return currentEntry.size_;
    case IsDirectory:
      return currentEntry.is_directory_;
    case IsChecked:
      return currentEntry.is_checked_;
    case IsRenaming:
      return currentEntry.is_renaming_;
  }
  return QVariant();
}
void AbstractFileEntryModel::append(FileEntry value) {
  entries_.append(value);
}
bool AbstractFileEntryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }
  FileEntry& currentEntry = entries_[index.row()];
  switch (role) {
    case FileName:
      currentEntry.name_ = value.toString();
      break;
    case FileSize:
      currentEntry.size_ = value.toReal();
      break;
    case IsDirectory:
      currentEntry.is_directory_ = value.toBool();
      break;
    case IsChecked:
      currentEntry.is_checked_ = value.toBool();
      break;
    case IsRenaming:
      currentEntry.is_renaming_ = value.toBool();
      break;
  }
  emit dataChanged(index, index, {role});
  emit allDataChanged();
  return true;
}
Qt::ItemFlags AbstractFileEntryModel::flags(const QModelIndex &index) const
{
  Q_UNUSED(index)
    return Qt::ItemIsEditable;
}

void AbstractFileEntryModel::setItemChecked(int index, bool checked)
{
    if (index > 0 && index < entries_.length()) {
        entries_[index].is_checked_ = checked;
    }
}
void AbstractFileEntryModel::openDirectory(QString path, bool isRelative) {
    ui_data_instance()->clearSelectedFiles();
  if (isRelative) {
    current_path_ = QFileInfo(current_path_ + '/' + path).canonicalFilePath();
  } else {
    current_path_ = QFileInfo(path).canonicalFilePath();
  }
  beginRemoveRows(QModelIndex(), 0, entries_.size() - 1);
  entries_.clear();
  endRemoveRows();
  QDir dirEntries(current_path_);
  //Excluding "." and ".." files
  beginInsertRows(QModelIndex(), 0, dirEntries.entryInfoList().size() - 3);
  for (auto file : dirEntries.entryInfoList()) {
    if (file.fileName() == "." || file.fileName() == "..") continue;
    FileEntry entry;
    entry.name_ = file.fileName();
    entry.size_ = file.size();
    entry.is_directory_ = file.isDir();
    entry.is_checked_ = false;
    entry.is_renaming_ = false;
    append(entry);
  }
  endInsertRows();
  emit currentPathChanged();
}
#if defined Q_OS_ANDROID
bool AbstractFileEntryModel::openFile(QString path)
{
  QString fullPath = QFileInfo(current_path_ + '/' + path).canonicalFilePath();
  using JniObj = QJniObject;
  QMimeDatabase mimeDb;
  QString mime = mimeDb.mimeTypeForFile(fullPath).name();
  bool isExternal = false;
  if (current_path_.startsWith("/storage/emulated/")) {
    isExternal = true;
  }
  qDebug () << QString("Opening %1 (%2)").arg(fullPath, mime);
  JniObj::callStaticMethod<jboolean>("com/bgcottrell/explorer/Utils",
                                     "foobar",
                                     "()Z");
  jboolean returnValue = JniObj::callStaticMethod<jboolean>("com/bgcottrell/explorer/Utils",
                                 "openFile",
                                 "(Landroid/app/Activity;ZLjava/lang/String;Ljava/lang/String;)Z",
                                 QNativeInterface::QAndroidApplication::context(),
                                 isExternal,
                                 JniObj::fromString(fullPath).object(),
                                 JniObj::fromString(mime).object());
  if (returnValue == JNI_FALSE) {
      qDebug() << "java method openFile returned false";
    return false;
  }
  return true;
}
#elif defined Q_OS_LINUX || defined Q_OS_WINDOWS
bool AbstractFileEntryModel::openFile(QString path)
{
    QDesktopServices::openUrl(QString("file://%1/%2").arg(current_path_,path));
    return false;
}
#endif
QString AbstractFileEntryModel::currentLocation()
{
  return current_path_;
}
void AbstractFileEntryModel::refresh()
{
  openDirectory(current_path_, false);
}
QString AbstractFileEntryModel::filenameAt(qint32 index)
{
    if (index >= 0 && index < entries_.length()) {
        return entries_.at(index).name_;
    }
    return "";
}
QString AbstractFileEntryModel::readableFileSize(quint64 byte_size)
{
  if (byte_size < 1e3) {
      return QString("%1 Bytes").arg(byte_size);
  } else if (byte_size > 1e3 && byte_size < 1e6) {
      return QString("%1 KB").arg(QString::number(byte_size * 0.001, 'f', 1));
  } else if (byte_size > 1e6 && byte_size < 1e9) {
      return QString("%1 MB").arg(QString::number(byte_size * 0.000001, 'f', 1));
  }
  return QString("%1 GB").arg(QString::number(byte_size * 0.000000001, 'f', 1));
}

void AbstractFileEntryModel::setItemIsRenaming(int index, bool value)
{
    if (index > 0 && index < entries_.length()) {
        entries_[index].is_renaming_ = value;
    }
}

void AbstractFileEntryModel::renameForSelectedFile()
{
    int i = 0;
    for (auto& f : entries_) {
        if (f.is_checked_) {
            f.is_renaming_ = true;
            emit allDataChanged();
            break;
        }
        i++;
    }
}

void AbstractFileEntryModel::update()
{
    emit dataChanged(index(0,0), index(entries_.length(),DisplayRoles::IsRenaming));
}

void AbstractFileEntryModel::renameFile(int index, QString file_name)
{
    if (index >= 0 && index < entries_.length()) {
        if (QFile::rename(QString("%1/%2").arg(
                              current_path_, entries_[index].name_),
                          QString("%1/%2").arg(
                              current_path_, file_name))) {
            entries_[index].name_ = file_name;
            emit allDataChanged();
            ui_data_instance()->fileRenamed(current_path_, file_name);
        }
    }
}

void AbstractFileEntryModel::CopyFilesToCurrentPath(QList<FileSelection>& files)
{

}

QString AbstractFileEntryModel::CurrentPath()
{
    return current_path_;
}

bool AbstractFileEntryModel::FilesAlreadyExist(QList<FileSelection>& files)
{
    auto file_iter = files.constBegin();
    for (auto f : entries_) {
        if (file_iter->filename_ == f.name_) {
            if (file_iter != files.cend()) {
                file_iter++;
            } else {
                return true;
            }
        }
    }
    return false;
}

AbstractFileEntryModel *abstract_file_entry_model_instance()
{
    static AbstractFileEntryModel instance;
    return &instance;
}

void AbstractFileEntryModel::MoveFilesToCurrentPath(QList<FileSelection>& files)
{

}
