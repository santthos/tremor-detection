#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include "tremor_processor.h"

TremorProcessor::TremorProcessor() {
    // Initialize any required parameters
    smoothing_window = 5;
    min_contour_area = 100;
}

std::vector<cv::Point2f> TremorProcessor::track_hand(const cv::Mat& frame) {
    std::vector<cv::Point2f> hand_points;
    
    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    
    // Apply threshold
    cv::Mat thresh;
    cv::threshold(gray, thresh, 127, 255, cv::THRESH_BINARY_INV);
    
    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // Find the largest contour (hand)
    if (!contours.empty()) {
        auto largest_contour = std::max_element(contours.begin(), contours.end(),
            [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                return cv::contourArea(a) < cv::contourArea(b);
            });
            
        if (cv::contourArea(*largest_contour) > min_contour_area) {
            // Get the center of mass
            cv::Moments m = cv::moments(*largest_contour);
            if (m.m00 != 0) {
                cv::Point2f center(m.m10 / m.m00, m.m01 / m.m00);
                hand_points.push_back(center);
            }
        }
    }
    
    return hand_points;
}

double TremorProcessor::calculate_tremor_intensity(const std::vector<cv::Point2f>& points) {
    if (points.size() < 2) return 0.0;
    
    // Calculate velocity between consecutive points
    std::vector<double> velocities;
    for (size_t i = 1; i < points.size(); ++i) {
        double dx = points[i].x - points[i-1].x;
        double dy = points[i].y - points[i-1].y;
        double velocity = std::sqrt(dx*dx + dy*dy);
        velocities.push_back(velocity);
    }
    
    // Apply smoothing
    std::vector<double> smoothed_velocities = smooth_velocities(velocities);
    
    // Calculate tremor intensity as the standard deviation of velocities
    double mean = 0.0;
    for (double v : smoothed_velocities) {
        mean += v;
    }
    mean /= smoothed_velocities.size();
    
    double variance = 0.0;
    for (double v : smoothed_velocities) {
        variance += (v - mean) * (v - mean);
    }
    variance /= smoothed_velocities.size();
    
    return std::sqrt(variance);
}

std::vector<double> TremorProcessor::smooth_velocities(const std::vector<double>& velocities) {
    std::vector<double> smoothed;
    int n = velocities.size();
    
    for (int i = 0; i < n; ++i) {
        double sum = 0.0;
        int count = 0;
        
        for (int j = -smoothing_window/2; j <= smoothing_window/2; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < n) {
                sum += velocities[idx];
                count++;
            }
        }
        
        smoothed.push_back(sum / count);
    }
    
    return smoothed;
} 