#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <customgraph.h>
#include "dialoghistory.h"
#include <QUndoStack>
#include "undocommands.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QSharedPointer<DialogHistory> dialog_history;
    void take_step_back();
    void take_step_forward();
    ~MainWindow();

private slots:
    void on_pushButton_MakeGraph_clicked();

    void on_pushButton_calc_distance_clicked();

    void on_action_triggered();

    void click_left();

    void click_right();

    void click_up();

    void click_down();

    void on_pushButton_minus_clicked();

    void on_pushButton_plus_clicked();

    void on_pushButton_divide_clicked();

    void on_pushButton_multiply_clicked();

private:
    Ui::MainWindow *ui;
    QSharedPointer<QShortcut> shortcut_back, shortcut_forward, left, right, up, down;
    QSharedPointer<QUndoStack> undoStack;

protected:
    void resizeEvent(QResizeEvent *event) override;

};
#endif // MAINWINDOW_H
