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

void WeekReportModel::updateProjectIDCrossRefMap(const QMap<int, QString> &projectIDCrossRefMap)
{
   mProjectIDCrossRefMap = projectIDCrossRefMap;
}

int WeekReportModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)

   return mWeekReport.getDayCount();
}

QVariant WeekReportModel::data(const QModelIndex &index, int role) const
{
   if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
   {
      return QVariant();
   }

   if (role == WeekDayNameRole)
   {
      const DayReport dayReport = mWeekReport.getDayReport(index.row());
      return dayReport.getDayName();
   }
   else if (role == WeekReportDay)
   {
      if (mReportFormat == WeekReportTypes::WeekReportTypesEnum::Totals)
      {
         return mWeekReport.getDayReportTotals(index.row(), mProjectIDCrossRefMap);
      }
      else
      {
         return mWeekReport.getDayReportPunchIns(index.row());
      }
   }

   return QVariant();
}


QHash<int, QByteArray> WeekReportModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[WeekDayNameRole] = "WeekDayName";
   roles[DateRole] = "Date";
   roles[TotalWorkTimeDay] = "TotalWorkTimeDay";
   roles[WeekReportDay] = "WeekReportDay";
   return roles;
}

/**
 * Sets the report format this model should return (Totals or Hours)
 *
 * @param reportType the report type. @see WeekReportTypes::WeekReportTypesEnum
 */
void WeekReportModel::setReportFormat(WeekReportTypes::WeekReportTypesEnum reportFormat)
{
   if (reportFormat != mReportFormat)
   {
      mReportFormat = reportFormat;

      QModelIndex topLeft = this->index(0);
      QModelIndex bottomRight = this->index(rowCount()-1);
      emit dataChanged(topLeft, bottomRight);
   }
}
