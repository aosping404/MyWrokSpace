#pragma once

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct Detection {
    std::string class_name;
    float confidence;
    float center_x;
    float center_y;
    float width;
    float height;
    cv::Rect bbox;
};

namespace my_visualization_tool {

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void setImage(const cv::Mat& image);
    void drawDetections(const std::vector<Detection>& detections);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateScaledPixmap();
    
    QLabel *image_label_;
    cv::Mat current_image_;
    QPixmap original_pixmap_;  // 保存原始图像，用于缩放
    
    QPixmap matToQPixmap(const cv::Mat& mat);
};

} // namespace my_visualization_tool
