#include <QtDebug>
#include "cw_animfigures.h"

CAbstractAnimFigure::CAbstractAnimFigure(QObject *parent)
    : QObject(parent)
{
    m_color = Qt::white;
    m_insideColor = Qt::transparent;
    m_tag = "";

    m_showTag = false;
    m_showInsideColor = false;
}

QRect CAbstractAnimFigure::scaledGeometry(float scaleX, float scaleY)
{
    return QRect(m_geometry.left() * scaleX, m_geometry.top() * scaleY,
                 m_geometry.width() * scaleX, m_geometry.height() * scaleY);
}

/* *************************************** */

void CRectAnimFigure::paintTo(QPainter *painter, QWidget *widget, float scaleX, float scaleY)
{
    painter->setPen(QPen(m_color, 3));
    painter->setBrush(Qt::NoBrush);
//    if (m_showInsideColor) {
//        painter->setBrush(m_insideColor);
//    }
//    else {
//        painter->setBrush(Qt::NoBrush);
//    }

    QRect drawRect = scaledGeometry(scaleX, scaleY);
    painter->drawRect(drawRect.adjusted(2, 2, -2, -2));

    if (m_showTag) {
        painter->setPen(m_color);
        painter->drawText(drawRect, Qt::AlignCenter, m_tag);
    }
}

/* *************************************** */

void CEllipseAnimFigure::paintTo(QPainter *painter, QWidget *widget, float scaleX, float scaleY)
{
    painter->setPen(QPen(m_color, 3));
    painter->setBrush(Qt::NoBrush);
//    if (m_showInsideColor) {
//        painter->setBrush(m_insideColor);
//    }
//    else {
//        painter->setBrush(Qt::NoBrush);
//    }

    QRect drawRect = scaledGeometry(scaleX, scaleY);
    painter->drawEllipse(drawRect.adjusted(2, 2, -2, -2));

    if (m_showTag) {
        painter->setPen(m_color);
        painter->drawText(drawRect, Qt::AlignCenter, m_tag);
    }
}


