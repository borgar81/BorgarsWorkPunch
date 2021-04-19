#include "WeekReportModel.h"

/**
 * @brief WeekReportModel::WeekReportModel
 * @param parent
 */
WeekReportModel::WeekReportModel(QObject *parent)
   :QAbstractListModel(parent)
{

}

void WeekReportModel::updateWeekReport(const WeekReport &weekReport)
{
   beginResetModel();

   mWeekReport = weekReport;

   endResetModel();
}

int WeekReportModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)

   return mWeekReport.getDayReportList().size();
}

QVariant WeekReportModel::data(const QModelIndex &index, int role) const
{
   if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
   {
      return QVariant();
   }

   if (role == WeekDayNameRole)
   {
      return mWeekReport.getDayReportList().at(index.row()).getDayName();
   }
   else if (role == WeekDayNameRole)
   {
      return mWeekReport.getFromDateTimeUTC().toLocalTime();
   }

   return QVariant();
}


QHash<int, QByteArray> WeekReportModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[WeekDayNameRole] = "WeekDayName";
   roles[DateRole] = "Date";
   roles[SizeRole] = "size";
   return roles;
}
