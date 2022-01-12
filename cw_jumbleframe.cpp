#include <Qpainter>
#include <QtDebug>
#include <QTimer>
#include "cw_jumbleframe.h"

CJumbleFrame::CJumbleFrame(QWidget * parent)
    : QWidget(parent)
{
    jumbleStarted = false;
    jumbleCount = 5;
    jumbleNum = 0;
    jumblePause = 1000;

    groupAnimation = true;
    hideFigureLabels = true;

    lastSequenceNum = 0;

    settingsBtn = new CPushButton(QPixmap(":/images/iconSettings.png"), this);
    settingsBtn->resize(40, 40);
    //connect(settingsBtn, SIGNAL(clicked()), this, SLOT(showHideSettigs()));
    connect(settingsBtn, SIGNAL(clicked()), SIGNAL(needSettings()));

    startStopBtn = new CPushButton(QPixmap(":/images/iconStart.png"), this);
    startStopBtn->resize(40, 40);
    connect(startStopBtn, SIGNAL(clicked()), this, SLOT(startStopJumble()));

    jumbleView = new CJumbleView(this);
    connect(jumbleView, SIGNAL(animationFinished()), SLOT(animationFinished()));

    infoLabel = new QLabel("info", this);
    infoLabel->hide();
    infoLabel->setFont(QFont("", 12));

    hideInfoTimer = new QTimer(this);
    connect(hideInfoTimer, SIGNAL(timeout()), SLOT(hideInfo()));
}

void CJumbleFrame::makeWordList(const QString &data)
{
    QString str = data;
    wordList = str.remove(' ').split(",", QString::SkipEmptyParts);
    usedWords.clear();
}

QString CJumbleFrame::randomWord()
{
    if (wordList.isEmpty())
        return "JUMBLE";

    QString result = "JUMBLE";
    int attempt = 0;
    while (attempt < wordList.length() * 2) {
        int idx = qrand() % wordList.length();
        result = wordList.at(idx);
        if (!usedWords.contains(result, Qt::CaseInsensitive)){
            usedWords.append(result);
            if (usedWords.length() > wordList.length() / 2) {
                usedWords.removeFirst();
            }
            break;
        }
        attempt ++;
    }

    currentWord = result.toUpper();
    return currentWord;
}

void CJumbleFrame::acceptSettings(CSettingsFrame *settings)
{
    if (settings) {
        stopJumble();
        jumbleView->clearFigures();
        wordList.clear();
        currentWord = "";

        lastSequenceNum = 0;

        jumbleCount = settings->jumbleCountSpinBox->value();

        jumbleView->setVaryFigures(settings->varyFiguresCBox->isChecked());
        jumbleView->setAnimDuration(settings->animDurationSpinBox->value());
        jumbleView->setAnimType((QEasingCurve::Type)settings->animTypeCmBox->itemData(settings->animTypeCmBox->currentIndex(), Qt::UserRole).toInt());
        jumbleView->setRandomAnimation(settings->randomAnimationCBox->isChecked());

        jumblePause = settings->animPauseSpinBox->value();
        hideFigureLabels = settings->hideFigureMarksCBox->isChecked();
        groupAnimation = settings->groupAnimationCBox->isChecked();

        if (settings->wordModeCBox->isChecked()) {
            makeWordList(settings->wordEdit->text());
            QString word = randomWord();
            for(int i = 0; i < word.length(); i++) {
                addFigure(word.at(i));
            }
        } else {
            if (settings->clusterModeCBox->isChecked())
            {
                QString letters = "ABCDEFGHKLMNOPQRST";
                int clusterCount = settings->clusterCountSpinBox->value();
                for (int clusterIdx = 0; clusterIdx < clusterCount; ++clusterIdx) {
                    QChar letter = letters[clusterIdx];
                    for(int i = 0; i < settings->figureCountSpinBox->value() ; i++) {
                        addFigure(letter);
                    }
                }

            } else {
                lastSequenceNum = settings->figureCountSpinBox->value();
                for(int i = 0; i < settings->figureCountSpinBox->value() ; i++) {
                    addFigure(QString::number(i + 1));
                }
            }
        }

        for(int i = 0; i < settings->falseFigureCountSpinBox->value(); i++) {
            addFalseFigure();
        }
    }
}

void CJumbleFrame::addFigure(const QString & tag)
{
    CEllipseAnimFigure *figure = new CEllipseAnimFigure();
    figure->setTag(tag);
    figure->showTag();
    jumbleView->addFigure(figure);
}

void CJumbleFrame::addNumFigure()
{
    lastSequenceNum ++;
    addFigure(QString::number(lastSequenceNum));
}

void CJumbleFrame::addFalseFigure()
{
    addFigure("X");
}

void CJumbleFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    painter.end();
}

void CJumbleFrame::resizeEvent(QResizeEvent *)
{
    int border = 50;
//        if (width() > height()) {
//          border = height()/12;
//        } else  {
//            border = width()/12;
//        }

    int jwW = width() - 2*border;
    int jwH = height() - 2*border;
    int jwHW = jwW;
    if (jwW > jwH) {
        jwHW = jwH;
    }

    jumbleView->setGeometry((width() - jwHW)/2,
                            (height() - jwHW)/2 - 10, jwHW, jwHW);

    settingsBtn->move(width() - settingsBtn->width() - 2, 5);
    startStopBtn->move((width() - startStopBtn->width())/2, jumbleView->geometry().bottom() + 10);

    infoLabel->move(10, 10);
}

void CJumbleFrame::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        startStopJumble();
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        stopJumble();
        return;
    }

    if (event->key() == Qt::Key_Return) {
        nextWord();
        return;
    }

    if (event->key() == Qt::Key_Up) {
        jumbleView->incAnimDuration(50);
        showInfo(QString("Anim Duration: %1").arg(jumbleView->animDuration()));
        return;
    }

    if (event->key() == Qt::Key_Down) {
        jumbleView->decAnimDuration(50);
        showInfo(QString("Anim Duration: %1").arg(jumbleView->animDuration()));
        return;
    }

    if (event->modifiers() & Qt::AltModifier) {
        if (event->key() == Qt::Key_R) {
            emit needResetSettings();
            return;
        }
        if (event->key() == Qt::Key_A) {
            addNumFigure();
            return;
        }
        if (event->key() == Qt::Key_F) {
            addFalseFigure();
            return;
        }
        if (event->key() == Qt::Key_S) {
            jumbleView->showFigureLabels();
            return;
        }
        if (event->key() == Qt::Key_H) {
            jumbleView->hideFigureLabels();
            return;
        }

        if (event->key() == Qt::Key_W) {
            showInfo(currentWord);
            return;
        }
    }
}

void CJumbleFrame::startJumble()
{
    if (jumbleCount > 0) {
        startStopBtn->hide();
    }

    startStopBtn->setPixmap(QPixmap(":/images/iconStop.png"));

    if (hideFigureLabels) {
        jumbleView->hideFigureLabels();
    }

    jumbleStarted = true;
    jumbleNum = 0;

    makeJumble();
}

void CJumbleFrame::stopJumble()
{
    jumbleView->stopAnimation();
    startStopBtn->setPixmap(QPixmap(":/images/iconStart.png"));
    startStopBtn->show();
    jumbleStarted = false;
}

void CJumbleFrame::startStopJumble()
{
    if (jumbleStarted) {
        stopJumble();
    } else {
        startJumble();
    }
}

void CJumbleFrame::nextWord()
{
    if (wordList.isEmpty()) {
        startStopJumble();
    } else {
        stopJumble();
        jumbleView->clearFigures();
        QString word = randomWord();
        for(int i = 0; i < word.length(); i++) {
            addFigure(word.at(i));
        }
        startJumble();
    }
}

void CJumbleFrame::makeJumble()
{
    if (jumbleStarted) {
        if (groupAnimation) {
            jumbleView->animateGroup();
        } else {
            jumbleView->animateOne();
        }
    }
    // -> see CJumbleFrame::animationFinished()
}

void CJumbleFrame::animationFinished()
{
    if (jumbleStarted) {
        if (jumbleCount > 0) {
            jumbleNum ++;
            if (jumbleNum == jumbleCount) {
                stopJumble();
                return;
            }
        }
        QTimer::singleShot(jumblePause, this, SLOT(makeJumble())); // pause
    }
}

void CJumbleFrame::showInfo(const QString & info)
{
    infoLabel->setText(info);
    infoLabel->show();
    hideInfoTimer->stop();
    hideInfoTimer->start(1000);
}

void CJumbleFrame::hideInfo()
{
    infoLabel->hide();
    hideInfoTimer->stop();
}

