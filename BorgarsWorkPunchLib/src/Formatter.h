#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>

class Formatter
{
   public:
      static QString intToString(long value, int numDigits=-1, char prefix='0');
};

#endif // FORMATTER_H
