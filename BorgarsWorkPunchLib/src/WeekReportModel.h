#pragma once

#include <QAbstractListModel>
#include "WeekReport.h"

class WeekReportModel : public QAbstractListModel
{
   Q_OBJECT

   private:
      WeekReport mWeekReport;

      QMap<int, QString> mProjectIDCrossRefMap;

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
};
