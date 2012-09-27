#ifndef SELECTNTHCONTACTACTION_H
#define SELECTNTHCONTACTACTION_H

#include <kaction.h>

class SelectNthContactAction : public KAction
{
    Q_OBJECT
public:
    explicit SelectNthContactAction(int n);
    virtual ~SelectNthContactAction();

    void setDefaultShortcut(const KShortcut& shortcut);

    void setShortcutFromConfig(const KShortcut& shortcut);

signals:

private slots:
    void slotTriggered();

private:
    int n;

    void setShortcut(const KShortcut& shortcut, ShortcutTypes type = ShortcutTypes(ActiveShortcut | DefaultShortcut));

    void setShortcut(const QKeySequence& shortcut, ShortcutTypes type = ShortcutTypes(ActiveShortcut | DefaultShortcut));

    void setShortcuts(const QList<QKeySequence> &shortcuts, ShortcutTypes type);

};

#endif // SELECTNTHCONTACTACTION_H
