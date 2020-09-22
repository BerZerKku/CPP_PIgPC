#include "mainwindow.h"

#include <QApplication>

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qSetMessagePattern("%{if-category}%{category}: %{endif}%{message}\n   Loc: [%{file}:%{line}]");
    qSetMessagePattern("[%{time h:mm:ss.zzz}](%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}): %{message}\n   Loc: [%{file}:%{line}]");

    w = new MainWindow();
    w->show();

    return a.exec();
}
