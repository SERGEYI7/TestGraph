#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include "customgraph.h"

class UndoMyBase : public QUndoCommand
{
public:
    UndoMyBase(const QUndoStack* undoStack, QUndoCommand * parent = nullptr) :QUndoCommand(parent), _undoStack(undoStack) {}
    virtual void make_graph() const = 0;
    virtual void step_back();
    virtual void step_forward();
    void undo() override {step_back();}
    void redo() override {step_forward();}
    virtual ~UndoMyBase() {}
private:
    const QUndoStack * _undoStack;
};

class UndoCommandAddSin : public UndoMyBase
{
public:
    UndoCommandAddSin(CustomGraph* qcp, int amplitude, int frequency, const QUndoStack* undoStack, UndoMyBase * parent = nullptr);
    virtual ~UndoCommandAddSin() {}

    virtual void make_graph() const override;
    int get_amplitude() const {return _amplitude;}
    int get_frequency() const {return _frequency;}

private:
    CustomGraph* _qcp;
    int _amplitude, _frequency;
};

class UndoCommandAddParabola : public UndoMyBase
{
public:
    UndoCommandAddParabola(CustomGraph* qcp, int cx, const QUndoStack* undoStack, UndoMyBase * parent = nullptr);
    virtual ~UndoCommandAddParabola() {}
    virtual void make_graph() const override;
    int get_cx() const {return _cx;}

private:
    CustomGraph* _qcp;
    int _cx;
};

#endif // UNDOCOMMANDS_H

