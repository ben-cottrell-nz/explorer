#include "ui_data.h"
#include "place_dir_pairs_abstract_model.h"

UIData::UIData(QObject *parent)
    : QObject{parent}
{

}

QString UIData::getLocation()
{
    return place_dir_pairs_abstract_model_instance()->GetEntryItem(
                selected_place_id_
                ).directory_;
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

UIData *ui_data_instance()
{
    static UIData instance;
    return &instance;
}
