#include "ServiceSystemUI.hpp"
#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);
    ServiceSystemUI w;
    w.show();

    return a.exec();
}
