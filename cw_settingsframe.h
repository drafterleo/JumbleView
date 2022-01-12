#ifndef CW_SETTINGSFRAME_H
#define CW_SETTINGSFRAME_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>

class CSettingsFrame : public QWidget
{
    Q_OBJECT

public:
    explicit CSettingsFrame(QWidget *parent = 0);

public:
    QSpinBox  *jumbleCountSpinBox;

    QComboBox *animTypeCmBox;
    QSpinBox  *animDurationSpinBox;
    QSpinBox  *animPauseSpinBox;
    QCheckBox *groupAnimationCBox;
    QCheckBox *randomAnimationCBox;

    QSpinBox  *figureCountSpinBox;
    QSpinBox  *falseFigureCountSpinBox;
    QCheckBox *hideFigureMarksCBox;
    QCheckBox *varyFiguresCBox;

    QCheckBox *clusterModeCBox;
    QSpinBox  *clusterCountSpinBox;

    QCheckBox *wordModeCBox;
    QLineEdit *wordEdit;

    QPushButton *fileBtn;

    QCheckBox *fullScreenCBox;

signals:
    void needJumble();
    void needQuit();
    void needFullScreen(bool);
    
public slots:
    void writeSettings(void);
    void readSettings(void);

private slots:
    void clusterModeToggled(bool checked);
    void clusterCountChanged(int value);
    void wordModeToggled(bool checked);
    void loadWords();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void makeControls();

private:
    QWidget *center;
    QPushButton *startBtn;
    QPushButton *quitBtn;

    QWidget *globalsPanel;

};

#endif // CW_SETTINGSFRAME_H
