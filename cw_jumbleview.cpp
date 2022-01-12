#include "cw_jumbleview.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>

CJumbleView::CJumbleView(QWidget *parent) :
    QWidget(parent)
{
    m_maxFigures = 50;
    m_currAnimateFigure = 0;
    m_animDuration = 1000;
    m_varyFigures = false;
    m_animType = QEasingCurve::OutQuad;

    m_figureColor = QColor(0xAAAAAA);
    m_activeColor = Qt::white;
    setMinimumSize(300, 300);

    m_animGroup = new QParallelAnimationGroup(this);
    connect(m_animGroup, SIGNAL(finished()), SIGNAL(animationFinished()));

    setMouseTracking(true);
}

CJumbleView::~CJumbleView()
{
    clearFigures();
}

void CJumbleView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

QRectF CJumbleView::randomRect(bool randomSize, bool intersectionAllowed)
{
    QRectF result;
    const int wmax = 1000;
    const int hmax = 1000;

    bool correct = false;
    int attempts = 1000;
    while(!correct && attempts > 0) {
        qreal w, h, x, y;

        if (randomSize) {
            w = (qrand() % wmax/13) + wmax/8;
            h = (qrand() % hmax/13) + hmax/8;
        } else {
            w = wmax/8;
            h = hmax/8;
        }
        x = qrand() % (wmax - (int)w - 10) + 5;
        y = qrand() % (hmax - (int)h - 10) + 5;
        result = QRectF(x/wmax, y/hmax, w/wmax, h/hmax); // normalized in 1x1

        correct = true;
        if (!intersectionAllowed) {
            for (int i = 0; i < m_figures.count(); ++i) {
                if (m_figures[i]->geometry().intersects(result)) {
                    correct = false;
                    break;
                }
            }
        }
        attempts --;
    }
    return result;
}

void CJumbleView::addFigure(CAbstractAnimFigure *figure)
{
    if (m_figures.count() >= m_maxFigures)
        return;

    if (figure) {
        figure->setGeometry(randomRect());
        figure->setColor(m_figureColor);
        connect(figure, SIGNAL(geometryChanged()), this, SLOT(update()));
        //figure->setTag(QString::number(m_figures.count() + 1));
        //figure->showTag();
        m_figures.append(figure);
        update();
    }
}

void CJumbleView::clearFigures()
{
    stopAnimation();
    for (int i = 0; i < m_figures.count(); ++i) {
        delete m_figures[i];
    }
    m_figures.clear();

}

void CJumbleView::hideFigureLabels()
{
    for (int i = 0; i < m_figures.count(); ++i) {
        m_figures[i]->hideInside();
    }
    update();
}

void CJumbleView::showFigureLabels()
{
    for (int i = 0; i < m_figures.count(); ++i) {
        m_figures[i]->showTag();
        m_figures[i]->showInsideColor();
    }
    update();
}


void CJumbleView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if (hasFocus())
        painter.setPen(QPen(QBrush(QColor(0x538BC4)), 2));
    else
        painter.setPen(QPen(QBrush(QColor(0x466A7E)), 2));
    painter.setBrush(QColor(0x272822));
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    painter.setRenderHints(QPainter::Antialiasing);

    QFont newFont = painter.font();
    newFont.setPointSize(15);
    painter.setFont(newFont);

    for (int i = 0; i < m_figures.count(); ++i) {
        m_figures[i]->paintTo(&painter,
                              this,
                              width(),  //denormalize
                              height());
    }
    painter.end();
}

QEasingCurve::Type CJumbleView::getRandomAnimType(void)
{
    return (QEasingCurve::Type) (qrand()%43);
}


void CJumbleView::animateGroup()
{
    setAllFiguresColor(m_figureColor);
    //m_animGroup->clear();
    stopAnimation();

    QVector <QRectF> targetRects;
    for (int i = 0; i < m_figures.count(); ++i) {
        QRectF currTargetRect;
        int attempts = 1000;
        bool correct = false;
        while(!correct && attempts > 0) {
            currTargetRect = randomRect(m_varyFigures, true);
            correct = true;
            for (int i = 0; i < targetRects.count(); ++i) {
                if (targetRects[i].intersects(currTargetRect)) {
                    correct = false;
                    break;
                }
            }
            attempts --;
        }
        targetRects.append(currTargetRect);

        QPropertyAnimation *anim = new QPropertyAnimation(m_figures[i], "geometry");
        anim->setStartValue(m_figures[i]->geometry());
        anim->setEndValue(currTargetRect);
        anim->setDuration(m_animDuration);
        if (m_randomAnimation) {
            anim->setEasingCurve(getRandomAnimType());
        } else {
            anim->setEasingCurve(m_animType);
        }
        m_animGroup->addAnimation(anim);
    }
    m_animGroup->start();
}

void CJumbleView::animateOne()
{
    if (m_figures.count() > 0) {
        setAllFiguresColor(m_figureColor);
        //m_animGroup->clear();
        stopAnimation();

        int animateFigure = qrand() % m_figures.count();
        QPropertyAnimation *anim = new QPropertyAnimation(m_figures[animateFigure], "geometry");
        anim->setStartValue(m_figures[animateFigure]->geometry());
        anim->setEndValue(randomRect(m_varyFigures));
        anim->setDuration(m_animDuration);
        if (m_randomAnimation) {
            anim->setEasingCurve(getRandomAnimType());
        } else {
            anim->setEasingCurve(m_animType);
        }
        m_animGroup->addAnimation(anim);
        m_animGroup->start();
    }
}

void CJumbleView::stopAnimation()
{
    m_animGroup->stop();
    for (int i = 0; i < m_animGroup->animationCount(); ++i) {
       m_animGroup->animationAt(i)->stop();
    }
    m_animGroup->clear();
}

int CJumbleView::figureUnderPoint(const QPoint &pos)
{
    for (int i = 0; i < m_figures.count(); ++i) {
        QRect figRect = m_figures.at(i)->scaledGeometry(width(), height());
        if (figRect.contains(pos)) {
            return i;
        }
    }
    return -1;
}

void CJumbleView::setAllFiguresColor(const QColor &color)
{
    for (int i = 0; i < m_figures.count(); ++i) {
            m_figures[i]->setColor(color);
        }
    update();
}

void CJumbleView::mouseMoveEvent(QMouseEvent *event)
{
    //bool doRedraw = false;
    int activeIdx = figureUnderPoint(event->pos());
    for (int i = 0; i < m_figures.count(); ++i) {
        QRect figRect = m_figures.at(i)->scaledGeometry(width(), height());
        if (activeIdx >= 0 && i == activeIdx) {
            m_figures[i]->setColor(m_activeColor);
        } else {
            m_figures[i]->setColor(m_figureColor);
        }
    }
    update();
}

void CJumbleView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int figIdx = figureUnderPoint(event->pos());
        if (figIdx >= 0) {
            bool show = !m_figures[figIdx]->identified();
            m_figures[figIdx]->showTag(show);
            m_figures[figIdx]->showInsideColor(show);
            update();
        }
    }
}

void CJumbleView::leaveEvent(QEvent *event)
{
    setAllFiguresColor(m_figureColor);
}

void CJumbleView::setAnimDuration(int duration)
{
    m_animDuration = duration;
    for (int i = 0; i < m_animGroup->animationCount(); ++i) {
       QPropertyAnimation *anim = dynamic_cast<QPropertyAnimation *> (m_animGroup->animationAt(i));
       if (anim) {
           anim->setDuration(m_animDuration);
       }
    }
}

void CJumbleView::incAnimDuration(int value)
{
    if (m_animDuration < 20000) {
        setAnimDuration(m_animDuration + value);
    }
}

void CJumbleView::decAnimDuration(int value)
{
    if (m_animDuration > value) {
        setAnimDuration(m_animDuration - value);
    }
}

