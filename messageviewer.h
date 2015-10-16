#ifndef MESSAGEVIEWER_H

#define MESSAGEVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_messageviewer.h"
#include "wombatfunctions.h"

namespace Ui
{
    class MessageViewer;
}

class MessageViewer : public QDialog
{
    Q_OBJECT
        
public:
    MessageViewer(QWidget* parent = 0);
    ~MessageViewer();
    
private slots:
    void HideClicked();
signals:
    void HideMessageViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MessageViewer* ui;
};

Q_DECLARE_METATYPE(MessageViewer*);

#endif // MESSAGEVIEWER_H
