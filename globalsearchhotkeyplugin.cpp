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
{
    g_instance = this;

    m_mainWindow = qobject_cast<KXmlGuiWindow*>( Kopete::UI::Global::mainWidget() );

    m_actionShowMainWindowAndSelectSearchBox = new KAction ( i18n ( "Show contact list and select search box" ), this );
    m_mainWindow->actionCollection()->addAction("ShowMainWindowAndSelectSearchBox", m_actionShowMainWindowAndSelectSearchBox );

    m_actionShowMainWindowAndSelectSearchBox->setEnabled( true );
    m_actionShowMainWindowAndSelectSearchBox->setGlobalShortcut ( KShortcut ( ) );
    m_actionShowMainWindowAndSelectSearchBox->setWhatsThis ( i18n ( "Show contact list and select search box" ) );

    connect ( m_actionShowMainWindowAndSelectSearchBox, SIGNAL (triggered(bool)), this, SLOT (slotTrigged()) );

    connect ( this, SIGNAL (readyForUnload()), this, SLOT (readyForUnload()) );


    m_searchBoxEdit = m_mainWindow->toolBar("quickSearchBar")->findChild<KLineEdit*>();

    connect( m_searchBoxEdit, SIGNAL(returnPressed()), this, SLOT(slotSearchReturnPressed()) );


    for(int i = 1; i <= 9; ++i) {
        SelectNthContactAction *a = new SelectNthContactAction(i);
        m_mainWindow->actionCollection()->addAction("OpenChatWithContact" + QString::number(i), a );

    }
/*
    m_selectResult2 = new KAction( i18n ( "Open chat with contact #2" ), this );


    m_selectResult2->setEnabled(true);
    m_selectResult2->setShortcutConfigurable(true);
    m_selectResult2->setShortcut( KShortcut(Qt::Key_2 + Qt::ALT) );
    connect(m_selectResult2, SIGNAL(triggered()), this, SLOT(slotSelectResult2()));
    m_selectResult2->setWhatsThis ( i18n ( "Open chat with contact #2" ) );



    m_selectResult3 = new KAction( i18n ( "Open chat with contact #3" ), this );

    m_mainWindow->actionCollection()->addAction("OpenChatWithContact3", m_selectResult3 );

    m_selectResult3->setEnabled(true);
    m_selectResult3->setShortcutConfigurable(true);
    m_selectResult3->setShortcut( KShortcut(Qt::Key_3 + Qt::ALT) );
    connect(m_selectResult3, SIGNAL(triggered()), this, SLOT(slotSelectResult3()));
    m_selectResult3->setWhatsThis ( i18n ( "Open chat with contact #3" ) );



    m_selectResult4 = new KAction( i18n ( "Open chat with contact #4" ), this );

    m_mainWindow->actionCollection()->addAction("OpenChatWithContact4", m_selectResult4 );

    m_selectResult4->setEnabled(true);
    m_selectResult4->setShortcutConfigurable(true);
    m_selectResult4->setShortcut( KShortcut(Qt::Key_4 + Qt::ALT) );
    connect(m_selectResult4, SIGNAL(triggered()), this, SLOT(slotSelectResult4()));
    m_selectResult4->setWhatsThis ( i18n ( "Open chat with contact #4" ) );


    m_selectResult5 = new KAction( i18n ( "Open chat with contact #5" ), this );

    m_mainWindow->actionCollection()->addAction("OpenChatWithContact5", m_selectResult5 );

    m_selectResult5->setEnabled(true);
    m_selectResult5->setShortcutConfigurable(true);
    m_selectResult5->setShortcut( KShortcut(Qt::Key_5 + Qt::ALT) );
    connect(m_selectResult5, SIGNAL(triggered()), this, SLOT(slotSelectResult5()));
    m_selectResult5->setWhatsThis ( i18n ( "Open chat with contact #5" ) );
*/
}

GlobalHotkeySearchboxPlugin::~GlobalHotkeySearchboxPlugin()
{
    if (m_actionShowMainWindowAndSelectSearchBox) {
        delete m_actionShowMainWindowAndSelectSearchBox;
        m_actionShowMainWindowAndSelectSearchBox = 0;
    }
    g_instance = 0;
}

GlobalHotkeySearchboxPlugin *GlobalHotkeySearchboxPlugin::self() {
    return g_instance;
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

void GlobalHotkeySearchboxPlugin::slotSelectResult1() {
    selectResult(1);
}

void GlobalHotkeySearchboxPlugin::slotSelectResult2() {
    selectResult(2);
}

void GlobalHotkeySearchboxPlugin::slotSelectResult3() {
    selectResult(3);
}

void GlobalHotkeySearchboxPlugin::slotSelectResult4() {
    selectResult(4);
}

void GlobalHotkeySearchboxPlugin::slotSelectResult5() {
    selectResult(5);
}

void GlobalHotkeySearchboxPlugin::slotSelectResult6() {
    selectResult(6);
}

void GlobalHotkeySearchboxPlugin::slotSelectResult7() {
    selectResult(7);
}

#include "globalsearchhotkeyplugin.moc"
