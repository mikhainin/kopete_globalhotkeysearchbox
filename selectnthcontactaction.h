#ifndef SELECTNTHCONTACTACTION_H
#define SELECTNTHCONTACTACTION_H

#include <kaction.h>

class SelectNthContactAction : public KAction
{
    Q_OBJECT
public:
    explicit SelectNthContactAction(int n);

signals:

private slots:
    void slotTriggered();

private:
    int n;

};

#endif // SELECTNTHCONTACTACTION_H
