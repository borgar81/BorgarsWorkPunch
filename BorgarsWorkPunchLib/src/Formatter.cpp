// Qt Includes
#include <QLocale>

// Local Includes
#include "Formatter.h"


/**
* Converts the given integer to a string with the given number of digits.
*
* Example:
*           Integer value is    : 123
*           Number of digits is : 5
*           Prefix is           : #
*
*           Returned string is  : ##123
*
* @param value the integer value
* @param numDigits the number of digits.
* @param prefix the prefix used if the integer value is less than number of digits
*
* @return string containing the integer value with the given number of digits
*/
QString Formatter::intToString(long value, int numDigits, char prefix)
{
   QString str;
   QString tmpValueString;

   bool negativeNumber = (value < 0);

   tmpValueString = QString::number(qAbs(value));

   if(negativeNumber)
   {
      QLocale systemLocale = QLocale::system();

      str.append(systemLocale.negativeSign());
   }

   for (int i=0; i<(numDigits-tmpValueString.length()); i++)
   {
      str.append(prefix);
   }

   str.append(tmpValueString);

   return str;
}
