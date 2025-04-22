#ifndef DIALOGHISTORY_H
#define DIALOGHISTORY_H

#include <QDialog>

namespace Ui {
class DialogHistory;
}

class DialogHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHistory(QWidget *parent = nullptr);
    void append(const QString & text);
    ~DialogHistory();

private slots:
    void on_pushButton_Clear_clicked();

private:
    Ui::DialogHistory *ui;
};

#endif // DIALOGHISTORY_H
