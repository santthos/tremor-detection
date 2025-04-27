#ifndef TREMOR_PROCESSOR_H
#define TREMOR_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

class TremorProcessor {
public:
    TremorProcessor();
    
    // Track hand movement in a frame
    std::vector<cv::Point2f> track_hand(const cv::Mat& frame);
    
    // Calculate tremor intensity from tracked points
    double calculate_tremor_intensity(const std::vector<cv::Point2f>& points);
    
private:
    // Smooth the velocity data
    std::vector<double> smooth_velocities(const std::vector<double>& velocities);
    
    int smoothing_window;
    int min_contour_area;
};

#endif // TREMOR_PROCESSOR_H 