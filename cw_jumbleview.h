#ifndef CW_JUMBLEVIEW_H
#define CW_JUMBLEVIEW_H

#include <QWidget>
#include <QEasingCurve>
#include <QParallelAnimationGroup>
#include "cw_animfigures.h"

class CJumbleView : public QWidget
{
    Q_OBJECT
public:
    explicit CJumbleView(QWidget *parent = 0);
    ~CJumbleView();

    void addFigure(CAbstractAnimFigure *figure);
    void clearFigures();

    inline int animDuration() {return m_animDuration;}
    void setAnimDuration(int duration);
    inline void setVaryFigures(bool vary) {m_varyFigures = vary;}
    inline void setAnimType(QEasingCurve::Type animType) {m_animType = animType;}
    inline void setRandomAnimation(bool randAnim) {m_randomAnimation = randAnim;}

    void hideFigureLabels();
    void showFigureLabels();

public slots:
    void animateGroup();
    void animateOne();
    void stopAnimation();

    void incAnimDuration(int value);
    void decAnimDuration(int value);

signals:
    void animationFinished();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    virtual void leaveEvent(QEvent *event);

    QRectF randomRect(bool randomSize = false, bool intersectionAllowed = false);
    int figureUnderPoint(const QPoint &pos);

    void setAllFiguresColor(const QColor &color);

    QEasingCurve::Type getRandomAnimType(void);

private:
    QVector <CAbstractAnimFigure *> m_figures; //normalized in 1x1

    QParallelAnimationGroup *m_animGroup;

    int m_currAnimateFigure;
    int m_animDuration;

    QEasingCurve::Type m_animType;
    bool m_randomAnimation;

    bool m_varyFigures;

    QColor m_figureColor;
    QColor m_activeColor;

    int m_maxFigures;
};


#endif // CW_JUMBLEVIEW_H
