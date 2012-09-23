#pragma once

#include <kopeteplugin.h>

class KAction;
class KXmlGuiWindow;

class GlobalHotkeySearchboxPlugin : public Kopete::Plugin {
    Q_OBJECT
public:
    GlobalHotkeySearchboxPlugin(QObject* parent, const QVariantList& args);
    virtual ~GlobalHotkeySearchboxPlugin();


protected slots:
    void readyForUnload();
    void slotTrigged();

private:
    KAction *m_actionShowMainWindowAndSelectSearchBox;
    KXmlGuiWindow *m_mainWindow;
};
