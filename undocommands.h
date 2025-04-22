#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include "customgraph.h"

struct OldSin {
    int amplitude {0};
    int frequency {0};
};
static OldSin oldSin;

struct OldParabola {
    int cx {0};
};
static OldParabola oldParabola;

enum class TypeGraph {None, Sin, Parabola} static typeGraph {TypeGraph::None};

class UndoCommandAddSin : public QUndoCommand
{
public:
    UndoCommandAddSin(CustomGraph* qcp, int amplitude, int frequency, const QUndoCommand* oldUndoCommand, const QUndoStack* undoStack, QUndoCommand * parent = nullptr);
    virtual ~UndoCommandAddSin() {}

    void undo() override;
    void redo() override;
    int get_amplitude() const {return _amplitude;}
    int get_frequency() const {return _frequency;}

private:
    CustomGraph* _qcp;
    int _amplitude, _frequency;
    int _old_amplitude {0};
    int _old_frequency {0};
    const QUndoCommand * _oldUndoCommand;
    const QUndoStack * _undoStack;
    TypeGraph type {TypeGraph::None};
};

class UndoCommandAddParabola : public QUndoCommand
{
public:
    UndoCommandAddParabola(CustomGraph* qcp, int cx, const QUndoCommand* oldUndoCommand, const QUndoStack* undoStack, QUndoCommand * parent = nullptr);
    virtual ~UndoCommandAddParabola() {}

    void undo() override;
    void redo() override;
    int get_cx() const {return _cx;}

private:
    CustomGraph* _qcp;
    int _cx;
    const QUndoCommand * _oldUndoCommand;
    const QUndoStack * _undoStack;
    TypeGraph type {TypeGraph::None};
};

#endif // UNDOCOMMANDS_H
