#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QTimer>
#include <QScrollBar>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    connect(ui->textEdit, &QTextEdit::selectionChanged,
            this, &MainWindow::clearSelection);

    initParam();

    protBSPs = new clProtocolBspS(bspBuf, sizeof(bspBuf), &menu.sParam);
    protBSPs->setEnable(PRTS_STATUS_NO);

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME –азобратьс€ почему константы именно такие
    ui->textEdit->setFixedSize(size.width() + 12, size.height()*7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());
    ui->debug->setFixedWidth(ui->textEdit->width());
    ui->debug->setMinimumHeight(300);
    setFixedSize(sizeHint());

    // ”дал€ет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);

    connect(ui->kbd, &QKeyboard::debug, this, &MainWindow::printDebug);
    connect(ui->bsp, &Bsp::debug, this, &MainWindow::printDebug);
    connect(this, &MainWindow::userChanged, ui->bsp, &Bsp::setUser);
    connect(ui->bsp, &Bsp::userChanged, this, &MainWindow::setUser);

    QTimer *timerMenu = new QTimer(this);
    connect(timerMenu, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timerMenu->start(100);
}

//
MainWindow::~MainWindow()
{
    delete ui;
}

//
void MainWindow::clearSelection()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.clearSelection();
    ui->textEdit->setTextCursor(c);
}

//
void MainWindow::cycleMenu()
{
    static uint8_t menucnt = 0;
    static TUser::user_t user = TUser::MAX;

    uartRead();

    menucnt = (menucnt >= 2) ? (menu.proc(), 0) : menucnt + 1;
    vLCDled();

    uartWrite();

    if (user != menu.sParam.security.User.get()) {
        user = menu.sParam.security.User.get();
        emit userChanged(user);
    }
}

//
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyRelease) {
        // ” элементов созданных на вкладке Bsp имен нет.
        if (!focusWidget()->objectName().isEmpty()) {
            ui->kbd->keyPressed(static_cast<QKeyEvent*>(event)->key());
        }
    }

    return QMainWindow::eventFilter(object, event);
}

//
void MainWindow::initParam()
{

}

//
void MainWindow::printDebug(QString msg)
{
    ui->debug->append(msg);
}

//
void MainWindow::setBacklight(bool enable)
{
    QColor color = enable ? Qt::green : Qt::gray;

    if (color.isValid()) {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->textEdit->setStyleSheet(qss);
    }
}

void MainWindow::setUser(int value)
{
    menu.sParam.security.User.set(static_cast<TUser::user_t> (value));
}

//
void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    ui->bsp->initParam();
}

void MainWindow::uartRead()
{
    QVector<uint8_t> pkg = ui->bsp->receiveFromBsp();
    for(uint8_t byte: pkg) {
        protBSPs->checkByte(byte);
    }

    protBSPs->checkStat();
    if (protBSPs->getCurrentStatus() == PRTS_STATUS_READ_OK) {
        menu.setConnectionBsp(true);
        if (protBSPs->checkReadData()) {
            protBSPs->getData(false);
        }
    } else {
        qWarning() << "Read packet error!";
    }
}

void MainWindow::uartWrite()
{
    protBSPs->checkStat();
    if (protBSPs->getCurrentStatus() == PRTS_STATUS_NO) {
        eGB_COM com = menu.getTxCommand();
        if (com != GB_COM_NO) {
            uint16_t num = protBSPs->sendData(com);
            QVector<uint8_t> pkg;
            for(uint16_t i = 0; i < num; i++) {
                pkg.append(bspBuf[i]);
            }
            ui->bsp->sendToBsp(pkg);
            protBSPs->setCurrentStatus(PRTS_STATUS_NO);
        } else {
            qWarning() << "No send package!";
        }
    }
}

