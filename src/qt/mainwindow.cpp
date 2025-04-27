#include "mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , videoCapture(0)
    , isRecording(false)
{
    setupUi();
    setupConnections();
    
    // Setup timer for video update
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(33); // ~30 fps
}

void MainWindow::setupUi() {
    // Create central widget and layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QVBoxLayout(centralWidget);
    
    // Create video display
    videoLabel = new QLabel(this);
    videoLabel->setMinimumSize(640, 480);
    videoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(videoLabel);
    
    // Create controls
    controlsLayout = new QHBoxLayout();
    
    startButton = new QPushButton("Start Recording", this);
    stopButton = new QPushButton("Stop Recording", this);
    analyzeButton = new QPushButton("Analyze Tremor", this);
    
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(stopButton);
    controlsLayout->addWidget(analyzeButton);
    
    layout->addLayout(controlsLayout);
    
    // Create status bar
    statusBar()->showMessage("Ready");
}

void MainWindow::setupConnections() {
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startRecording);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopRecording);
    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzeTremor);
}

void MainWindow::updateFrame() {
    bool success;
    cv::Mat frame;
    std::tie(success, frame) = videoCapture.get_frame();
    
    if (success) {
        // Convert OpenCV Mat to QImage
        QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        videoLabel->setPixmap(QPixmap::fromImage(qImage.rgbSwapped()));
    }
}

void MainWindow::startRecording() {
    if (videoCapture.start_recording()) {
        isRecording = true;
        statusBar()->showMessage("Recording...");
        startButton->setEnabled(false);
        stopButton->setEnabled(true);
    }
}

void MainWindow::stopRecording() {
    auto frames = videoCapture.stop_recording();
    if (frames) {
        isRecording = false;
        statusBar()->showMessage("Recording stopped");
        startButton->setEnabled(true);
        stopButton->setEnabled(false);
        recordedFrames = *frames;
    }
}

void MainWindow::analyzeTremor() {
    if (recordedFrames.empty()) {
        QMessageBox::warning(this, "Error", "No recorded frames to analyze");
        return;
    }
    
    // Process frames using C++ processor
    TremorProcessor processor;
    std::vector<cv::Point2f> allPoints;
    
    for (const auto& frame : recordedFrames) {
        auto points = processor.track_hand(frame);
        allPoints.insert(allPoints.end(), points.begin(), points.end());
    }
    
    double intensity = processor.calculate_tremor_intensity(allPoints);
    
    // Display results
    QString message = QString("Tremor Intensity: %1").arg(intensity);
    statusBar()->showMessage(message);
    QMessageBox::information(this, "Analysis Results", message);
}

MainWindow::~MainWindow() {
    videoCapture.release();
} 