#include <kdebug.h>
#include <kgenericfactory.h>
#include <kxmlguiwindow.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <kwindowsystem.h>
#include <ktoolbar.h>
#include <kfilterproxysearchline.h>
#include <klineedit.h>

// Kopete
#include <kopeteuiglobal.h>
#include <kopete/kopeteuiglobal.h>
#include <kopetechatsessionmanager.h>


#include "globalsearchhotkeyplugin.h"

K_PLUGIN_FACTORY( GlobalHotkeySearchboxPluginFactory, registerPlugin<GlobalHotkeySearchboxPlugin>(); )
K_EXPORT_PLUGIN( GlobalHotkeySearchboxPluginFactory( "kopete_globalhotkeysearchbox" ) )

GlobalHotkeySearchboxPlugin::GlobalHotkeySearchboxPlugin ( QObject* parent, const QVariantList& /* args */ )
        : Kopete::Plugin ( GlobalHotkeySearchboxPluginFactory::componentData(), parent )
        , m_actionShowMainWindowAndSelectSearchBox(0)
{

    m_mainWindow = qobject_cast<KXmlGuiWindow*>( Kopete::UI::Global::mainWidget() );

    m_actionShowMainWindowAndSelectSearchBox = new KAction ( i18n ( "Show contact list and select search box" ), this );
    m_mainWindow->actionCollection()->addAction("ShowMainWindowAndSelectSearchBox", m_actionShowMainWindowAndSelectSearchBox );

    m_actionShowMainWindowAndSelectSearchBox->setEnabled( true );
    m_actionShowMainWindowAndSelectSearchBox->setGlobalShortcut ( KShortcut ( ) );
    m_actionShowMainWindowAndSelectSearchBox->setWhatsThis ( i18n ( "Show contact list and select search box" ) );

    connect ( m_actionShowMainWindowAndSelectSearchBox, SIGNAL (triggered(bool)), this, SLOT (slotTrigged()) );

    connect ( this, SIGNAL (readyForUnload()), this, SLOT (readyForUnload()) );

}

GlobalHotkeySearchboxPlugin::~GlobalHotkeySearchboxPlugin()
{
    if (m_actionShowMainWindowAndSelectSearchBox) {
        delete m_actionShowMainWindowAndSelectSearchBox;
        m_actionShowMainWindowAndSelectSearchBox = 0;
    }
}

void GlobalHotkeySearchboxPlugin::slotTrigged() {
    // raise main window
    m_mainWindow->show();
    m_mainWindow->raise();
    KWindowSystem::forceActiveWindow( m_mainWindow->winId() );

    // set focus on line edit of "Quick search bar"
    m_mainWindow->toolBar("quickSearchBar")->show();
    KLineEdit *lineEdit = m_mainWindow->toolBar("quickSearchBar")->findChild<KLineEdit*>();
    lineEdit->setFocus();
    lineEdit->selectAll();
}

void GlobalHotkeySearchboxPlugin::readyForUnload()
{
    m_actionShowMainWindowAndSelectSearchBox->deleteLater();
    m_actionShowMainWindowAndSelectSearchBox = 0;
}

#include "globalsearchhotkeyplugin.moc"
