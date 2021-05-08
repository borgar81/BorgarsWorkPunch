#pragma once

#include <QAbstractListModel>
#include "WeekReport.h"

class WeekReportModel : public QAbstractListModel
{
   Q_OBJECT

   Q_PROPERTY(WeekReportTypes::WeekReportTypesEnum reportFormat READ getReportFormat WRITE setReportFormat)

   private:
      WeekReport mWeekReport;

      QMap<int, QString> mProjectIDCrossRefMap;

      //WeekReportTypes::WeekReportTypesEnum mCurrentReportType {WeekReportTypes::Totals};

      /** The format to be displayed from this model (Total hours or PunchIns) */
      WeekReportTypes::WeekReportTypesEnum mReportFormat = WeekReportTypes::Totals;

public:
      enum Roles
      {
         WeekDayNameRole = Qt::UserRole + 1,
         DateRole,
         TotalWorkTimeDay,
         WeekReportDay,
         SizeRole
      };


      WeekReportModel(QObject *parent=nullptr);

      void updateWeekReport(const WeekReport &weekReport);

      void updateProjectIDCrossRefMap(const QMap<int, QString> &projectIDCrossRefMap);

      int rowCount(const QModelIndex &parent = QModelIndex()) const override;
      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

      QHash<int, QByteArray> roleNames() const override;

      //Q_INVOKABLE void setReportType(WeekReportTypes::WeekReportTypesEnum reportType);


      WeekReportTypes::WeekReportTypesEnum getReportFormat() const { return mReportFormat; }
      void setReportFormat(WeekReportTypes::WeekReportTypesEnum reportFormat);
};
