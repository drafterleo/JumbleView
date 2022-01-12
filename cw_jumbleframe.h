#ifndef CW_JUMBLEFRAME_H
#define CW_JUMBLEFRAME_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <QStack>


#include "cw_jumbleview.h"
#include "cw_pushbutton.h"
#include "cw_settingsframe.h"

class CJumbleFrame: public QWidget
{
    Q_OBJECT
public:
    CJumbleFrame(QWidget * parent = 0);

    void acceptSettings(CSettingsFrame *settings);

public slots:
    void startJumble();
    void stopJumble();

protected slots:
    void startStopJumble();
    void makeJumble();

    void animationFinished();

    void addFigure(const QString & tag);
    void addNumFigure();
    void addFalseFigure();

    void showInfo(const QString & info);
    void hideInfo();

    void makeWordList(const QString &data);
    QString randomWord();
    void nextWord();

signals:
    void needSettings();
    void needResetSettings();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    CJumbleView *jumbleView;
    bool jumbleStarted;
    int jumbleCount;
    int jumbleNum;
    int jumblePause;
    bool groupAnimation;

    //QTimer *jumblePauseTimer;

    bool hideFigureLabels;

    CPushButton *settingsBtn;
    CPushButton *startStopBtn;

    QLabel *infoLabel;
    QTimer *hideInfoTimer;

    int lastSequenceNum;

    QStringList wordList;
    QStringList usedWords;
    QString     currentWord;
};


#endif // CW_JUMBLEFRAME_H
