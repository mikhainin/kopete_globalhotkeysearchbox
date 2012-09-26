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

    void slotSelectResult1();
    void slotSelectResult2();
    void slotSelectResult3();
    void slotSelectResult4();
    void slotSelectResult5();
    void slotSelectResult6();
    void slotSelectResult7();

private:
    KAction *m_actionShowMainWindowAndSelectSearchBox;
    KAction *m_selectResult1;
    KAction *m_selectResult2;
    KAction *m_selectResult3;
    KAction *m_selectResult4;
    KAction *m_selectResult5;
    KAction *m_selectResult6;
    KAction *m_selectResult7;

    KXmlGuiWindow *m_mainWindow;
    KLineEdit *m_searchBoxEdit;

private:
    void selectResult(int n);
};
