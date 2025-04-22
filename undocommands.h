#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include "customgraph.h"

class UndoMyBase : public QUndoCommand
{
public:
    UndoMyBase(QUndoCommand * parent = nullptr) :QUndoCommand(parent) {}
    virtual void make_graph() const = 0;
    virtual void step_back() = 0;
    virtual void step_forward() = 0;
    void undo() override {step_back();}
    void redo() override {step_forward();}
    virtual ~UndoMyBase() {}
};

class UndoCommandAddSin : public UndoMyBase
{
public:
    UndoCommandAddSin(CustomGraph* qcp, int amplitude, int frequency, const QUndoStack* undoStack, UndoMyBase * parent = nullptr);
    virtual ~UndoCommandAddSin() {}

    virtual void make_graph() const override;
    void step_back() override;
    void undo() override;
    void step_forward() override;
    void redo() override;
    int get_amplitude() const {return _amplitude;}
    int get_frequency() const {return _frequency;}

private:
    CustomGraph* _qcp;
    int _amplitude, _frequency;
    int _old_amplitude {0};
    int _old_frequency {0};
    const UndoMyBase * _oldUndoCommand;
    const QUndoStack * _undoStack;
};

class UndoCommandAddParabola : public UndoMyBase
{
public:
    UndoCommandAddParabola(CustomGraph* qcp, int cx, const QUndoStack* undoStack, UndoMyBase * parent = nullptr);
    virtual ~UndoCommandAddParabola() {}

    virtual void make_graph() const override;
    void step_back() override;
    void undo() override;
    void step_forward() override;
    void redo() override;
    int get_cx() const {return _cx;}

private:
    CustomGraph* _qcp;
    int _cx;
    const UndoMyBase * _oldUndoCommand;
    const QUndoStack * _undoStack;
};

#endif // UNDOCOMMANDS_H

