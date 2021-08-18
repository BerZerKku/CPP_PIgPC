#ifndef QKEYBOARD_H
#define QKEYBOARD_H

#include <QMap>
#include <QPushButton>
#include <QSignalMapper>
#include <QWidget>

#include "PIg/src/drivers/keyboard.h"
#include "PIg/src/glbDefine.h"

namespace Ui
{
class QKeyboard;
}

class QKeyboard : public QWidget
{
    Q_OBJECT
    static const quint8   kMaxButtons        = 8;
    const Qt::GlobalColor kBtnColorPrimary   = Qt::black;
    const Qt::GlobalColor kBtnColorSecondary = Qt::darkRed;

public:
    explicit QKeyboard(QWidget *parent = nullptr);
    ~QKeyboard();

    void    reset();
    void    setLayoutButton(uint8_t number, eKEY key);
    uint8_t getKey();

public slots:
    void btnPressed(int value);
    void keyPressed(int value);

private slots:
    void switchAlt();

signals:
    void clicked(int value);
    void debug(QString msg);

private:
    uint8_t        key = KEY_NO;
    bool           alt = true;
    Ui::QKeyboard *ui;
    QSignalMapper *signalMapper;

    QVector<QPushButton *> btns;
    QVector<eKEY>          btnLayout;

    void    refresh();
    void    setButtonColor(QColor color);
    void    setButtonTextColor(QColor color);
    QString getButtonName(eKEY) const;
};

#endif  // QKEYBOARD_H
