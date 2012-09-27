#pragma once

#include <kopeteplugin.h>

class KAction;
class KXmlGuiWindow;
class KLineEdit;
class KConfig;
class KConfigGroup;
class SelectNthContactAction;

class GlobalHotkeySearchboxPlugin : public Kopete::Plugin {
    Q_OBJECT
public:
    GlobalHotkeySearchboxPlugin(QObject* parent, const QVariantList& args);
    virtual ~GlobalHotkeySearchboxPlugin();

    static GlobalHotkeySearchboxPlugin* self();

    void selectResult(int n);
    void updateActionShortcutInConfig(int n);
    QString getActionName(int n);




protected slots:
    void readyForUnload();
    void slotSearchLineReturnPressed();
    void slotSearchReturnPressed();

private:
    KAction *m_actionShowMainWindowAndSelectSearchBox;
    QList<SelectNthContactAction*> m_selectActions;

    KXmlGuiWindow *m_mainWindow;
    KLineEdit *m_searchBoxEdit;
    KConfig* m_config;
    KConfigGroup *m_configActionHotkeys;
private:
};
