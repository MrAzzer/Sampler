#ifndef SOUNDVISUALIZATIONWIDGET_H
#define SOUNDVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QPushButton> // Add this line
#include <QVBoxLayout> // Add this line
#include <QSlider>     // Add this line
#include <QLabel>      // Add this line
#include <QPainter>    // Add this line
#include <QMatrix4x4>  // Add this line
#include <QVector3D>   // Add this line
#include <QMap>        // Add this line
#include <QString>     // Add this line

class SoundVisualizationWidget : public QWidget {
    Q_OBJECT
public:
    explicit SoundVisualizationWidget(QWidget *parent = nullptr);
    void addSoundSource(const QString &id, QVector3D position, const QString &filePath);
    void updateSoundPosition(const QString &id, QVector3D position);
    QVector3D getSoundPosition(const QString &id) const;
    QMap<QString, QVector3D> getAllSoundSources() const;
    QString getSelectedFileName() const;

signals:
    void fileDropped(QString filePath, QVector3D position);
    void positionChanged(QString id, QVector3D newPosition);
    void nodeSelected(QString filePath);
    void playSelectedFileRequested(const QString &filePath);
    

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onPlaySelectedFileClicked();

private:
    SoundVisualizationWidget *soundVisualizationWidget; // Declare pointer
    void createRotationControls();
    void updateSliders();
    void drawCube(QPainter &painter, const QMatrix4x4 &mvp, const QRect &viewport);
    void drawSoundSources(QPainter &painter, const QMatrix4x4 &mvp, const QRect &viewport);
    void drawAxisLabels(QPainter &painter);
    QPointF convertTo3DSpace(QVector3D position) const;

    QMap<QString, QVector3D> soundSources;
    QMap<QString, QString> soundSourceFiles; // Map ID to file path
    float cubeSize = 800.0f;
    float rotateX = 0, rotateY = 0, rotateZ = 0;
    QPoint lastMousePos;
    QSlider *xRotSlider, *yRotSlider, *zRotSlider;
    QPushButton *playSelectedFileButton;

    QString m_selectedId;
};

#endif // SOUNDVISUALIZATIONWIDGET_H
