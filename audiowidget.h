#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

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
#include <cmath>
#include <QPushButton> // Add this line
#include <QVBoxLayout> // Add this line
#include <QSlider>     // Add this line
#include <QLabel>      // Add this line
#include <QPainter>    // Add this line
#include <QMatrix4x4>  // Add this line
#include <QVector3D>   // Add this line
#include <QMap>        // Add this line
#include <QString>     // Add this line

class SoundVisualizationWidget;
class AudioWidget : public QWidget
{
    Q_OBJECT

public:
    AudioWidget(QWidget *parent = nullptr);
    void setFile(const QString &file);

private slots:
    void playAudio();
    void updatePosition();
    void newOcclusion();
    void modeChanged();
    void fileChanged(const QString &file);
    void openFileDialog();
    void updateRoom();
    void animateChanged(bool checked);
    void handlePositionChanged(QString id, QVector3D pos);
    void handleFileDropped(QString filePath, QVector3D position);
    void updateKnobs(QString id, QVector3D position);
    void onSoundSourceSelected(int index);
    void handlePlaySelectedFileRequested(const QString &filePath); // Slot for playing selected file
    void playAllFiles(); // Slot for playing all files


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
    QLabel *fileNameLabel;

    QAudioEngine engine;
    QAudioRoom *room;
    QAudioListener *listener;
    QSpatialSound *sound;
    QPropertyAnimation *animation;

    QMap<QString, QSpatialSound*> soundSources;
    QComboBox *soundSourceSelector; // Add this line
    void setupVisualization();

    void updateOcclusion(QSpatialSound *sound, QVector3D position); // Add this line
    void updateDistanceAttenuation(QSpatialSound *sound, QVector3D position);
    // Add maxDistance
    float maxDistance = 10.0f;

    QPushButton *playAllButton;

    SoundVisualizationWidget *soundVisualizationWidget;
};

#endif // AUDIOWIDGET_H
