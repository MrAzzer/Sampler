#include "soundvisualizationwidget.h"
#include <QPainter>

SoundVisualizationWidget::SoundVisualizationWidget(QWidget *parent)
    : QWidget(parent), soundX(0), soundY(0), soundZ(0)
{
    setStyleSheet("background-color: #444;");
    setMinimumSize(200, 200);
}

void SoundVisualizationWidget::setSoundPosition(float x, float y, float z)
{
    soundX = x;
    soundY = y;
    soundZ = z;
    update();
}

void SoundVisualizationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.setBrush(Qt::blue);
    painter.drawEllipse(QPoint(centerX, centerY), 10, 10);

    float maxRangeX = static_cast<float>(width()) / 2 - 10;
    float maxRangeY = static_cast<float>(height()) / 2 - 10;

    int soundPosX = centerX + static_cast<int>(qBound(-maxRangeX, soundX * 10, maxRangeX));
    int soundPosY = centerY - static_cast<int>(qBound(-maxRangeY, soundY * 10, maxRangeY));

    painter.setBrush(Qt::red);
    painter.drawEllipse(QPoint(soundPosX, soundPosY), 10, 10);
}
