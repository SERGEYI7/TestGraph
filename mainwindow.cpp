#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog_history = QSharedPointer<DialogHistory>(new DialogHistory);
    undoStack = QSharedPointer<QUndoStack>(new QUndoStack(this));


    shortcut_back = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Ctrl+Z"), this));
    shortcut_forward = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Ctrl+Y"), this));

    connect(shortcut_back.data(), &QShortcut::activated, this, &MainWindow::take_step_back);
    connect(shortcut_forward.data(), &QShortcut::activated, this, &MainWindow::take_step_forward);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_MakeGraph_clicked()
{
    const QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (tabText == "Функция синуса") {
        qDebug() << "Undo sin stack current index before push -> " << undoStack->index();
        undoStack->push(new UndoCommandAddSin(ui->customPlot, ui->spinBox_Amplitude->value(), ui->spinBox_Frequency->value(), undoStack.data()));
        qDebug() << "Undo sin stack current index after push -> " << undoStack->index();
        dialog_history->append(QDateTime::currentDateTime().toString() +
                               QString(" | Постороен график функции синуса: y = %1 * sin(PI/%2)").
                                                                         arg(ui->spinBox_Amplitude->value()).arg(ui->spinBox_Frequency->value()));
    }
    else if (tabText == "Другая функция") {
        qDebug() << "Undo parabola stack current index before push -> " << undoStack->index();
        undoStack->push(new UndoCommandAddParabola(ui->customPlot, ui->spinBox_XDG->value(), undoStack.data()));
        qDebug() << "Undo parabola stack current index after push -> " << undoStack->index();
        dialog_history->append(QDateTime::currentDateTime().toString() +
                               QString(" | Постороен график фуyкции параболы: y = (x/%1)^2").arg(ui->spinBox_XDG->value()));
    }
}


void MainWindow::on_pushButton_calc_distance_clicked()
{
    qDebug() << "Чё посчитали: " << ui->customPlot->getSecondPoint() - ui->customPlot->getFirstPoint();
    ui->statusbar->showMessage(QString("Чё посчитали: %1").arg(ui->customPlot->getSecondPoint() - ui->customPlot->getFirstPoint()));
}

void MainWindow::take_step_back() {
    qDebug() << "take_step_back";
    undoStack->undo();
    if (undoStack->index() == 0) {
        ui->customPlot->makeDefaultGraph(0);
    }
}

void MainWindow::take_step_forward() {
    qDebug() << "take_step_forward";
    undoStack->redo();
}

void MainWindow::on_action_triggered()
{
    dialog_history->show();
}

