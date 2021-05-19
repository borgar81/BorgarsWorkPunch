#ifndef WEEKDATA_H
#define WEEKDATA_H

#include <QDate>

/**
 * Week data struct
 */
class WeekData
{
   private:
      int mWeekNumber;
      QDateTime mStartDate;
      QDateTime mEndDate;

   public:
      WeekData(int weekNumber, const QDateTime &startDate, const QDateTime &endDate);

      QString toString() const;

      static QDateTime getCurrentWeekStartDate();
      static QDateTime getCurrentWeekEndDate();

      // Get methods
      int getWeekNumber() const { return mWeekNumber; }
      QDateTime getStartDate() const { return mStartDate; }
      QDateTime getEndDate() const { return mEndDate; }
};


#endif // WEEKDATA_H
