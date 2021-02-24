#ifndef TAB_H
#define TAB_H

#include <QWidget>

namespace Ui {
class TTab;
}

class TTab : public QWidget
{
    Q_OBJECT

   public:
    explicit TTab(QWidget *parent = nullptr);
    ~TTab();

    bool set(QVariantMap &content);

    bool addItemToTree(QVariantMap &item);

   private:
    Ui::TTab *ui;

    bool parseContent(QVariantMap &content, QWidget *widget);
    bool parseDescription(QVariantMap &content, QWidget *widget);
    bool parseField(QVariantMap &content, QWidget *widget);
    bool parseName(QVariantMap &content, QWidget *widget);
    bool parseType(QVariantMap &content, QWidget *widget);
};

#endif // TAB_H
