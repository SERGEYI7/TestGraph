#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // dialog_history = QSharedPointer<DialogHistory>(new DialogHistory);
    undoStack = QSharedPointer<QUndoStack>(new QUndoStack(this));

    shortcut_back = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Ctrl+Z"), this));
    shortcut_forward = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Ctrl+Y"), this));
    connect(shortcut_back.data(), &QShortcut::activated, this, &MainWindow::take_step_back);
    connect(shortcut_forward.data(), &QShortcut::activated, this, &MainWindow::take_step_forward);

    left = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Left"), this));
    right = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Right"), this));
    up = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Up"), this));
    down = QSharedPointer<QShortcut>(new QShortcut(QKeySequence("Down"), this));
    connect(left.data(), &QShortcut::activated, this, &MainWindow::click_left);
    connect(right.data(), &QShortcut::activated, this, &MainWindow::click_right);
    connect(up.data(), &QShortcut::activated, this, &MainWindow::click_up);
    connect(down.data(), &QShortcut::activated, this, &MainWindow::click_down);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_MakeGraph_clicked()
{
    const QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (tabText == "Функция синуса") {
        undoStack->push(new UndoCommandAddSin(ui->customPlot, ui->spinBox_Amplitude->value(), ui->spinBox_Frequency->value(), undoStack.data()));
        // dialog_history->append(QDateTime::currentDateTime().toString() +
        //                        QString(" | Постороен график функции синуса: y = %1 * sin(PI/%2)").
        //                                                                  arg(ui->spinBox_Amplitude->value()).arg(ui->spinBox_Frequency->value()));
    }
    else if (tabText == "Другая функция") {
        undoStack->push(new UndoCommandAddParabola(ui->customPlot, ui->spinBox_XDG->value(), undoStack.data()));
        // dialog_history->append(QDateTime::currentDateTime().toString() +
        //                        QString(" | Постороен график фуyкции параболы: y = (x/%1)^2").arg(ui->spinBox_XDG->value()));
    }
}


void MainWindow::on_pushButton_calc_distance_clicked()
{
    ui->statusbar->showMessage(QString("Расстояние между точками: %1").arg(qFabs(ui->customPlot->getSecondPoint() - ui->customPlot->getFirstPoint())));
}

void MainWindow::take_step_back() {
    undoStack->undo();
    if (undoStack->index() == 0) {
        ui->customPlot->makeDefaultGraph(0);
    }
}

void MainWindow::take_step_forward() {
    undoStack->redo();
}

void MainWindow::on_action_triggered()
{
    dialog_history->show();
}

void MainWindow::click_left() {
    ui->customPlot->click_left();
}

void MainWindow::click_right() {
    ui->customPlot->click_right();
}

void MainWindow::click_up() {
    ui->customPlot->click_up();
}

void MainWindow::click_down() {
    ui->customPlot->click_down();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    ui->scrollArea->setMaximumHeight(this->size().height()/10);
}

void MainWindow::on_pushButton_minus_clicked()
{
    if (!ui->customPlot->graphCount() || ui->customPlot->graph(0)->dataCount() <= 1 )
        return;
    QVector<double> x, y;
    x.reserve(ui->customPlot->graph(0)->dataCount());
    y.reserve(ui->customPlot->graph(0)->dataCount());
    for (auto point : *(ui->customPlot->graph(0)->data())) {
        x.append(point.key-ui->doubleSpinBox_minus->value());
        y.append(point.value-ui->doubleSpinBox_minus->value());
    }
    undoStack->push(new UndoCommandAddFromData(ui->customPlot, std::move(x), std::move(y), undoStack.data()));
}


void MainWindow::on_pushButton_plus_clicked()
{
    if (!ui->customPlot->graphCount() || ui->customPlot->graph(0)->dataCount() <= 1 )
        return;
    QVector<double> x, y;
    x.reserve(ui->customPlot->graph(0)->dataCount());
    y.reserve(ui->customPlot->graph(0)->dataCount());
    for (auto point : *(ui->customPlot->graph(0)->data())) {
        x.append(point.key+ui->doubleSpinBox_minus->value());
        y.append(point.value+ui->doubleSpinBox_minus->value());
    }
    undoStack->push(new UndoCommandAddFromData(ui->customPlot, std::move(x), std::move(y), undoStack.data()));
}


void MainWindow::on_pushButton_divide_clicked()
{
    if (!ui->customPlot->graphCount() || ui->customPlot->graph(0)->dataCount() <= 1 )
        return;
    QVector<double> x, y;
    x.reserve(ui->customPlot->graph(0)->dataCount());
    y.reserve(ui->customPlot->graph(0)->dataCount());
    for (auto point : *(ui->customPlot->graph(0)->data())) {
        x.append(point.key/ui->doubleSpinBox_minus->value());
        y.append(point.value/ui->doubleSpinBox_minus->value());
    }
    undoStack->push(new UndoCommandAddFromData(ui->customPlot, std::move(x), std::move(y), undoStack.data()));
}


void MainWindow::on_pushButton_multiply_clicked()
{
    if (!ui->customPlot->graphCount() || ui->customPlot->graph(0)->dataCount() <= 1 )
        return;
    QVector<double> x, y;
    x.reserve(ui->customPlot->graph(0)->dataCount());
    y.reserve(ui->customPlot->graph(0)->dataCount());
    for (auto point : *(ui->customPlot->graph(0)->data())) {
        x.append(point.key*ui->doubleSpinBox_minus->value());
        y.append(point.value*ui->doubleSpinBox_minus->value());
    }
    undoStack->push(new UndoCommandAddFromData(ui->customPlot, std::move(x), std::move(y), undoStack.data()));
}

