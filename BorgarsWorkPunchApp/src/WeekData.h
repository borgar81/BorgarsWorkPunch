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
      QDate mStartDate;
      QDate mEndDate;

   public:
      WeekData(int weekNumber, const QDate &startDate, const QDate &endDate);

      QString toString() const;

      static QDate getCurrentWeekStartDate();
      static QDate getCurrentWeekEndDate();

      // Get methods
      int getWeekNumber() const { return mWeekNumber; }
      QDate getStartDate() const { return mStartDate; }
      QDate getEndDate() const { return mEndDate; }
};


#endif // WEEKDATA_H
