#include <QTreeView>

#include <kdebug.h>
#include <kgenericfactory.h>
#include <kxmlguiwindow.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <kwindowsystem.h>
#include <ktoolbar.h>
#include <kfilterproxysearchline.h>
#include <klineedit.h>
#include <KConfig>
#include <KConfigGroup>

// Kopete
#include <kopeteuiglobal.h>
#include <kopete/kopeteuiglobal.h>
#include <kopetechatsessionmanager.h>
#include <kopete/kopetecontactlist.h>
#include <kopete/kopetemetacontact.h>
#include <kopete/kopetecontact.h>


#include "selectnthcontactaction.h"
#include "globalsearchhotkeyplugin.h"

K_PLUGIN_FACTORY( GlobalHotkeySearchboxPluginFactory, registerPlugin<GlobalHotkeySearchboxPlugin>(); )
K_EXPORT_PLUGIN( GlobalHotkeySearchboxPluginFactory( "kopete_globalhotkeysearchbox" ) )

namespace {
    static GlobalHotkeySearchboxPlugin* g_instance = 0;
}

GlobalHotkeySearchboxPlugin::GlobalHotkeySearchboxPlugin ( QObject* parent, const QVariantList& /* args */ )
        : Kopete::Plugin ( GlobalHotkeySearchboxPluginFactory::componentData(), parent )
        , m_actionShowMainWindowAndSelectSearchBox(0)
        , m_searchBoxEdit(0)
        , m_config(0)
        , m_configActionHotkeys(0)
{
    g_instance = this;

    m_mainWindow = qobject_cast<KXmlGuiWindow*>( Kopete::UI::Global::mainWidget() );

    m_actionShowMainWindowAndSelectSearchBox = new KAction ( i18n ( "Show contact list and select search box" ), this );
    m_mainWindow->actionCollection()->addAction("ShowMainWindowAndSelectSearchBox", m_actionShowMainWindowAndSelectSearchBox );

    m_actionShowMainWindowAndSelectSearchBox->setEnabled( true );
    m_actionShowMainWindowAndSelectSearchBox->setGlobalShortcut ( KShortcut ( ) );
    m_actionShowMainWindowAndSelectSearchBox->setWhatsThis ( i18n ( "Show contact list and select search box" ) );

    connect ( m_actionShowMainWindowAndSelectSearchBox, SIGNAL (triggered(bool)), this, SLOT (slotSearchLineReturnPressed()) );

    connect ( this, SIGNAL (readyForUnload()), this, SLOT (readyForUnload()) );


    m_searchBoxEdit = m_mainWindow->toolBar("quickSearchBar")->findChild<KLineEdit*>();

    connect( m_searchBoxEdit, SIGNAL(returnPressed()), this, SLOT(slotSearchReturnPressed()) );


    m_config = new KConfig(GlobalHotkeySearchboxPluginFactory::componentData());
    m_configActionHotkeys = new KConfigGroup(m_config, "select_actions");

    for(int i = 1; i <= 9; ++i) {

        const QString currentActionName = getActionName(i);

        SelectNthContactAction *a = new SelectNthContactAction(i);
        m_mainWindow->actionCollection()->addAction( currentActionName, a );
        m_selectActions.append(a);

        if (m_configActionHotkeys->hasKey(getActionName(i))) {
            a->setShortcutFromConfig( KShortcut( m_configActionHotkeys->readEntry(currentActionName) ) );
        }

    }

}

GlobalHotkeySearchboxPlugin::~GlobalHotkeySearchboxPlugin()
{

    foreach(SelectNthContactAction *a, m_selectActions) {
        delete a;
    }
    m_selectActions.clear();

    g_instance = 0;

    if (m_actionShowMainWindowAndSelectSearchBox) {
        delete m_actionShowMainWindowAndSelectSearchBox;
        m_actionShowMainWindowAndSelectSearchBox = 0;
    }


    if (m_configActionHotkeys) {
        delete m_configActionHotkeys;
        m_configActionHotkeys = 0;
    }

    if (m_config) {
        delete m_config;
        m_config = 0;
    }

}

GlobalHotkeySearchboxPlugin *GlobalHotkeySearchboxPlugin::self() {
    return g_instance;
}

void GlobalHotkeySearchboxPlugin::slotSearchLineReturnPressed() {
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

void GlobalHotkeySearchboxPlugin::updateActionShortcutInConfig(int n) {
    const QString itemName = getActionName(n);

    kDebug() << itemName << m_selectActions[n - 1]->shortcut(KAction::ActiveShortcut).toString();

    m_configActionHotkeys->writeEntry( itemName, m_selectActions[n - 1]->shortcut(KAction::ActiveShortcut).toString() );
    m_configActionHotkeys->config()->sync();
}

QString GlobalHotkeySearchboxPlugin::getActionName(int n) {
    return "OpenChatWithContact" + QString::number(n);
}

void GlobalHotkeySearchboxPlugin::readyForUnload()
{
    m_actionShowMainWindowAndSelectSearchBox->deleteLater();
    m_actionShowMainWindowAndSelectSearchBox = 0;
}

void GlobalHotkeySearchboxPlugin::slotSearchReturnPressed() {
    selectResult(1);
}


void GlobalHotkeySearchboxPlugin::selectResult(int n) {

    Kopete::ContactList *list = Kopete::ContactList::self();
    QTreeView *tree = m_mainWindow->findChild<QTreeView*>();

    QItemSelectionModel *OldSelectionModel = tree->selectionModel();

    QModelIndex root = tree->rootIndex();

    int currentRootItem = 0;
    QModelIndex currentChild;
    while(n > 0) {

        currentChild = root.child(currentRootItem++,0);

        if (! currentChild.isValid() ) {
            return;
        }

        OldSelectionModel->select( currentChild, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );

        if ( list->selectedMetaContacts().size() == 0 ) {

            int currentSubIndex = 0;
            while(n > 0) {

                if ( ! currentChild.child(currentSubIndex, 0).isValid() ) {
                    break;
                }

                OldSelectionModel->select( currentChild.child(currentSubIndex++, 0),
                                           QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );

                --n;

            }
        } else {
            --n;
        }

    }


    if ( list->selectedMetaContacts().size() == 0 ) {
        return;
    }
    list->selectedMetaContacts()[0]->startChat();

    tree->setFocus();

}

#include "globalsearchhotkeyplugin.moc"
