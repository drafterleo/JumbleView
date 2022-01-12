#include <QPainter>
#include <QtDebug>

#include "cw_mainform.h"

CMainForm::CMainForm(QWidget *parent) :
    QWidget(parent)
{
    m_settingsFrame = new CSettingsFrame(this);
    m_settingsFrame->readSettings();
    connect(m_settingsFrame, SIGNAL(needJumble()), SLOT(animToJumble()));
    connect(m_settingsFrame, SIGNAL(needQuit()), SLOT(close()));
    connect(m_settingsFrame, SIGNAL(needFullScreen(bool)), SLOT(switchFullScreen(bool)));

    m_jumbleFrame = new CJumbleFrame(this);
    m_jumbleFrame->acceptSettings(m_settingsFrame);
    connect(m_jumbleFrame, SIGNAL(needSettings()), SLOT(animToSettings()));
    connect(m_jumbleFrame, SIGNAL(needResetSettings()), SLOT(resetSettings()));

    m_currFrame = m_jumbleFrame;
    m_jumbleFrame->grabKeyboard();
    m_oldFrame = 0;
    m_settingsFrame->hide();

    m_animGroup = new QParallelAnimationGroup(this);
    m_duration = 300;
    m_curve = QEasingCurve::OutQuad;
    connect(m_animGroup, SIGNAL(finished()), SLOT(animationFinished()));

    resize(600, 600);
    setMinimumSize(600, 600);
}

void CMainForm::resizeEvent(QResizeEvent *event)
{
    if (m_animGroup->state() == QAbstractAnimation::Running) {
        m_animGroup->stop();
    }

    m_settingsFrame->hide();
    m_jumbleFrame->hide();
    m_currFrame->show();

    m_currFrame->setGeometry(this->rect());
}


void CMainForm::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    painter.end();
}


void CMainForm::animToJumble()
{
    m_jumbleFrame->acceptSettings(m_settingsFrame);
    m_jumbleFrame->grabKeyboard();

    m_oldFrame = m_settingsFrame;
    m_currFrame = m_jumbleFrame;
    m_jumbleFrame->resize(this->size());
    m_jumbleFrame->move(this->width(), 0);
    m_jumbleFrame->show();

    QPropertyAnimation *anim = new QPropertyAnimation(m_settingsFrame, "pos");
    anim->setStartValue(QPoint(0, 0));
    anim->setEndValue(QPoint(this->width(), 0));
    anim->setDuration(m_duration);
    anim->setEasingCurve(m_curve);
    m_animGroup->addAnimation(anim);

    anim = new QPropertyAnimation(m_jumbleFrame, "pos");
    anim->setStartValue(QPoint(-this->width(), 0));
    anim->setEndValue(QPoint(0, 0));
    anim->setDuration(m_duration);
    anim->setEasingCurve(m_curve);
    m_animGroup->addAnimation(anim);

    m_animGroup->start();
}

void CMainForm::animToSettings()
{
    m_jumbleFrame->stopJumble();
    m_jumbleFrame->releaseKeyboard();

    m_oldFrame = m_jumbleFrame;
    m_currFrame = m_settingsFrame;
    m_settingsFrame->resize(this->size());
    m_settingsFrame->move(this->width(), 0);
    m_settingsFrame->show();

    QPropertyAnimation *anim = new QPropertyAnimation(m_settingsFrame, "pos");
    anim->setStartValue(QPoint(this->width(), 0));
    anim->setEndValue(QPoint(0, 0));
    anim->setDuration(m_duration);
    anim->setEasingCurve(m_curve);
    m_animGroup->addAnimation(anim);

    anim = new QPropertyAnimation(m_jumbleFrame, "pos");
    anim->setStartValue(QPoint(0, 0));
    anim->setEndValue(QPoint(-this->width(), 0));
    anim->setDuration(m_duration);
    anim->setEasingCurve(m_curve);
    m_animGroup->addAnimation(anim);

    m_animGroup->start();
}

void CMainForm::animationFinished()
{
    m_animGroup->clear();

    if (m_oldFrame) {
        m_oldFrame->hide();
    }

}

void CMainForm::switchFullScreen(bool yes)
{
    if (yes) {
        showFullScreen();
    } else {
        showNormal();
    }

}

void CMainForm::closeEvent(QCloseEvent *event)
{
    m_settingsFrame->writeSettings();
    event->accept();
}

void CMainForm::resetSettings()
{
    m_jumbleFrame->acceptSettings(m_settingsFrame);
}

