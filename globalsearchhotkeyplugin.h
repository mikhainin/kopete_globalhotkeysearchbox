#pragma once

#include <kopeteplugin.h>

class KAction;
class KXmlGuiWindow;
class KLineEdit;
class GlobalHotkeySearchboxPlugin : public Kopete::Plugin {
    Q_OBJECT
public:
    GlobalHotkeySearchboxPlugin(QObject* parent, const QVariantList& args);
    virtual ~GlobalHotkeySearchboxPlugin();


protected slots:
    void readyForUnload();
    void slotTrigged();
    void slotSearchReturnPressed();

private:
    KAction *m_actionShowMainWindowAndSelectSearchBox;
    KXmlGuiWindow *m_mainWindow;
    KLineEdit *m_searchBoxEdit;
};
