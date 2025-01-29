#include "audiowidget.h"
#include "soundvisualizationwidget.h"
#include <sox.h>

AudioWidget::AudioWidget(QWidget *parent)
    : QWidget(parent), fileDialog(nullptr), isPlaying(false)
{
    setMinimumSize(800, 400);
    sox_init();

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


    // Saving Files
    auto *saveLayout = new QHBoxLayout;
    auto *saveLocationLabel = new QLabel(tr("Save Location:"));
    saveLayout->addWidget(saveLocationLabel);
    saveLocationEdit = new QLineEdit; // To display the selected directory
    saveLocationEdit->setPlaceholderText(tr("Choose save location..."));
    saveLayout->addWidget(saveLocationEdit);
    saveLocationButton = new QPushButton(tr("Choose..."));
    saveLayout->addWidget(saveLocationButton);
    leftPanel->addLayout(saveLayout);

    connect(saveLocationButton, &QPushButton::clicked, this, &AudioWidget::chooseSaveLocation);
    connect(fileTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AudioWidget::fileSelected);

    // Right Panel: Knobs (Sliders)
    auto *rightPanel = new QVBoxLayout;

    auto *saveButton = new QPushButton(tr("Save Processed File"));
    rightPanel->addWidget(saveButton);

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
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        saveProcessedFile(selectedFile);  // Or another QString that you want to pass
    });

    connect(fileTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](const QItemSelection &selected, const QItemSelection &deselected) {
        fileSelected(selected, deselected);
    });

    connect(saveLocationButton, &QPushButton::clicked, this, &AudioWidget::chooseSaveLocation);

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
    sox_quit(); // Cleanup SoX
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
    if (fileDialog->exec() == QDialog::Accepted)
        fileEdit->setText(fileDialog->selectedFiles().constFirst());
}

void AudioWidget::updateRoom()
{
    const float d = roomDimension->value();
    room->setDimensions(QVector3D(d, d, 400));
    room->setReflectionGain(float(reflectionGain->value()) / 100);
    room->setReverbGain(float(reverbGain->value()) / 100);
}

void AudioWidget::fileSelected(const QItemSelection &selected, const QItemSelection &deselected) {
    (void)deselected;  // Mark as unused to avoid the warning
    QModelIndex index = selected.indexes().first();
    selectedFile = fileSystemModel->filePath(index);
    fileEdit->setText(selectedFile);
}


// Modified save functions
void AudioWidget::chooseSaveLocation() {
    QString dir = QFileDialog::getExistingDirectory(this, "Choose Save Directory",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (!dir.isEmpty()) {
        saveLocation = dir;
        saveLocationEdit->setText(dir);
    }
}

void AudioWidget::saveProcessedFile(const QString &format) {
    if (selectedFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected");
        return;
    }

    if (saveLocation.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please choose a save location first");
        return;
    }

    QFileInfo inputInfo(selectedFile);
    QString outputPath = QDir(saveLocation).filePath(inputInfo.baseName() + "_processed." + format);

    sox_format_t *in = sox_open_read(selectedFile.toUtf8().constData(), nullptr, nullptr, nullptr);
    if (!in) {
        QMessageBox::critical(this, "Error", "Failed to open input file");
        return;
    }

    // Check the encoding directly from the sox_format_t structure
    if (in->encoding.encoding != SOX_ENCODING_FLOAT) {
        QMessageBox::critical(this, "Error", "Input file must be in floating point format for spatial processing");
        sox_close(in);
        return;
    }

    QByteArray audioData;
    audioData.resize(in->signal.length);
    sox_sample_t *audioBuffer = reinterpret_cast<sox_sample_t *>(audioData.data());
    sox_read(in, audioBuffer, audioData.size() / sizeof(sox_sample_t));
    applySpatialEffects(audioData);

    sox_format_t *out = sox_open_write(outputPath.toUtf8().constData(), &in->signal, nullptr, format.toUtf8().constData(), nullptr, nullptr);
    if (!out) {
        sox_close(in);
        QMessageBox::critical(this, "Error", "Failed to create output file");
        return;
    }

    sox_write(out, audioBuffer, audioData.size() / sizeof(sox_sample_t));
    sox_close(out);
    sox_close(in);

    QMessageBox::information(this, "Success", "File saved successfully");
}

void AudioWidget::applySpatialEffects(QByteArray &audioData) {
    // Only apply effects if parameters (e.g., position) have changed
    static float lastAzimuth = azimuth->value();
    static float lastElevation = elevation->value();
    static float lastDistance = distance->value();

    // Check if any parameters have changed
    if (azimuth->value() != lastAzimuth || elevation->value() != lastElevation || distance->value() != lastDistance) {
        lastAzimuth = azimuth->value();
        lastElevation = elevation->value();
        lastDistance = distance->value();

        // Apply listener positioning, room acoustics, occlusion, etc.
        const float az = azimuth->value() / 180. * M_PI;
        const float el = elevation->value() / 180. * M_PI;
        const float d = distance->value();
        const float x = d * sin(az) * cos(el);
        const float y = d * sin(el);
        const float z = -d * cos(az) * cos(el);

        // Update the sound's position
        sound->setPosition({x, y, z});

        // Apply other spatial effects like occlusion
        sound->setOcclusionIntensity(occlusion->value() / 100.0);
        updateRoom();  // Update room effects like reverb

    }
}

void AudioWidget::animateChanged(bool checked) {
    if (checked) {
        // Only start animation if it's not already running
        if (animation->state() != QPropertyAnimation::Running) {
            animation->start();
        }
    } else {
        animation->stop();
    }
}
