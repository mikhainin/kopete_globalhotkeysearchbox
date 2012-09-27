#include <QVariantList>
#include <klocalizedstring.h>
#include "globalsearchhotkeyplugin.h"

#include "selectnthcontactaction.h"

SelectNthContactAction::SelectNthContactAction(int n)
    : KAction(GlobalHotkeySearchboxPlugin::self())
    , n(n)
{
    setText( i18n( "Open chat with contact #%1", n ) );
    setWhatsThis( i18n( "Open chat with contact #%d", n ) );
    setDefaultShortcut( KShortcut(Qt::ALT + (Qt::Key_0 + n)) );
    connect( this, SIGNAL(triggered()), SLOT(slotTriggered()) );
}


void SelectNthContactAction::setShortcut(const KShortcut& shortcut, ShortcutTypes type) {
    KAction::setShortcut(shortcut, type);
    GlobalHotkeySearchboxPlugin::self()->updateActionShortcutInConfig(n);
}

 SelectNthContactAction::~SelectNthContactAction() {
    GlobalHotkeySearchboxPlugin::self()->updateActionShortcutInConfig(n);
}

void SelectNthContactAction::setShortcut(const QKeySequence& shortcut, ShortcutTypes type) {
    KAction::setShortcut(shortcut, type);
    GlobalHotkeySearchboxPlugin::self()->updateActionShortcutInConfig(n);
}

void SelectNthContactAction::setShortcuts(const QList<QKeySequence> &shortcuts, ShortcutTypes type) {
   KAction::setShortcuts(shortcuts, type);
   GlobalHotkeySearchboxPlugin::self()->updateActionShortcutInConfig(n);
}

void SelectNthContactAction::slotTriggered() {
    GlobalHotkeySearchboxPlugin::self()->selectResult(n);
}

void SelectNthContactAction::setDefaultShortcut(const KShortcut& shortcut) {
    KAction::setShortcut(shortcut, DefaultShortcut);
}

void SelectNthContactAction::setShortcutFromConfig(const KShortcut& shortcut) {
    KAction::setShortcut(shortcut, ActiveShortcut);
}
