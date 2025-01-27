#ifndef SOUNDVISUALIZATIONWIDGET_H
#define SOUNDVISUALIZATIONWIDGET_H
#include <QWidget>
#include <QPainter>
class SoundVisualizationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundVisualizationWidget(QWidget *parent = nullptr);
    void setSoundPosition(float x, float y, float z);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    float soundX = 0, soundY = 0, soundZ = 0;
};
#endif // SOUNDVISUALIZATIONWIDGET_H
