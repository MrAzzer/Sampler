#ifndef SOUNDVISUALIZATIONWIDGET_H
#define SOUNDVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QVector3D>
#include <QMap>
#include <QMatrix4x4>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

class SoundVisualizationWidget : public QWidget {
    Q_OBJECT
public:
    explicit SoundVisualizationWidget(QWidget *parent = nullptr);
    void addSoundSource(const QString &id, QVector3D position);
    void updateSoundPosition(const QString &id, QVector3D position);
    QVector3D getSoundPosition(const QString &id) const;
    QMap<QString, QVector3D> getAllSoundSources() const;

signals:
    void fileDropped(QString filePath, QVector3D position);
    void positionChanged(QString id, QVector3D newPosition);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void createRotationControls();
    void updateSliders();
    void drawCube(QPainter &painter);
    void drawSoundSources(QPainter &painter);
    void drawAxisLabels(QPainter &painter);
    QPointF convertTo3DSpace(QVector3D position) const;

    QMap<QString, QVector3D> soundSources;
    float cubeSize = 200.0f;
    float rotateX = 0, rotateY = 0, rotateZ = 0;
    QPoint lastMousePos;
    QSlider *xRotSlider, *yRotSlider, *zRotSlider;

    QString m_selectedId;
};

#endif // SOUNDVISUALIZATIONWIDGET_H
