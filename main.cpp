#include <QCoreApplication>
#include <QDebug>
#include "longnumber.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const char *X = "1685287499328328297814655639278583667919355849391453456921116729";
    const char *Y = "7114192848577754587969744626558571536728983167954552999895348492";

    const LongNumber x {X};
    const LongNumber y {Y};
    LongNumber suma = (x + y);
    LongNumber multipl = x * y;

    qDebug() << multipl.toStr();

    return a.exec();
}
