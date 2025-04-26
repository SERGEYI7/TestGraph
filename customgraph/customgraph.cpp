#include "customgraph.h"
#include <QtMath>

CustomGraph::CustomGraph(QWidget *parent) : QCustomPlot(parent) {
    customPlot = this;
    // ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
    //                                 QCP::iSelectLegend | QCP::iSelectPlottables );
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
    // makeGraph();
}

CustomGraph::~CustomGraph() {}

void CustomGraph::makeDefaultGraph(int cx) {
    qDebug() << "Новый дэфолт график, сх = " << cx;
    QVector<double> x(cx*2+1), y(cx*2+1);
    x.reserve(cx*2);
    y.reserve(cx*2);
    for (int i=-cx; i<=cx; ++i)
    {
        x[i+cx] = i;
        y[i+cx] = std::abs(x[i+cx]*x[i+cx]);
    }
    // create graph and assign data to it:
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    qDebug() << "-cx = " << -cx;
    qDebug() << "cx = " << cx;
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
    // connect(ui->customPlot->graph(0), static_cast<void (QCPGraph::*)()>(&QCPGraph::selectionChanged), this, &MainWindow::selectionChanged);

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
        // if (dataIndex >= 0 && dataIndex < customPlot->graph(0)->data()->size()) {
        //     QCPDataSelection selection;
        //     selection.addDataRange(QCPDataRange(dataIndex, dataIndex + 1));
        //     customPlot->graph(0)->setSelection(selection);
        //     customPlot->replot();
        // }

        // if an axis is selected, only allow the direction of that axis to be dragged
        // if no axis is selected, both directions may be dragged
        // qDebug() << "Mouse event";
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

        qDebug() << customPlot->graph()->dataMainKey(dataBeginIndex-1);
        qDebug() << customPlot->graph()->dataMainValue(dataBeginIndex-1);


        if(dataBeginIndex >= 0 && dataBeginIndex <= customPlot->graph(0)->data()->size())
        {
            qDebug() << dataBeginIndex << "X:" << x << "Y:" << y;
        }
        else
        {
            qDebug() << dataBeginIndex << "X:" << 0 << "Y:" << 0;
        }

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
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    // if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    //     customPlot->axisRect()->setRangeZoom(customPlot->xAxis->orientation());
    // else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    //     customPlot->axisRect()->setRangeZoom(customPlot->yAxis->orientation());
    // else
    //     customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);

    qDebug() << "Сработала колесо";
    customPlot->xAxis->setRangeLower(customPlot->xAxis->range().lower-x_range_upper/20);
    customPlot->xAxis->setRangeUpper(customPlot->xAxis->range().upper+x_range_upper/20);

    customPlot->yAxis->setRangeLower(customPlot->yAxis->range().lower-y_range_upper/20);
    customPlot->yAxis->setRangeUpper(customPlot->yAxis->range().upper+y_range_upper/20);
}

void CustomGraph::mouseMove(QMouseEvent * event)
{
    // QToolTip::showText(event->globalPos(), QString("X:%1, Y:%2").arg(event->pos().x()).arg(event->pos().y()));
    // Получаем координаты клика
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = customPlot->yAxis->pixelToCoord(event->pos().y());

    // Находим ближайшую точку
    int dataIndex = customPlot->graph(0)->findBegin(x);

    // Если точка найдена, выделяем её
    if (dataIndex >= 0 && dataIndex < customPlot->graph(0)->data()->size()) {
        const auto item = customPlot->graph(0)->data()->at(dataIndex);
        QToolTip::showText(event->globalPos(), QString("Key: %1, Valur: %2\nFind: %3").arg(item->key).arg(item->value).arg(dataIndex));
    }
}

void CustomGraph::selectionChanged(const QCPDataSelection &selection) {
    // QCPDataSelection selection = ui->customPlot->graph(0)->selection();
    for(QCPDataRange & dataRange: selection.dataRanges()) {
        QCPGraphDataContainer::const_iterator begin = customPlot->graph(0)->data()->at(dataRange.begin()); // get range begin iterator from index
        QCPGraphDataContainer::const_iterator end = customPlot->graph(0)->data()->at(dataRange.end()); // get range end iterator from index
        for (QCPGraphDataContainer::const_iterator it=begin; it!=end; ++it)
        {
            // iterator "it" will go through all selected data points, as an example, we calculate the value average
            qDebug() << "Key: " << it->key << ", value: " << it->value;

        }
    }
}

void CustomGraph::changeRangeX(const QCPRange &newRange, const QCPRange &oldRange) {

    qDebug() << "Изменился диапазон Х";
    if (newRange.lower < x_range_lower) {
        customPlot->xAxis->setRangeLower(x_range_lower);
        // customPlot->replot();
        // if (newRange.upper ) // Если не максимум то не трогаем, или как то так
        customPlot->xAxis->setRangeUpper(oldRange.upper);
    }
    if (newRange.upper > x_range_upper) {
        customPlot->xAxis->setRangeUpper(x_range_upper);
        // customPlot->replot();
        customPlot->xAxis->setRangeLower(oldRange.lower);
    }
}

void CustomGraph::changeRangeY(const QCPRange &newRange, const QCPRange &oldRange) {
    qDebug() << "Изменился диапазон У";
    if (newRange.lower < y_range_lower) {
        customPlot->yAxis->setRangeLower(y_range_lower);
        // customPlot->replot();
        customPlot->yAxis->setRangeUpper(oldRange.upper);
    }
    if (newRange.upper > y_range_upper) {
        customPlot->yAxis->setRangeUpper(y_range_upper);
        // customPlot->replot();
        customPlot->yAxis->setRangeLower(oldRange.lower);
    }
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
