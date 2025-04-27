#include "undocommands.h"

void UndoMyBase::step_back() {
    if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index()-2)))
        undoCmd->make_graph();
    else {
        qDebug() << "Неизвестный тип";
    }
}

void UndoMyBase::step_forward() {
    if (_undoStack->count() != _undoStack->index() || _undoStack->count() == 0) {
        make_graph();
        return;
    }

    if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index())))
        undoCmd->make_graph();
    else {
        qDebug() << "Неизвестный тип";
    }
}

UndoCommandAddSin::UndoCommandAddSin(CustomGraph* qcp, int amplitude, int frequency, const QUndoStack* undoStack, UndoMyBase * parent):
    UndoMyBase(undoStack, parent), _amplitude(amplitude), _frequency(frequency), _qcp(qcp)
{
    qcp->make_sin(_amplitude, _frequency);
}

void UndoCommandAddSin::make_graph() const {
    _qcp->make_sin(get_amplitude(), get_frequency());
}

UndoCommandAddParabola::UndoCommandAddParabola(CustomGraph* qcp, int cx, const QUndoStack* undoStack, UndoMyBase * parent):
    UndoMyBase(undoStack, parent), _cx(cx), _qcp(qcp)
{
    _qcp->makeDefaultGraph(get_cx());
}

void UndoCommandAddParabola::make_graph() const {
    _qcp->makeDefaultGraph(get_cx());
}

UndoCommandAddFromData::UndoCommandAddFromData(CustomGraph* qcp, QVector<double> x, QVector<double> y, const QUndoStack* undoStack, UndoMyBase * parent):
    UndoMyBase(undoStack, parent), _qcp(qcp), _x(x), _y(y)
{
    _qcp->MakeFromData(x, y);
}

void UndoCommandAddFromData::make_graph() const {
    _qcp->MakeFromData(get_vector_x(), get_vector_y());
}
