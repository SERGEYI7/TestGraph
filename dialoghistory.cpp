#include "dialoghistory.h"
#include "ui_dialoghistory.h"

DialogHistory::DialogHistory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogHistory)
{
    ui->setupUi(this);
}

void DialogHistory::append(const QString & text) {
    ui->textEdit->append(text);
}

DialogHistory::~DialogHistory()
{
    delete ui;
}

void DialogHistory::on_pushButton_Clear_clicked()
{
    ui->textEdit->clear();
}

