#ifndef CW_MAINFORM_H
#define CW_MAINFORM_H

#include <QWidget>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include "cw_jumbleframe.h"
#include "cw_settingsframe.h"

class CMainForm : public QWidget
{
    Q_OBJECT
public:
    explicit CMainForm(QWidget *parent = 0);
    
signals:
    
public slots:

private slots:
    void animToJumble();
    void animToSettings();
    void switchFullScreen(bool yes);

    void animationFinished();

    void resetSettings();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    QWidget        *m_currFrame;
    QWidget        *m_oldFrame;
    CJumbleFrame   *m_jumbleFrame;
    CSettingsFrame *m_settingsFrame;

    QParallelAnimationGroup *m_animGroup;
    int m_duration;
    QEasingCurve::Type m_curve;
};

#endif // CW_MAINFORM_H
