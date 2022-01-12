#include <QPainter>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QEasingCurve>
#include <QFormLayout>
#include <QLineEdit>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QtDebug>
#include <QApplication>
#include "cw_settingsframe.h"

CSettingsFrame::CSettingsFrame(QWidget *parent) :
    QWidget(parent)
{
    center = new QWidget(this);
    center->resize(500,500);

    startBtn = new QPushButton("Start", this);
    startBtn->resize(80, 30);
    connect(startBtn, SIGNAL(clicked()), SIGNAL(needJumble()));

    quitBtn = new QPushButton("Quit", this);
    quitBtn->resize(80, 30);
    connect(quitBtn, SIGNAL(clicked()), SIGNAL(needQuit()));

    makeControls();

}

void CSettingsFrame::makeControls()
{
    jumbleCountSpinBox = new QSpinBox;
    jumbleCountSpinBox->setMinimum(0);
    jumbleCountSpinBox->setMaximum(1000);
    jumbleCountSpinBox->setSingleStep(1);
    jumbleCountSpinBox->setValue(5);
    jumbleCountSpinBox->findChild<QLineEdit*>()->setReadOnly(true);

    animTypeCmBox = new QComboBox;
    animTypeCmBox->setGeometry(80, 35, 200, 20);
    animTypeCmBox->addItem("Linear", QVariant((int)QEasingCurve::Linear));
    animTypeCmBox->addItem("InQuad", QVariant((int)QEasingCurve::InQuad));
    animTypeCmBox->addItem("OutQuad", QVariant((int)QEasingCurve::OutQuad));
    animTypeCmBox->addItem("InOutQuad", QVariant((int)QEasingCurve::InOutQuad));
    animTypeCmBox->addItem("OutInQuad", QVariant((int)QEasingCurve::OutInQuad));
    animTypeCmBox->addItem("InCubic", QVariant((int)QEasingCurve::InCubic));
    animTypeCmBox->addItem("OutCubic", QVariant((int)QEasingCurve::OutCubic));
    animTypeCmBox->addItem("InOutCubic", QVariant((int)QEasingCurve::InOutCubic));
    animTypeCmBox->addItem("OutInCubic", QVariant((int)QEasingCurve::OutInCubic));
    animTypeCmBox->addItem("InQuint", QVariant((int)QEasingCurve::InQuint));
    animTypeCmBox->addItem("OutQuint", QVariant((int)QEasingCurve::OutQuint));
    animTypeCmBox->addItem("InOutQuint", QVariant((int)QEasingCurve::InOutQuint));
    animTypeCmBox->addItem("OutInQuint", QVariant((int)QEasingCurve::OutInQuint));
    animTypeCmBox->addItem("InSine", QVariant((int)QEasingCurve::InSine));
    animTypeCmBox->addItem("OutSine", QVariant((int)QEasingCurve::OutSine));
    animTypeCmBox->addItem("InOutSine", QVariant((int)QEasingCurve::InOutSine));
    animTypeCmBox->addItem("OutInSine", QVariant((int)QEasingCurve::OutInSine));
    animTypeCmBox->addItem("InExpo", QVariant((int)QEasingCurve::InExpo));
    animTypeCmBox->addItem("OutExpo", QVariant((int)QEasingCurve::OutExpo));
    animTypeCmBox->addItem("InOutExpo", QVariant((int)QEasingCurve::InOutExpo));
    animTypeCmBox->addItem("OutInExpo", QVariant((int)QEasingCurve::OutInExpo));
    animTypeCmBox->addItem("InCirc", QVariant((int)QEasingCurve::InCirc));
    animTypeCmBox->addItem("OutCirc", QVariant((int)QEasingCurve::OutCirc));
    animTypeCmBox->addItem("InOutCirc", QVariant((int)QEasingCurve::InOutCirc));
    animTypeCmBox->addItem("OutInCirc", QVariant((int)QEasingCurve::OutInCirc));
    animTypeCmBox->addItem("InElastic", QVariant((int)QEasingCurve::InElastic));
    animTypeCmBox->addItem("OutElastic", QVariant((int)QEasingCurve::OutElastic));
    animTypeCmBox->addItem("InOutElastic", QVariant((int)QEasingCurve::InOutElastic));
    animTypeCmBox->addItem("OutInElastic", QVariant((int)QEasingCurve::OutInElastic));
    animTypeCmBox->addItem("InBack", QVariant((int)QEasingCurve::InBack));
    animTypeCmBox->addItem("OutBack", QVariant((int)QEasingCurve::OutBack));
    animTypeCmBox->addItem("InOutBack", QVariant((int)QEasingCurve::InOutBack));
    animTypeCmBox->addItem("OutInBack", QVariant((int)QEasingCurve::OutInBack));
    animTypeCmBox->addItem("InBounce", QVariant((int)QEasingCurve::InBounce));
    animTypeCmBox->addItem("OutBounce", QVariant((int)QEasingCurve::OutBounce));
    animTypeCmBox->addItem("InOutBounce", QVariant((int)QEasingCurve::InOutBounce));
    animTypeCmBox->addItem("OutInBounce", QVariant((int)QEasingCurve::OutInBounce));
    animTypeCmBox->addItem("InCurve", QVariant((int)QEasingCurve::InCurve));
    animTypeCmBox->addItem("OutCurve", QVariant((int)QEasingCurve::OutCurve));
    animTypeCmBox->addItem("SineCurve", QVariant((int)QEasingCurve::SineCurve));
    animTypeCmBox->addItem("CosineCurve", QVariant((int)QEasingCurve::CosineCurve));
    animTypeCmBox->setCurrentIndex(3);

    animDurationSpinBox = new QSpinBox;
    animDurationSpinBox->setMinimum(100);
    animDurationSpinBox->setMaximum(5000);
    animDurationSpinBox->setSingleStep(100);
    animDurationSpinBox->setValue(1000);
    animDurationSpinBox->setSuffix(tr(" msec."));
    animDurationSpinBox->findChild<QLineEdit*>()->setReadOnly(true);

    animPauseSpinBox = new QSpinBox;
    animPauseSpinBox->setMinimum(0);
    animPauseSpinBox->setMaximum(5000);
    animPauseSpinBox->setSingleStep(100);
    animPauseSpinBox->setValue(1000);
    animPauseSpinBox->setSuffix(tr(" msec."));
    animPauseSpinBox->findChild<QLineEdit*>()->setReadOnly(true);

    groupAnimationCBox = new QCheckBox("Group Animation");
    groupAnimationCBox->setChecked(true);

    randomAnimationCBox = new QCheckBox("Random Animation");
    randomAnimationCBox->setChecked(false);

    figureCountSpinBox = new QSpinBox;
    figureCountSpinBox->setMinimum(2);
    figureCountSpinBox->setMaximum(20);
    figureCountSpinBox->setSingleStep(1);
    figureCountSpinBox->setValue(3);
    figureCountSpinBox->findChild<QLineEdit*>()->setReadOnly(true);

    falseFigureCountSpinBox = new QSpinBox;
    falseFigureCountSpinBox->setMinimum(0);
    falseFigureCountSpinBox->setMaximum(10);
    falseFigureCountSpinBox->setSingleStep(1);
    falseFigureCountSpinBox->setValue(0);
    falseFigureCountSpinBox->findChild<QLineEdit*>()->setReadOnly(true);

    hideFigureMarksCBox = new QCheckBox("Hide Figure Marks");
    hideFigureMarksCBox->setChecked(true);

    varyFiguresCBox = new QCheckBox("Vary Figure Forms");
    varyFiguresCBox->setChecked(true);

    clusterCountSpinBox = new QSpinBox;
    clusterCountSpinBox->setMinimum(2);
    clusterCountSpinBox->setMaximum(5);
    clusterCountSpinBox->setSingleStep(1);
    clusterCountSpinBox->setValue(2);
    clusterCountSpinBox->findChild<QLineEdit*>()->setReadOnly(true);
    clusterCountSpinBox->setEnabled(false);
    connect(clusterCountSpinBox, SIGNAL(valueChanged(int)), SLOT(clusterCountChanged(int)));

    clusterModeCBox = new QCheckBox("Cluster Mode");
    clusterModeCBox->setChecked(false);
    connect(clusterModeCBox, SIGNAL(toggled(bool)), SLOT(clusterModeToggled(bool)));

    wordEdit = new QLineEdit;
    wordEdit->setText("JUMBLE");

    wordModeCBox = new QCheckBox("Word Mode");
    wordModeCBox->setChecked(false);
    connect(wordModeCBox, SIGNAL(toggled(bool)), SLOT(wordModeToggled(bool)));

    fileBtn = new QPushButton("...");
    fileBtn->setMaximumHeight(25);
    connect(fileBtn, SIGNAL(clicked()), SLOT(loadWords()));

    fullScreenCBox = new QCheckBox("Full Screen");
    fullScreenCBox->setChecked(false);
    connect(fullScreenCBox, SIGNAL(clicked(bool)), SIGNAL(needFullScreen(bool)));

    QFormLayout *layout = new QFormLayout;
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setHorizontalSpacing(15);

    layout->addRow(tr("Jumble Count"), jumbleCountSpinBox);

    QWidget *spaceWidget = new QWidget;
    spaceWidget->setMinimumHeight(10);
    layout->addRow("", spaceWidget);

    layout->addRow(tr("Figure Count"), figureCountSpinBox);
    layout->addRow(tr("False Figure Count"), falseFigureCountSpinBox);
    layout->addRow("", hideFigureMarksCBox);
    layout->addRow("", varyFiguresCBox);
    layout->addRow(tr("Cluster Count"), clusterCountSpinBox);
    layout->addRow("", clusterModeCBox);
    layout->addRow(tr("Word"), wordEdit);

    QHBoxLayout *wordLayout = new QHBoxLayout;
    wordLayout->addWidget(wordModeCBox);
    wordLayout->addSpacing(30);
    wordLayout->addWidget(fileBtn);
    wordLayout->addStretch(1);
    layout->addRow("", wordLayout);

    spaceWidget = new QWidget;
    spaceWidget->setMinimumHeight(10);
    layout->addRow("", spaceWidget);

    layout->addRow(tr("Anim Type"), animTypeCmBox);
    layout->addRow(tr("Anim Duration"), animDurationSpinBox);
    layout->addRow(tr("Pause"), animPauseSpinBox);
    layout->addRow("", groupAnimationCBox);
    layout->addRow("", randomAnimationCBox);

    spaceWidget = new QWidget;
    spaceWidget->setMinimumHeight(10);
    layout->addRow("", spaceWidget);

    layout->addRow("", fullScreenCBox);

    center->setLayout(layout);
}


void CSettingsFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    painter.setPen(QPen(QColor(0x466A7E), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(center->geometry());

    painter.end();

}

void CSettingsFrame::resizeEvent(QResizeEvent *event)
{
    center->move((width() - center->width())/2, (height() - center->height())/2);
    startBtn->move(center->geometry().left(), center->geometry().bottom() + 5);
    quitBtn->move(center->geometry().right() - quitBtn->width() + 1, center->geometry().bottom() + 5);
}

void CSettingsFrame::clusterModeToggled(bool checked)
{
    if (checked) {
        clusterCountSpinBox->setEnabled(true);
        figureCountSpinBox->setSuffix(QString(" x %1").arg(clusterCountSpinBox->value()));
    } else {
        clusterCountSpinBox->setEnabled(false);
        figureCountSpinBox->setSuffix("");
    }
}

void CSettingsFrame::wordModeToggled(bool checked)
{
    if (checked) {
        wordEdit->setEnabled(true);
        clusterModeCBox->setEnabled(false);
        clusterCountSpinBox->setEnabled(false);
        figureCountSpinBox->setEnabled(false);
    } else {
        wordEdit->setEnabled(false);
        clusterModeCBox->setEnabled(true);
        clusterCountSpinBox->setEnabled(true);
        figureCountSpinBox->setEnabled(true);
    }
}


void CSettingsFrame::clusterCountChanged(int value)
{
    if (clusterModeCBox->isChecked()) {
        figureCountSpinBox->setSuffix(QString(" x %1").arg(value));
    } else {
        figureCountSpinBox->setSuffix("");
    }
}

void CSettingsFrame::writeSettings(void)
{
    QSettings settings(QSettings::UserScope, "DrafterSoft", "JumbleView");

    settings.setValue("JumbleCount", jumbleCountSpinBox->value());
    settings.setValue("FigureCount", figureCountSpinBox->value());
    settings.setValue("FalseFigureCount", falseFigureCountSpinBox->value());
    settings.setValue("HideFigureMarks", hideFigureMarksCBox->isChecked());
    settings.setValue("VaryFigureForms", varyFiguresCBox->isChecked());
    settings.setValue("ClusterCount", clusterCountSpinBox->value());
    settings.setValue("ClusterMode", clusterModeCBox->isChecked());
    settings.setValue("AnimType", animTypeCmBox->currentIndex());
    settings.setValue("AnimDuration", animDurationSpinBox->value());
    settings.setValue("AnimPause", animPauseSpinBox->value());
    settings.setValue("GroupAnimation", groupAnimationCBox->isChecked());
    settings.setValue("RandomAnimation", randomAnimationCBox->isChecked());
    settings.setValue("Word", wordEdit->text());
    settings.setValue("WordMode", wordModeCBox->isChecked());
}

void CSettingsFrame::readSettings(void)
{
    QSettings settings(QSettings::UserScope, "DrafterSoft", "JumbleView");

    jumbleCountSpinBox->setValue(settings.value("JumbleCount", 5).toInt());
    figureCountSpinBox->setValue(settings.value("FigureCount", 3).toInt());
    falseFigureCountSpinBox->setValue(settings.value("FalseFigureCount", 0).toInt());
    hideFigureMarksCBox->setChecked(settings.value("HideFigureMarks", true).toBool());
    varyFiguresCBox->setChecked(settings.value("VaryFigureForms", true).toBool());
    clusterCountSpinBox->setValue(settings.value("ClusterCount", 2).toInt());
    clusterModeCBox->setChecked(settings.value("ClusterMode", false).toBool());
    animTypeCmBox->setCurrentIndex(settings.value("AnimType", 3).toInt());
    animDurationSpinBox->setValue(settings.value("AnimDuration", 1000).toInt());
    animPauseSpinBox->setValue(settings.value("AnimPause", 1000).toInt());
    groupAnimationCBox->setChecked(settings.value("GroupAnimation", true).toBool());
    randomAnimationCBox->setChecked(settings.value("RandomAnimation", false).toBool());
    wordEdit->setText(settings.value("Word", "JUMBLE").toString());
    wordModeCBox->setChecked(settings.value("WordMode", false).toBool());
    wordModeToggled(wordModeCBox->isChecked());
}

void CSettingsFrame::loadWords()
{

  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open File"),
                                                  qApp->applicationDirPath(),
                                                  tr("Lists (*.lst);; All Files (*.*)"));

  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
        //ioError(file, tr("Cannot open file %1 for reading"));
        return;
      }

      QString words;
      QTextStream in(&file);
      in.setCodec("UTF-8");

      while (!in.atEnd()) {
        QString word;
        in >> word;
        if (!word.isEmpty()) {
            words.append(word.toUpper());
            words.append(",");
        }
      }

      file.close();

      wordEdit->setText(words);
  }
}

