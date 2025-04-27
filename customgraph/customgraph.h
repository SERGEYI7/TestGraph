#ifndef CUSTOMGRAPH_H
#define CUSTOMGRAPH_H

#include "qcustomplot.h"

class CustomGraph : public QCustomPlot
{
    Q_OBJECT
public:
    CustomGraph(QWidget *parent = nullptr);
    QCustomPlot *customPlot;
    QCPItemLine *cursor_start;
    QCPItemLine *cursor_end;
    void makeDefaultGraph(int x);
    void make_sin(float amplitude, float frequency);
    void MakeFromData(QVector<double> x, QVector<double> y);
    float getFirstPoint() {return first_point;}
    float getSecondPoint() {return second_point;}
    virtual ~CustomGraph();

private:
    float first_point{0}, second_point{0};
    double x_range_lower{0}, x_range_upper{0}, y_range_lower{0}, y_range_upper{0};

private slots:
    void mousePress(QMouseEvent * event);
    void mouseWheel(QWheelEvent * event);
    void mouseMove(QMouseEvent * event);
    void selectionChanged(const QCPDataSelection &selection);
    void changeRangeX(const QCPRange &newRange, const QCPRange &oldRange);
    void changeRangeY(const QCPRange &newRange, const QCPRange &oldRange);

public slots:
    void click_left();
    void click_right();
    void click_up();
    void click_down();
};

#endif // CUSTOMGRAPH_H
