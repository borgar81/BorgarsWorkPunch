// Qt Includes
#include <QVector>

// Local Includes
#include "WeekData.h"
#include "WeekDatesModel.h"

/**
 * Constructor
 *
 * @parent parent object
 */
WeekDatesModel::WeekDatesModel(QObject *parent)
   :QAbstractListModel(parent)
{
   // Set Role names
   mRoleNames[WeekNumberRole]    = "weekNumber";
   mRoleNames[StartDateRole]     = "startDate";
   mRoleNames[EndDateRole]       = "endDate";
   mRoleNames[ModelData]         = "modelData";

   refreshWeekData();
}

/**
 * Destructor
 */
WeekDatesModel::~WeekDatesModel()
{
   clearWeekDataVector();
}

/**
 * Deletes Project data
 */
void WeekDatesModel::clearWeekDataVector()
{
   if(!mWeekDataVector.isEmpty())
   {
      beginRemoveRows(QModelIndex(), 0, mWeekDataVector.size()-1);
      qDeleteAll(mWeekDataVector);
      mWeekDataVector.clear();
      endRemoveRows();
   }
}

void WeekDatesModel::refreshWeekData()
{
   clearWeekDataVector();
   populateWeekDataVector(12, 3);
}

int WeekDatesModel::getIndexOfWeekNumber(int weekNumber)
{
   int row = 0;
   foreach(WeekData *weekData, mWeekDataVector)
   {
      if(weekData->getWeekNumber() == weekNumber)
      {
         return row;
      }

      row++;
   }

   return -1;
}


/**
 * Populates the week data vector
 *
 * @param weeksIntoThePast number of weeks into the past
 * @param weeksIntoTheFuture number of weeks into the future
 */
void WeekDatesModel::populateWeekDataVector(int weeksIntoThePast, int weeksIntoTheFuture)
{
   QVector <WeekData *> tmpVector;

   QDateTime tmpDate = WeekData::getCurrentWeekEndDate();

   for(int i=0; i<weeksIntoThePast; i++)
   {
      QDateTime endDate = tmpDate;
      tmpDate = tmpDate.addDays(-6);
      QDateTime startDate = tmpDate;
      startDate.setTime(QTime(0, 0, 0));
      int weekNumber = startDate.date().weekNumber();

      WeekData *weekData = new WeekData(weekNumber, startDate, endDate);
      tmpVector.prepend(weekData);
      tmpDate = tmpDate.addDays(-1);
   }

   tmpDate = WeekData::getCurrentWeekStartDate();

   mCurrentWeekNumber = tmpDate.date().weekNumber();
   emit currentWeekNumberChanged();

   tmpDate = tmpDate.addDays(7);
   for(int i=0; i<weeksIntoTheFuture; i++)
   {
      QDateTime startDate = tmpDate;
      tmpDate = tmpDate.addDays(6);
      QDateTime endDate = tmpDate;
      endDate.setTime(QTime(23, 59, 59));

      int weekNumber = startDate.date().weekNumber();

      WeekData *weekData = new WeekData(weekNumber, startDate, endDate);
      tmpVector.append(weekData);
      tmpDate = tmpDate.addDays(1);
   }

   if(!tmpVector.isEmpty())
   {
      beginInsertRows(QModelIndex(), 0, tmpVector.size()-1);
      mWeekDataVector = tmpVector;
      endInsertRows();
   }
}

/**
 * Overridden
 *
 * @return Map of Role Names
 */
QHash<int, QByteArray> WeekDatesModel::roleNames() const
{
   return mRoleNames;
}

/**
 * Return the Week number for the given index
 *
 * @param row the index
 *
 * @return week number
 */
int WeekDatesModel::getWeekNumber(int row) const
{
   // TODO Add check?
   return data(this->index(row), WeekNumberRole).toInt();
}

/**
 * Return the Start Date for the given index
 *
 * @param row the index
 *
 * @return start date
 */
QDateTime WeekDatesModel::getStartDate(int row) const
{
   // TODO Add check?
   return data(this->index(row), StartDateRole).toDateTime();
}

/**
 * Return the End Date for the given index
 *
 * @param row the index
 *
 * @return end date
 */
QDateTime WeekDatesModel::getEndDate(int row) const
{
   // TODO Add check?
   return data(this->index(row), EndDateRole).toDateTime();
}


/**
 * @return the week count
 */
int WeekDatesModel::getWeekCount() const
{
   return rowCount(QModelIndex());
}

/**
 * @return the current week number
 */
int WeekDatesModel::getCurrentWeekNumber() const
{
   return mCurrentWeekNumber;
}

/**
 * Overridden. Returns the Number of rows in this model
 *
 * @param parent parent index. not used
 *
 * @return row count
 */
int WeekDatesModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return mWeekDataVector.size();
}

/**
 * Overridden. Returns the data for the given data at the given index
 *
 * @param index the index
 * @param role the role
 *
 * @return the data as QVariant
 */
QVariant WeekDatesModel::data(const QModelIndex &index, int role) const
{
   int row = index.row();

   // boundary check for the row
   if(row < 0 || row >= mWeekDataVector.size())
   {
     return QVariant();
   }

   WeekData *weekData = mWeekDataVector.at(row);

   if(role == WeekNumberRole)
   {
      return weekData->getWeekNumber();
   }
   else if(role == StartDateRole)
   {
      return weekData->getStartDate();
   }
   else if(role == EndDateRole)
   {
      return weekData->getEndDate();
   }
   else if(role == ModelData)
   {
      return weekData->toString();
   }

   return QVariant();
}

