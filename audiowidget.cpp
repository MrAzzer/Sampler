#include "audiowidget.h"
#include "soundvisualizationwidget.h"

AudioWidget::AudioWidget(QWidget *parent)
    : QWidget(parent), fileDialog(nullptr), isPlaying(false)
{
    setMinimumSize(800, 400);

    auto *mainLayout = new QHBoxLayout(this);

    // Left Panel: File and Save Location
    auto *leftPanel = new QVBoxLayout;

    // File upload section
    auto *fileLayout = new QHBoxLayout;
    fileEdit = new QLineEdit;
    fileEdit->setPlaceholderText(tr("Audio File"));
    fileLayout->addWidget(fileEdit);
    fileDialogButton = new QPushButton(tr("Choose..."));
    fileLayout->addWidget(fileDialogButton);
    leftPanel->addLayout(fileLayout);

    // File tree view
    fileTreeView = new QTreeView;
    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setRootPath("/Users/spikespiegel/Library/CloudStorage/OneDrive-UniwersytetWrocławski/Uwr/Sampler/Spatial-Audio-Plugin/sample");
    fileTreeView->setModel(fileSystemModel);
    fileTreeView->setRootIndex(fileSystemModel->index("/Users/spikespiegel/Library/CloudStorage/OneDrive-UniwersytetWrocławski/Uwr/Sampler/Spatial-Audio-Plugin/sample"));
    leftPanel->addWidget(fileTreeView);
    mainLayout->addLayout(leftPanel);

    connect(fileTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AudioWidget::fileSelected);

    // Right Panel: Knobs (Sliders)
    auto *rightPanel = new QVBoxLayout;


    // Center Panel: 3D Visualization
    auto *centerPanel = new QWidget;
    centerPanel->setStyleSheet("background-color: #333;"); // Dark background for visualization
    auto *centerLayout = new QVBoxLayout(centerPanel);

    // Add a label for the visualization
    auto *visualizationLabel = new QLabel(tr("Sound Source Visualization"));
    visualizationLabel->setAlignment(Qt::AlignCenter);
    visualizationLabel->setStyleSheet("color: white; font-size: 16px;");
    centerLayout->addWidget(visualizationLabel);

    // Add the custom visualization widget
    soundVisualizationWidget = new SoundVisualizationWidget;
    centerLayout->addWidget(soundVisualizationWidget);
    mainLayout->addWidget(centerPanel);

    // Right Panel: Knobs (Sliders)
    rightPanel->addWidget(new QLabel(tr("Azimuth:")));
    azimuth = new QSlider(Qt::Horizontal);
    azimuth->setRange(-180, 180);
    rightPanel->addWidget(azimuth);

    rightPanel->addWidget(new QLabel(tr("Elevation (-90 - 90 degree):")));
    elevation = new QSlider(Qt::Horizontal);
    elevation->setRange(-90, 90);
    rightPanel->addWidget(elevation);

    rightPanel->addWidget(new QLabel(tr("Distance (0 - 10 meter):")));
    distance = new QSlider(Qt::Horizontal);
    distance->setRange(0, 1000);
    distance->setValue(100);
    rightPanel->addWidget(distance);

    rightPanel->addWidget(new QLabel(tr("Occlusion (0 - 4):")));
    occlusion = new QSlider(Qt::Horizontal);
    occlusion->setRange(0, 400);
    rightPanel->addWidget(occlusion);

    rightPanel->addWidget(new QLabel(tr("Room dimension (0 - 100 meter):")));
    roomDimension = new QSlider(Qt::Horizontal);
    roomDimension->setRange(0, 10000);
    roomDimension->setValue(1000);
    rightPanel->addWidget(roomDimension);

    rightPanel->addWidget(new QLabel(tr("Reverb gain (0-5):")));
    reverbGain = new QSlider(Qt::Horizontal);
    reverbGain->setRange(0, 500);
    reverbGain->setValue(0);
    rightPanel->addWidget(reverbGain);

    rightPanel->addWidget(new QLabel(tr("Reflection gain (0-5):")));
    reflectionGain = new QSlider(Qt::Horizontal);
    reflectionGain->setRange(0, 500);
    reflectionGain->setValue(0);
    rightPanel->addWidget(reflectionGain);

    rightPanel->addWidget(new QLabel(tr("Output mode:")));
    mode = new QComboBox;
    mode->addItem(tr("Surround"), QVariant::fromValue(QAudioEngine::Surround));
    mode->addItem(tr("Stereo"), QVariant::fromValue(QAudioEngine::Stereo));
    mode->addItem(tr("Headphone"), QVariant::fromValue(QAudioEngine::Headphone));
    rightPanel->addWidget(mode);

    animateButton = new QCheckBox(tr("Animate sound position"));
    rightPanel->addWidget(animateButton);

    playButton = new QPushButton(tr("Play Audio"));
    playButton->setEnabled(false);
    rightPanel->addWidget(playButton);

    mainLayout->addLayout(rightPanel);

    // Connect signals and slots
    connect(playButton, &QPushButton::clicked, this, &AudioWidget::playAudio);
    connect(fileEdit, &QLineEdit::textChanged, this, &AudioWidget::fileChanged);
    connect(fileDialogButton, &QPushButton::clicked, this, &AudioWidget::openFileDialog);
    connect(azimuth, &QSlider::valueChanged, this, &AudioWidget::updatePosition);
    connect(elevation, &QSlider::valueChanged, this, &AudioWidget::updatePosition);
    connect(distance, &QSlider::valueChanged, this, &AudioWidget::updatePosition);
    connect(occlusion, &QSlider::valueChanged, this, &AudioWidget::newOcclusion);
    connect(roomDimension, &QSlider::valueChanged, this, &AudioWidget::updateRoom);
    connect(reverbGain, &QSlider::valueChanged, this, &AudioWidget::updateRoom);
    connect(reflectionGain, &QSlider::valueChanged, this, &AudioWidget::updateRoom);
    connect(mode, &QComboBox::currentIndexChanged, this, &AudioWidget::modeChanged);
    connect(animateButton, &QCheckBox::toggled, this, &AudioWidget::animateChanged);

    connect(fileTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](const QItemSelection &selected, const QItemSelection &deselected) {
        fileSelected(selected, deselected);
    });

    // Initialize audio components
    room = new QAudioRoom(&engine);
    room->setWallMaterial(QAudioRoom::BackWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::FrontWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::LeftWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::RightWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::Floor, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::Ceiling, QAudioRoom::Marble);
    updateRoom();

    listener = new QAudioListener(&engine);
    listener->setPosition({});
    listener->setRotation({});
    engine.start();

    sound = new QSpatialSound(&engine);
    updatePosition();

    animation = new QPropertyAnimation(azimuth, "value");
    animation->setDuration(10000);
    animation->setStartValue(-180);
    animation->setEndValue(180);
    animation->setLoopCount(-1);
}

AudioWidget::~AudioWidget() {
}

void AudioWidget::setFile(const QString &file)
{
    fileEdit->setText(file);
}

void AudioWidget::playAudio()
{
    if (!sound || fileEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No valid audio file selected for playback."));
        return;
    }
    if (isPlaying) {
        sound->stop();
        isPlaying = false;
        playButton->setText(tr("Play Audio"));
    } else {
        if (QFile::exists(fileEdit->text())) {
            sound->play();
            isPlaying = true;
            playButton->setText(tr("Stop Audio"));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("The selected audio file does not exist."));
        }
    }
}

void AudioWidget::updatePosition()
{
    const float az = azimuth->value() / 180. * M_PI;
    const float el = elevation->value() / 180. * M_PI;
    const float d = distance->value();
    const float x = d * sin(az) * cos(el);
    const float y = d * sin(el);
    const float z = -d * cos(az) * cos(el);
    sound->setPosition({x, y, z});
    soundVisualizationWidget->setSoundPosition(x, y, z);
}

void AudioWidget::newOcclusion()
{
    sound->setOcclusionIntensity(occlusion->value() / 100.);
}

void AudioWidget::modeChanged()
{
    engine.setOutputMode(mode->currentData().value<QAudioEngine::OutputMode>());
}

void AudioWidget::fileChanged(const QString &file)
{
    if (QFile::exists(file)) {
        sound->setSource(QUrl::fromLocalFile(file));
        sound->setSize(5);
        sound->setLoops(QSpatialSound::Infinite);
        playButton->setEnabled(true);
    } else {
        playButton->setEnabled(false);
    }
}

void AudioWidget::openFileDialog()
{
    if (fileDialog == nullptr) {
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
        fileDialog = new QFileDialog(this, tr("Open Audio File"), dir);
        fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
        const QStringList mimeTypes{"audio/mpeg", "audio/aac", "audio/x-ms-wma", "audio/x-flac+ogg", "audio/x-wav"};
        fileDialog->setMimeTypeFilters(mimeTypes);
        fileDialog->selectMimeTypeFilter(mimeTypes.constFirst());
    }
    if (fileDialog->exec() == QDialog::Accepted) {
        QString selectedDirectory = QFileInfo(fileDialog->selectedFiles().constFirst()).absolutePath();

        fileSystemModel->setRootPath(selectedDirectory);
        fileTreeView->setRootIndex(fileSystemModel->index(selectedDirectory));
    }
}


void AudioWidget::updateRoom()
{
    const float d = roomDimension->value();
    room->setDimensions(QVector3D(d, d, 400));
    room->setReflectionGain(float(reflectionGain->value()) / 100);
    room->setReverbGain(float(reverbGain->value()) / 100);
}

void AudioWidget::fileSelected(const QItemSelection &selected, const QItemSelection &deselected)
{
    (void)deselected;
    QModelIndex index = selected.indexes().first();
    selectedFile = fileSystemModel->filePath(index);
    fileEdit->setText(selectedFile);
}


void AudioWidget::applySpatialEffects(QByteArray &audioData, sox_rate_t /*sampleRate*/, unsigned /*channels*/) {
    float *audioBuffer = reinterpret_cast<float *>(audioData.data());
    size_t numSamples = audioData.size() / sizeof(float);

    const float d = distance->value();
    float gain = 1.0f / (1.0f + d); // Simple distance attenuation
    for (size_t i = 0; i < numSamples; ++i) {
        audioBuffer[i] *= gain;
    }
}
void AudioWidget::animateChanged(bool checked) {
    if (checked) {
        if (animation->state() != QPropertyAnimation::Running) {
            animation->start();
        }
    } else {
        animation->stop();
    }
}
