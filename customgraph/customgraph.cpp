#include "customgraph.h"
#include <QtMath>

CustomGraph::CustomGraph(QWidget *parent) : QCustomPlot(parent) {
    customPlot = this;
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                QCP::iSelectItems | QCP::iMultiSelect);
    customPlot->addGraph();
    customPlot->graph(0)->setSelectable(QCP::stSingleData);
    customPlot->setMultiSelectModifier(Qt::KeyboardModifier::ControlModifier);
    connect(customPlot, &QCustomPlot::mousePress, this, &CustomGraph::mousePress);
    connect(customPlot, &QCustomPlot::mouseWheel, this, &CustomGraph::mouseWheel);
    connect(customPlot, &QCustomPlot::mouseMove, this, &CustomGraph::mouseMove);
    connect(customPlot->xAxis, qOverload<const QCPRange &, const QCPRange &>(&QCPAxis::rangeChanged), this, &CustomGraph::changeRangeX);
    connect(customPlot->yAxis, qOverload<const QCPRange &, const QCPRange &>(&QCPAxis::rangeChanged), this, &CustomGraph::changeRangeY);
    cursor_start = new QCPItemLine(customPlot);
    cursor_start->start->setCoords(0, 0);
    cursor_start->end->setCoords(0, 0);
    cursor_end = new QCPItemLine(customPlot);
    cursor_end->start->setCoords(0, 0);
    cursor_end->end->setCoords(0, 0);
}

CustomGraph::~CustomGraph() {}

void CustomGraph::MakeFromData(QVector<double> x, QVector<double> y) {
    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    x_range_lower = *std::min_element(x.constBegin(), x.constEnd());
    x_range_upper = *std::max_element(x.constBegin(), x.constEnd());
    y_range_lower = *std::min_element(y.constBegin(), y.constEnd());;
    y_range_upper = *std::max_element(y.constBegin(), y.constEnd());
    customPlot->xAxis->setRange(x_range_lower, x_range_upper);
    customPlot->yAxis->setRange(y_range_lower, y_range_upper);
    connect(customPlot->graph(0),
            static_cast<void (QCPGraph::*)(const QCPDataSelection&)>(&QCPGraph::selectionChanged),
            this,
            &CustomGraph::selectionChanged);
    customPlot->replot();
}

void CustomGraph::makeDefaultGraph(int cx) {
    QVector<double> x(cx*2+1), y(cx*2+1);
    x.reserve(cx*2);
    y.reserve(cx*2);
    for (int i=-cx; i<=cx; ++i)
    {
        x[i+cx] = i;
        y[i+cx] = std::abs(x[i+cx]*x[i+cx]);
    }

    customPlot->graph(0)->setData(x, y);

    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    x_range_lower = -cx;
    x_range_upper = cx;
    y_range_lower = 0;
    y_range_upper = y.last();
    customPlot->xAxis->setRange(-cx, cx);
    customPlot->yAxis->setRange(0, y.last());
    connect(customPlot->graph(0),
            static_cast<void (QCPGraph::*)(const QCPDataSelection&)>(&QCPGraph::selectionChanged),
            this,
            &CustomGraph::selectionChanged);

    customPlot->replot();
}

void CustomGraph::make_sin(float amplitude, float frequency) {
    int size {100};
    float step {0.1};
    QVector<double> x_points, y_points;
    x_points.reserve(size);
    y_points.reserve(size);
    for (double i = 0; i <= M_PI*2; i += 0.01) {
        x_points.append(frequency*i/M_PI/2);
        y_points.append(amplitude*qSin(i));
    }
    customPlot->graph(0)->setData(x_points, y_points);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    x_range_lower = 0;
    x_range_upper = frequency;
    y_range_lower = -amplitude;
    y_range_upper = amplitude;
    customPlot->xAxis->setRange(0, frequency);
    customPlot->yAxis->setRange(-amplitude, amplitude);
    customPlot->replot();
}

void CustomGraph::mousePress(QMouseEvent * event)
{
    auto data = customPlot->graph(0)->data();
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    int dataBeginIndex = customPlot->graph(0)->findBegin(x, false);
    int dataEndIndex = customPlot->graph(0)->findEnd(x, false);

    if (event->button() == Qt::LeftButton && dataBeginIndex && dataEndIndex <= customPlot->graph(0)->dataCount()) {
        // Получаем координаты клика
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());

        // Находим ближайшую точку
        // Если точка найдена, выделяем её
        if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
            customPlot->axisRect()->setRangeDrag(customPlot->xAxis->orientation());
        else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
            customPlot->axisRect()->setRangeDrag(customPlot->yAxis->orientation());
        else
            customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);

        double start = customPlot->yAxis->pixelToCoord(0);
        double end = customPlot->yAxis->pixelToCoord(customPlot->size().height());
        if (dataEndIndex == customPlot->graph(0)->dataCount()) {
            x = customPlot->graph(0)->dataMainKey(dataEndIndex-1);
        }

        cursor_start->start->setCoords(x, start);
        cursor_start->end->setCoords(x, end);
        first_point = x;

        customPlot->replot();
    }
    else if (event->button() == Qt::RightButton && dataBeginIndex && dataEndIndex <= customPlot->graph(0)->dataCount()) {
        double start = customPlot->yAxis->pixelToCoord(0);
        double end = customPlot->yAxis->pixelToCoord(customPlot->size().height());
        if (dataEndIndex == customPlot->graph(0)->dataCount()) {
            x = customPlot->graph(0)->dataMainKey(dataEndIndex-1);
        }
        cursor_end->start->setCoords(x, start);
        cursor_end->end->setCoords(x, end);
        second_point = x;
        QPen pen_cursor;
        pen_cursor.setWidth(1);
        pen_cursor.setColor(Qt::GlobalColor::green);
        cursor_end->setPen(pen_cursor);
        customPlot->replot();
    }
}

void CustomGraph::mouseWheel(QWheelEvent * event)
{
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    customPlot->xAxis->setRangeLower(customPlot->xAxis->range().lower-x_range_upper/20);
    customPlot->xAxis->setRangeUpper(customPlot->xAxis->range().upper+x_range_upper/20);

    customPlot->yAxis->setRangeLower(customPlot->yAxis->range().lower-y_range_upper/20);
    customPlot->yAxis->setRangeUpper(customPlot->yAxis->range().upper+y_range_upper/20);
}

void CustomGraph::mouseMove(QMouseEvent * event)
{
    // Получаем координаты клика
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = customPlot->yAxis->pixelToCoord(event->pos().y());

    // Находим ближайшую точку
    int dataIndex = customPlot->graph(0)->findBegin(x);

    // Если точка найдена, выделяем её
    if (dataIndex >= 0 && dataIndex < customPlot->graph(0)->data()->size()) {
        const auto item = customPlot->graph(0)->data()->at(dataIndex);
        QToolTip::showText(event->globalPos(), QString("X: %1, Y: %2").arg(item->key).arg(item->value));
    }
}

void CustomGraph::selectionChanged(const QCPDataSelection &selection) {
    for(QCPDataRange & dataRange: selection.dataRanges()) {
        QCPGraphDataContainer::const_iterator begin = customPlot->graph(0)->data()->at(dataRange.begin()); // get range begin iterator from index
        QCPGraphDataContainer::const_iterator end = customPlot->graph(0)->data()->at(dataRange.end()); // get range end iterator from index
    }
}

void CustomGraph::changeRangeX(const QCPRange &newRange, const QCPRange &oldRange) {
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    if (newRange.lower < x_range_lower) {
        customPlot->xAxis->setRangeLower(x_range_lower);
        customPlot->xAxis->setRangeUpper(oldRange.upper);
    }
    if (newRange.upper > x_range_upper) {
        customPlot->xAxis->setRangeUpper(x_range_upper);
        customPlot->xAxis->setRangeLower(oldRange.lower);
    }
}

void CustomGraph::changeRangeY(const QCPRange &newRange, const QCPRange &oldRange) {
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    if (newRange.lower < y_range_lower) {
        customPlot->yAxis->setRangeLower(y_range_lower);
        customPlot->yAxis->setRangeUpper(oldRange.upper);
    }
    if (newRange.upper > y_range_upper) {
        customPlot->yAxis->setRangeUpper(y_range_upper);
        customPlot->yAxis->setRangeLower(oldRange.lower);
    }

    cursor_start->start->setCoords(cursor_start->start->key(), customPlot->yAxis->pixelToCoord(0));
    cursor_start->end->setCoords(cursor_start->start->key(), customPlot->yAxis->pixelToCoord(customPlot->size().height()));

    cursor_end->start->setCoords(cursor_end->start->key(), customPlot->yAxis->pixelToCoord(0));
    cursor_end->end->setCoords(cursor_end->start->key(), customPlot->yAxis->pixelToCoord(customPlot->size().height()));
}

void CustomGraph::click_left() {
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    const auto & currentRangeLower = customPlot->xAxis->range().lower;
    const auto & currentRangeUpper = customPlot->xAxis->range().upper;
    auto max_range = x_range_upper - x_range_lower;
    if (currentRangeLower == x_range_lower)
        return;
    else if (currentRangeLower - (max_range/20) < x_range_lower) {
        auto newUpper = currentRangeUpper - (x_range_lower-currentRangeLower);
        customPlot->xAxis->setRangeUpper(newUpper);
        customPlot->xAxis->setRangeLower(x_range_lower);
    }
    else {
        customPlot->xAxis->setRangeLower(currentRangeLower-(max_range/20));
        customPlot->xAxis->setRangeUpper(currentRangeUpper-(max_range/20));
    }
    customPlot->replot();
}

void CustomGraph::click_right() {
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    const auto & currentRangeLower = customPlot->xAxis->range().lower;
    const auto & currentRangeUpper = customPlot->xAxis->range().upper;
    auto max_range = x_range_upper - x_range_lower;
    if (currentRangeUpper == x_range_upper)
        return;
    else if (currentRangeUpper + (max_range/20) > x_range_upper) {
        customPlot->xAxis->setRangeLower(currentRangeLower+(x_range_upper-currentRangeUpper));
        customPlot->xAxis->setRangeUpper(x_range_upper);
    }
    else {
        customPlot->xAxis->setRangeLower(currentRangeLower+(max_range/20));
        customPlot->xAxis->setRangeUpper(currentRangeUpper+(max_range/20));
    }
    customPlot->replot();
}

void CustomGraph::click_up() {
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    const auto & currentRangeLower = customPlot->yAxis->range().lower;
    const auto & currentRangeUpper = customPlot->yAxis->range().upper;
    auto max_range = y_range_upper - y_range_lower;
    if (currentRangeUpper == y_range_upper)
        return;
    else if (currentRangeUpper + (max_range/20) > y_range_upper) {
        customPlot->yAxis->setRangeLower(currentRangeLower+(y_range_upper-currentRangeUpper));
        customPlot->yAxis->setRangeUpper(y_range_upper);
    }
    else {
        customPlot->yAxis->setRangeLower(currentRangeLower+(max_range/20));
        customPlot->yAxis->setRangeUpper(currentRangeUpper+(max_range/20));
    }
    customPlot->replot();
}

void CustomGraph::click_down() {
    if (!customPlot->graphCount() || customPlot->graph(0)->dataCount() <= 1)
        return;
    const auto & currentRangeLower = customPlot->yAxis->range().lower;
    const auto & currentRangeUpper = customPlot->yAxis->range().upper;
    auto max_range = y_range_upper - y_range_lower;
    if (currentRangeLower == y_range_lower)
        return;
    else if (currentRangeLower - (max_range/20) < y_range_lower) {
        auto newUpper = currentRangeUpper - (y_range_lower-currentRangeLower);
        customPlot->yAxis->setRangeUpper(newUpper);
        customPlot->yAxis->setRangeLower(y_range_lower);
    }
    else {
        customPlot->yAxis->setRangeLower(currentRangeLower-(max_range/20));
        customPlot->yAxis->setRangeUpper(currentRangeUpper-(max_range/20));
    }
    customPlot->replot();
}
