#ifndef WEEKDATESMODEL_H
#define WEEKDATESMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QDate>

// Forward declarations
class WeekData;

/**
 * Model for displaying weeks
 */
class WeekDatesModel : public QAbstractListModel
{
   Q_OBJECT

   Q_PROPERTY(int weekCount READ getWeekCount NOTIFY weekCountChanged)
   Q_PROPERTY(int currentWeekNumber READ getCurrentWeekNumber NOTIFY currentWeekNumberChanged)

   signals:
      void weekCountChanged();
      void currentWeekNumberChanged();

   private:
      QVector <WeekData *> mWeekDataVector;

      /** The week number of the current week */
      int mCurrentWeekNumber;

      QHash<int, QByteArray> mRoleNames;

      void populateWeekDataVector(int weeksIntoThePast, int weeksIntoTheFuture);
      void clearWeekDataVector();

   public:
      enum RoleNames
      {
         WeekNumberRole = Qt::UserRole,
         StartDateRole,
         EndDateRole,
         ModelData
      };

      explicit WeekDatesModel(QObject *parent = 0);
      ~WeekDatesModel();

      // Overridden methods
      int rowCount(const QModelIndex &parent) const override;
      QVariant data(const QModelIndex &index, int role) const override;
      QHash<int, QByteArray> roleNames() const override;

      // QML Invokable Methods
      Q_INVOKABLE int getWeekNumber(int row) const;
      Q_INVOKABLE QDate getStartDate(int row) const;
      Q_INVOKABLE QDate getEndDate(int row) const;
      Q_INVOKABLE void refreshWeekData();

      Q_INVOKABLE int getIndexOfWeekNumber(int weekNumber);

      // Get and Set methods
      int getWeekCount() const;
      int getCurrentWeekNumber() const;
};

#endif // WEEKDATESMODEL_H
