#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include <sndfile.h>
#include <QWidget>
#include <QAudioEngine>
#include <QAudioListener>
#include <QAudioRoom>
#include <QSpatialSound>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QPropertyAnimation>
#include <QFileDialog>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <cmath>

class SoundVisualizationWidget;

class AudioWidget : public QWidget
{
    Q_OBJECT

public:
    AudioWidget(QWidget *parent = nullptr);
    void setFile(const QString &file);
    void saveProcessedFile(const QString &format);
    ~AudioWidget() override;

private slots:
    void playAudio();
    void updatePosition();
    void newOcclusion();
    void modeChanged();
    void fileChanged(const QString &file);
    void openFileDialog();
    void updateRoom();
    void animateChanged(bool checked);
    void fileSelected(const QItemSelection &selected, const QItemSelection &deselected);
    void applySpatialEffects(QByteArray &audioData, int sampleRate, unsigned channels);

private:
    QLineEdit *fileEdit;
    QPushButton *fileDialogButton;
    QTreeView *fileTreeView;
    QFileSystemModel *fileSystemModel;
    QSlider *azimuth;
    QSlider *elevation;
    QSlider *distance;
    QSlider *occlusion;
    QSlider *roomDimension;
    QSlider *reverbGain;
    QSlider *reflectionGain;
    QComboBox *mode;
    QCheckBox *animateButton;
    QPushButton *playButton;
    QFileDialog *fileDialog;
    bool isPlaying;
    QAudioEngine engine;
    QAudioRoom *room;
    QAudioListener *listener;
    QSpatialSound *sound;
    QPropertyAnimation *animation;
    SoundVisualizationWidget *soundVisualizationWidget;
    QPushButton *saveLocationButton;
    QLineEdit *saveLocationEdit;

    QVBoxLayout* rightPanel;
    QString selectedFile;
    QString saveLocation;
};

#endif // AUDIOWIDGET_H
