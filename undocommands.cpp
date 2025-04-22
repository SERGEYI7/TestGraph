#include "undocommands.h"

UndoCommandAddSin::UndoCommandAddSin(CustomGraph* qcp, int amplitude, int frequency, const QUndoStack* undoStack, UndoMyBase * parent):
    UndoMyBase(parent), _amplitude(amplitude), _frequency(frequency), _qcp(qcp), _undoStack(undoStack)
{
    qcp->make_sin(_amplitude, _frequency);
}

void UndoCommandAddSin::make_graph() const {
    _qcp->make_sin(get_amplitude(), get_frequency());
}

void UndoCommandAddSin::step_back() {
    if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index()-2)))
        undoCmd->make_graph();
    else {
        qDebug() << "Неизвестный тип";
    }
}

void UndoCommandAddSin::step_forward() {
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

void UndoCommandAddSin::undo() {
    qDebug() << "undo sina";
    qDebug() << "undo index in undocommand" << _undoStack->index();
    qDebug() << "amplitude = " << get_amplitude();
    qDebug() << "frequency = " << get_frequency();
    // if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index()-2)))
    //     undoCmd->make_graph();
    // else {
    //     qDebug() << "Неизвестный тип";
    // }
    step_back();

}

void UndoCommandAddSin::redo() {
    // if (_undoStack->count() != _undoStack->index()) {
    //     _qcp->make_sin(get_amplitude(), get_frequency());
    //     return;
    // }
    // else if (_undoStack->count() == 0) {
    //     _qcp->make_sin(get_amplitude(), get_frequency());
    //     return;
    // }

    // if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index())))
    //     undoCmd->make_graph();
    // else {
    //     qDebug() << "Неизвестный тип";
    // }
    step_forward();
}

UndoCommandAddParabola::UndoCommandAddParabola(CustomGraph* qcp, int cx, const QUndoStack* undoStack, UndoMyBase * parent):
    UndoMyBase(parent), _cx(cx), _qcp(qcp), _undoStack(undoStack)
{
    _qcp->makeDefaultGraph(get_cx());
}

void UndoCommandAddParabola::make_graph() const {
    _qcp->makeDefaultGraph(get_cx());
}

void UndoCommandAddParabola::step_back() {
    if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index()-2)))
        undoCmd->make_graph();
    else {
        qDebug() << "Неизвестный тип";
    }
}

void UndoCommandAddParabola::step_forward() {
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

void UndoCommandAddParabola::undo() {
    qDebug() << "undo Parabola";
    qDebug() << "undo index in undocommand" << _undoStack->index();
    qDebug() << "cx = " << get_cx();
    qDebug() << "undostack count" << _undoStack->count();
    // if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index()-2)))
    //     undoCmd->make_graph();
    // else {
    //     qDebug() << "Не известный тип";
    // }
    step_back();
}

void UndoCommandAddParabola::redo() {
    qDebug() << "redo Parabola";
    qDebug() << "redo Parabola count = " << _undoStack->count();
    qDebug() << "redo parabola in undo command parabola " << _undoStack->index();
    // if (_undoStack->count() != _undoStack->index()) {
    //     _qcp->makeDefaultGraph(get_cx());
    //     return;
    // }
    // else if (_undoStack->count() == 0) {
    //         _qcp->makeDefaultGraph(get_cx());
    //         return;
    // }

    // if (const UndoMyBase * undoCmd = dynamic_cast<const UndoMyBase*>(_undoStack->command(_undoStack->index())))
    //     undoCmd->make_graph();
    // else {
    //     qDebug() << "Неизвестный тип";
    // }
    step_forward();
}
