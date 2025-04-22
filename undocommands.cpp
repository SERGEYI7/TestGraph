#include "undocommands.h"

UndoCommandAddSin::UndoCommandAddSin(CustomGraph* qcp, int amplitude, int frequency, const QUndoCommand* oldUndoCommand, const QUndoStack* undoStack, QUndoCommand * parent):
    QUndoCommand(parent), _amplitude(amplitude), _frequency(frequency), _qcp(qcp), _undoStack(undoStack)
{
    qcp->make_sin(_amplitude, _frequency);
    type = TypeGraph::Sin;
}

void UndoCommandAddSin::undo() {
    qDebug() << "undo sina";
    qDebug() << "undo index in undocommand" << _undoStack->index();
    qDebug() << "amplitude = " << get_amplitude();
    qDebug() << "frequency = " << get_frequency();
    const QUndoCommand * undoCmd = _undoStack->command(_undoStack->index()-2);
    if (dynamic_cast<const UndoCommandAddSin*>(undoCmd)) {
        const UndoCommandAddSin* newUndoCmd = dynamic_cast<const UndoCommandAddSin*>(undoCmd);
        _qcp->make_sin(newUndoCmd->get_amplitude(), newUndoCmd->get_frequency());
    }
    else if(dynamic_cast<const UndoCommandAddParabola*>(undoCmd)) {
        const UndoCommandAddParabola* newUndoCmd = dynamic_cast<const UndoCommandAddParabola*>(undoCmd);
        _qcp->makeDefaultGraph(newUndoCmd->get_cx());
    }
}

void UndoCommandAddSin::redo() {
    if (_undoStack->count() != _undoStack->index()) {
        _qcp->make_sin(get_amplitude(), get_frequency());
        return;
    }
    const QUndoCommand * undoCmd = _undoStack->command(_undoStack->index());
    if (dynamic_cast<const UndoCommandAddSin*>(undoCmd)) {
        const UndoCommandAddSin* newUndoCmd = dynamic_cast<const UndoCommandAddSin*>(undoCmd);
        _qcp->make_sin(newUndoCmd->get_amplitude(), newUndoCmd->get_frequency());
    }
    else if(dynamic_cast<const UndoCommandAddParabola*>(undoCmd)) {
        const UndoCommandAddParabola* newUndoCmd = dynamic_cast<const UndoCommandAddParabola*>(undoCmd);
        _qcp->makeDefaultGraph(newUndoCmd->get_cx());
    }
}

UndoCommandAddParabola::UndoCommandAddParabola(CustomGraph* qcp, int cx,const QUndoCommand* oldUndoCommand, const QUndoStack* undoStack, QUndoCommand * parent):
    QUndoCommand(parent), _cx(cx), _qcp(qcp), _undoStack(undoStack)
{
    // _qcp->makeDefaultGraph(_cx);
    type = TypeGraph::Parabola;
}

void UndoCommandAddParabola::undo() {
    qDebug() << "undo Parabola";
    qDebug() << "undo index in undocommand" << _undoStack->index();
    qDebug() << "cx = " << get_cx();
    qDebug() << "undostack count" << _undoStack->count();
    const QUndoCommand * undoCmd = _undoStack->command(_undoStack->index()-2);
    if (dynamic_cast<const UndoCommandAddSin*>(undoCmd)) {
        const UndoCommandAddSin* newUndoCmd = dynamic_cast<const UndoCommandAddSin*>(undoCmd);
        // qDebug() << "undo cmd -1 cx = " << get_cx();
        _qcp->make_sin(newUndoCmd->get_amplitude(), newUndoCmd->get_frequency());
    }
    else if(dynamic_cast<const UndoCommandAddParabola*>(undoCmd)) {
        const UndoCommandAddParabola* newUndoCmd = dynamic_cast<const UndoCommandAddParabola*>(undoCmd);
        qDebug() << "undocmd -1 cx = " << newUndoCmd->get_cx();
        _qcp->makeDefaultGraph(newUndoCmd->get_cx());
    }
}

void UndoCommandAddParabola::redo() {
    qDebug() << "redo Parabola";
    qDebug() << "redo Parabola count = " << _undoStack->count();
    qDebug() << "redo parabola in undo command parabola " << _undoStack->index();
    if (_undoStack->count() != _undoStack->index()) {
        _qcp->makeDefaultGraph(get_cx());
        return;
    }

    const QUndoCommand * undoCmd = _undoStack->command(_undoStack->index());
    if (dynamic_cast<const UndoCommandAddSin*>(undoCmd)) {
        const UndoCommandAddSin* newUndoCmd = dynamic_cast<const UndoCommandAddSin*>(undoCmd);
        _qcp->make_sin(newUndoCmd->get_amplitude(), newUndoCmd->get_frequency());
    }
    else if(dynamic_cast<const UndoCommandAddParabola*>(undoCmd)) {
        const UndoCommandAddParabola* newUndoCmd = dynamic_cast<const UndoCommandAddParabola*>(undoCmd);
        _qcp->makeDefaultGraph(newUndoCmd->get_cx());
    }
    else {
        _qcp->makeDefaultGraph(get_cx());
    }
}
