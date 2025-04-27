#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "video_capture.h"
#include "tremor_processor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame();
    void startRecording();
    void stopRecording();
    void analyzeTremor();

private:
    void setupUi();
    void setupConnections();

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QHBoxLayout *controlsLayout;
    QLabel *videoLabel;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *analyzeButton;
    QTimer *timer;

    VideoCapture videoCapture;
    bool isRecording;
    std::vector<cv::Mat> recordedFrames;
};

#endif // MAINWINDOW_H 