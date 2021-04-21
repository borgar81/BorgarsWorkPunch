#pragma once

#include <QAbstractListModel>
#include "WeekReport.h"

class WeekReportModel : public QAbstractListModel
{
   Q_OBJECT

   private:
      WeekReport mWeekReport;

      QMap<QString, QString> mProjectIDCrossRefMap;

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

      void updateProjectIDCrossRefMap(QMap<QString, QString> projectIDCrossRefMap);

      int rowCount(const QModelIndex &parent = QModelIndex()) const override;
      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

      QHash<int, QByteArray> roleNames() const override;
};
