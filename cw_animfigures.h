#ifndef CGW_BODYFIGURES_H
#define CGW_BODYFIGURES_H

#include <QGraphicsWidget>
#include <QPainter>

class CAbstractAnimFigure : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
public:
    CAbstractAnimFigure(QObject *parent = 0);
    inline void setColor(const QColor & color) {m_color = color;}
    inline void setInsideColor(const QColor & color) {m_insideColor = color;}
    inline void setTag(const QString &tag) {m_tag = tag;}

    inline QRectF geometry() {return m_geometry;}
    QRect scaledGeometry(float scaleX, float scaleY);
    inline void setGeometry(QRectF rect) {m_geometry = rect; emit geometryChanged();}

    inline void showTag(bool doShow = true) {m_showTag = doShow;}
    inline void showInsideColor(bool doShow = true) {m_showInsideColor = doShow;}
    inline void hideInside() {showTag(false); showInsideColor(false);}
    inline bool identified() {return m_showTag || m_showInsideColor;}

    virtual void paintTo(QPainter *painter, QWidget *widget, float scaleX = 1, float scaleY = 1) = 0;

signals:
    void geometryChanged();

protected:
    QColor  m_color;
    QColor  m_insideColor;
    QString m_tag;
    QRectF  m_geometry;

    bool m_showTag;
    bool m_showInsideColor;
};

class CRectAnimFigure : public CAbstractAnimFigure
{
    Q_OBJECT
public:
    CRectAnimFigure(const QColor & color = Qt::white, QObject *parent = 0): CAbstractAnimFigure(parent)
    {
        setColor(color);
    }

    void paintTo(QPainter *painter, QWidget *widget, float scaleX = 1, float scaleY = 1);
};

class CEllipseAnimFigure : public CAbstractAnimFigure
{
    Q_OBJECT
public:
    CEllipseAnimFigure(const QColor & color = Qt::white, QObject *parent = 0): CAbstractAnimFigure(parent)
    {
        setColor(color);
    }

    void paintTo(QPainter *painter, QWidget *widget, float scaleX = 1, float scaleY = 1);
};


#endif // CGW_BODYFIGURES_H
