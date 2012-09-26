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
    setShortcut( KShortcut(Qt::ALT + (Qt::Key_0 + n)), DefaultShortcut );
    connect( this, SIGNAL(triggered()), SLOT(slotTriggered()) );
}

void SelectNthContactAction::slotTriggered() {
    GlobalHotkeySearchboxPlugin::self()->selectResult(n);
}
