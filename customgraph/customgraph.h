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
    float getFirstPoint() {return first_point;}
    float getSecondPoint() {return second_point;}
    virtual ~CustomGraph();

private:
    float first_point{0}, second_point{0};

private slots:
    void mousePress(QMouseEvent * event);
    void mouseWheel();
    void mouseMove(QMouseEvent * event);
    void selectionChanged(const QCPDataSelection &selection);
};

#endif // CUSTOMGRAPH_H
