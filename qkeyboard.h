#ifndef QKEYBOARD_H
#define QKEYBOARD_H

#include <QPushButton>
#include <QMap>
#include <QSignalMapper>
#include <QWidget>

#include "PIg/src/glbDefine.h"
#include "PIg/src/drivers/keyboard.h"

namespace Ui {
class QKeyboard;
}

class QKeyboard : public QWidget
{
    Q_OBJECT
    static const quint8 kMaxButtons = 8;
    const Qt::GlobalColor kBtnColorPrimary = Qt::black;
    const Qt::GlobalColor kBtnColorSecondary = Qt::darkRed;

public:
    explicit QKeyboard(QWidget *parent = nullptr);
    ~QKeyboard();

    void reset();
    void setType(eGB_TYPE_DEVICE device);
    eKEY getKey();

public slots:
    void btnPressed(int value);
    void keyPressed(int value);

private slots:
    void switchAlt();

signals:
    void clicked(int value);
    void debug(QString msg);

private:
    eKEY key = KEY_NO;
    bool alt = true;
    Ui::QKeyboard *ui;
    QSignalMapper *signalMapper;
    QVector<QPushButton*> btns;

    QMap<QPushButton*, eKEY> primary;
    QMap<QPushButton*, eKEY> secondary;

    void refresh();
    void setButtonColor(QColor color);
    void setButtonTextColor(QColor color);
    void setTypeK400();
    void setTypeOpto();
    void setTypeR400M();
    void setTypeRZSK();
    QString getButtonName(eKEY ) const;
};

#endif // QKEYBOARD_H
