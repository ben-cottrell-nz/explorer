#include "ui_data.h"
#include "place_dir_pairs_abstract_model.h"
#include <QFile>

UIData::UIData(QObject *parent)
    : QObject{parent}
{

}

void UIData::setFileSelected(int id, bool selected)
{
    AbstractFileEntryModel* fm = abstract_file_entry_model_instance();
    FileSelection file_selection;
    if (selected) {
        file_selection.index_ = id;
        file_selection.filename_ = fm->filenameAt(id);
        file_selection.dir_ = fm->CurrentPath();
        selected_files_.append(file_selection);
        emit selectedFilesCountChanged(selected_files_.length());
    } else {
        selected_files_.removeIf([id](FileSelection s){
            return s.filename_ == abstract_file_entry_model_instance()
                    ->filenameAt(id) &&
                    s.dir_ == abstract_file_entry_model_instance()
                    ->CurrentPath();
        });
        emit selectedFilesCountChanged(selected_files_.length());
    }
    fm->setItemChecked(id, selected);
}

QVariant UIData::selectedFiles()
{
    QList<QString> files_list;
    for (auto l : selected_files_) {
        files_list.append(
                    QString("%1/%2").arg(
                        l.dir_, l.filename_));
    }
    return files_list;
}

QVariant UIData::getLocation()
{
    return place_dir_pairs_abstract_model_instance()->GetEntryItem(
                selected_place_id_
                ).directory_;
}

bool UIData::selectingMode()
{
    return selecting_mode_;
}

void UIData::setOperation(int value)
{
    if (value >= 0 && value <= 2) {
        last_op_ = (Op)value;
    }
}

int UIData::getOperation()
{
    return last_op_;
}

int UIData::selectedFilesCount()
{
    return selected_files_.length();
}

bool UIData::copySelectedFilesToCurrentPath()
{
    if (isCopyAllowed() && selected_files_.length() > 0) {
        AbstractFileEntryModel* fm = abstract_file_entry_model_instance();
        if (fm->FilesAlreadyExist(selected_files_)) {
            last_op_ = Op::COPY;
            emit overwriteApprovalRequested();
            return false;
        } else {
            fm->CopyFilesToCurrentPath(selected_files_);
            return true;
        }
    }
    return false;
}

bool UIData::moveSelectedFilesToCurrentPath()
{
    if (isCutAllowed() && selected_files_.length() > 0) {
        AbstractFileEntryModel* fm = abstract_file_entry_model_instance();
        if (fm->FilesAlreadyExist(selected_files_)) {
            last_op_ = Op::MOVE;
            emit overwriteApprovalRequested();
            return false;
        } else {
            fm->MoveFilesToCurrentPath(selected_files_);
            return true;
        }
    }
    return false;
}

bool UIData::overwriteApproved(bool value)
{
    if (value) {
        AbstractFileEntryModel* fm = abstract_file_entry_model_instance();
        switch (last_op_) {
        case Op::NONE:
            return false;
        case Op::COPY:
            fm->CopyFilesToCurrentPath(selected_files_);
            break;
        case Op::MOVE:
            fm->MoveFilesToCurrentPath(selected_files_);
            break;
        }
        last_op_ = Op::NONE;
        return true;
    } else {
        return false;
    }
}

void UIData::fileRenamed(QString dir, QString file_name)
{
    for (auto& f : selected_files_) {
        if (f.dir_ == dir) {
            f.filename_ = file_name;
        }
    }
}

bool UIData::isCutAllowed()
{
    return isCopyAllowed();
}

bool UIData::isCopyAllowed()
{
    if (selected_files_.length() > 0) {
        return true;
    } else {
        return false;
    }
}

bool UIData::isPasteAllowed()
{
    //check if files would exclusively
    //overwrite existing files in the current directory
    return abstract_file_entry_model_instance()
            ->FilesAlreadyExist(selected_files_)
            == false;
}

void UIData::clearSelectedFiles()
{
    selected_files_.clear();
}

int UIData::selectedPlaceId() const
{
    return selected_place_id_;
}

void UIData::setSelectedPlaceId(int newSelectedPlaceId)
{
    if (selected_place_id_ == newSelectedPlaceId)
        return;
    selected_place_id_ = newSelectedPlaceId;
    emit selectedPlaceIdChanged();
}

void UIData::setSelectingMode(bool newSelectingMode)
{
    selecting_mode_ = newSelectingMode;
    emit selectingModeChanged();
}

UIData *ui_data_instance()
{
    static UIData instance;
    return &instance;
}
