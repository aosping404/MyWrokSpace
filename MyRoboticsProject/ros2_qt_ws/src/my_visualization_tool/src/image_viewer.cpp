#include "my_visualization_tool/image_viewer.hpp"
#include <QVBoxLayout>
#include <QDebug>
#include <QImage>
#include <QResizeEvent>

namespace my_visualization_tool {

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , image_label_(new QLabel(this))
{
    image_label_->setAlignment(Qt::AlignCenter);
    image_label_->setText("等待图像...");
    image_label_->setScaledContents(false);  // 不使用自动缩放，手动控制以保持宽高比
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(image_label_);
    layout->setContentsMargins(0, 0, 0, 0);
    
    setLayout(layout);
    
    // 启用大小策略，让图像查看器可以扩展
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    image_label_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ImageViewer::setImage(const cv::Mat& image)
{
    if (image.empty()) {
        qWarning() << "ImageViewer: 图像为空";
        image_label_->setText("图像为空");
        return;
    }
    
    current_image_ = image.clone();
    
    // 转换 OpenCV Mat 为 QPixmap
    QPixmap pixmap = matToQPixmap(image);
    
    if (pixmap.isNull()) {
        qWarning() << "ImageViewer: QPixmap 转换失败";
        image_label_->setText("图像转换失败");
        return;
    }
    
    // 保存原始 pixmap（用于缩放）
    original_pixmap_ = pixmap;
    
    // 更新显示（会自动缩放以适应窗口）
    updateScaledPixmap();
}

void ImageViewer::updateScaledPixmap()
{
    if (original_pixmap_.isNull()) {
        return;
    }
    
    // 获取可用空间（减去边距）
    QSize available_size = image_label_->size();
    
    if (available_size.width() <= 0 || available_size.height() <= 0) {
        // 如果还没有大小，使用原始尺寸
        image_label_->setPixmap(original_pixmap_);
        return;
    }
    
    // 计算缩放比例，保持宽高比
    QSize pixmap_size = original_pixmap_.size();
    QSize scaled_size = pixmap_size.scaled(available_size, Qt::KeepAspectRatio);
    
    // 缩放 pixmap（使用平滑变换）
    QPixmap scaled_pixmap = original_pixmap_.scaled(scaled_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    image_label_->setPixmap(scaled_pixmap);
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 窗口大小改变时，更新缩放后的图像
    updateScaledPixmap();
}

void ImageViewer::drawDetections(const std::vector<Detection>& detections)
{
    if (current_image_.empty()) {
        return;
    }
    
    cv::Mat image_with_detections = current_image_.clone();
    
    for (const auto& det : detections) {
        // 绘制边界框
        cv::rectangle(image_with_detections, det.bbox,
                     cv::Scalar(0, 255, 0), 2);
        
        // 绘制标签和置信度
        std::string label = det.class_name + ": " + 
                           std::to_string(static_cast<int>(det.confidence * 100)) + "%";
        
        cv::Point text_pos(det.bbox.x, det.bbox.y - 10);
        if (text_pos.y < 0) {
            text_pos.y = det.bbox.y + 20;
        }
        
        cv::putText(image_with_detections, label, text_pos,
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }
    
    // 更新显示的图像
    QPixmap pixmap = matToQPixmap(image_with_detections);
    original_pixmap_ = pixmap;
    updateScaledPixmap();
}

QPixmap ImageViewer::matToQPixmap(const cv::Mat& mat)
{
    if (mat.empty()) {
        qWarning() << "ImageViewer: 输入图像为空";
        return QPixmap();
    }
    
    cv::Mat rgb_mat;
    
    // 先确保数据是连续的
    cv::Mat mat_cont = mat.isContinuous() ? mat : mat.clone();
    
    if (mat_cont.channels() == 1) {
        cv::cvtColor(mat_cont, rgb_mat, cv::COLOR_GRAY2RGB);
    } else if (mat_cont.channels() == 3) {
        // 从 BGR 转换为 RGB（OpenCV 使用 BGR，Qt 使用 RGB）
        cv::cvtColor(mat_cont, rgb_mat, cv::COLOR_BGR2RGB);
    } else if (mat_cont.channels() == 4) {
        cv::cvtColor(mat_cont, rgb_mat, cv::COLOR_BGRA2RGB);
    } else {
        qWarning() << "ImageViewer: 不支持的通道数:" << mat_cont.channels();
        return QPixmap();
    }
    
    // 确保 RGB 数据是连续的
    if (!rgb_mat.isContinuous()) {
        rgb_mat = rgb_mat.clone();
    }
    
    // 创建 QImage（使用深拷贝，确保数据独立）
    QImage qimg(static_cast<const uchar*>(rgb_mat.data), 
                rgb_mat.cols, 
                rgb_mat.rows, 
                static_cast<int>(rgb_mat.step), 
                QImage::Format_RGB888);
    
    // 深拷贝图像数据（重要：确保 QPixmap 拥有独立的数据副本）
    QImage qimg_copy = qimg.copy();
    
    if (qimg_copy.isNull()) {
        qWarning() << "ImageViewer: QImage 复制失败";
        return QPixmap();
    }
    
    QPixmap pixmap = QPixmap::fromImage(qimg_copy);
    
    if (pixmap.isNull()) {
        qWarning() << "ImageViewer: QPixmap 创建失败";
        return QPixmap();
    }
    
    return pixmap;
}

} // namespace my_visualization_tool
