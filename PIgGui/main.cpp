#include "mainwindow.h"

#include <QApplication>

MainWindow *w = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString pattern = QString("%1 (%2%3%4%5%6): %{message} %7%8")
                          .arg("[%{time h:mm:ss.zzz}]")
                          .arg("%{if-debug}\033[32;1mD%{endif}")
                          .arg("%{if-info}\033[32;1mI%{endif}")
                          .arg("%{if-warning}\033[33;1mW%{endif}")
                          .arg("%{if-critical}\033[31;1mC%{endif}")
                          .arg("%{if-fatal}\033[31;1mF%{endif}")
                          .arg("\033[0m")
                          .arg("\n   Loc: [%{file}:%{line}]");

    qSetMessagePattern(pattern);

    w = new MainWindow();
    w->show();

    return a.exec();
}
