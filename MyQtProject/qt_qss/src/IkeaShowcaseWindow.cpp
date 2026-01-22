#include "ui_IkeaShowcaseWindow.h"
#include "IkeaShowcaseWindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>
#include <QAbstractAnimation>

IkeaShowcaseWindow::IkeaShowcaseWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IkeaShowcaseWindow)
    , m_fadeAnimation(nullptr)
    , m_animationTimer(new QTimer(this))
    , m_currentAnimationIndex(0)
{
    ui->setupUi(this);
    
    // 加载 IKEA 样式
    setupIkeaStyle();
    
    // 设置配色方案
    setupColorPalette();
    
    // 设置图标库
    setupIconGallery();
    
    // 设置动画
    setupAnimations();
    
    setWindowTitle("IKEA 设计风格展示 - 官方风格演示");
    resize(1400, 900);
}

IkeaShowcaseWindow::~IkeaShowcaseWindow()
{
    delete ui;
}

void IkeaShowcaseWindow::setupIkeaStyle()
{
    // 加载 IKEA QSS 样式
    QFile file(":/styles/IKEA.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        this->setStyleSheet(styleSheet);
        file.close();
    }
    
    // 设置顶部导航栏样式
    ui->topHeader->setStyleSheet(
        "QWidget#topHeader {"
        "    background-color: #0058a3;"
        "    color: #ffffff;"
        "}"
    );
    
    // 设置主导航栏样式
    ui->mainHeader->setStyleSheet(
        "QWidget#mainHeader {"
        "    background-color: #faf9fc;"
        "    border-bottom: 1px solid #dfdfdf;"
        "}"
        "QLabel#ikeaLogo {"
        "    color: #0058a3;"
        "    font-weight: bold;"
        "    padding: 20px;"
        "}"
    );
    
    // 设置搜索区域样式
    ui->searchSection->setStyleSheet(
        "QWidget#searchSection {"
        "    background-color: #f8f8f8;"
        "    border-bottom: 1px solid #e0e0e0;"
        "}"
    );
}

void IkeaShowcaseWindow::setupColorPalette()
{
    // IKEA 配色方案
    struct ColorInfo {
        QString name;
        QString color;
        QString description;
    };
    
    QList<ColorInfo> colors = {
        {"IKEA 蓝", "#0058a3", "品牌主色，代表信任与品质"},
        {"纯白", "#ffffff", "简洁明亮，营造空间感"},
        {"浅灰", "#f5f5f5", "中性色调，平衡视觉"},
        {"深灰", "#767676", "文字与边框，清晰易读"},
        {"深黑", "#111111", "标题文字，突出重点"}
    };
    
    QList<QWidget*> colorWidgets = {
        ui->colorCard1, ui->colorCard2, ui->colorCard3, 
        ui->colorCard4, ui->colorCard5
    };
    
    m_colorCards = colorWidgets;
    
    for (int i = 0; i < colorWidgets.size() && i < colors.size(); ++i) {
        QWidget* card = colorWidgets[i];
        const ColorInfo& info = colors[i];
        
        // 设置卡片颜色和样式
        card->setStyleSheet(
            QString("QWidget {"
                    "    background-color: %1;"
                    "    border: 2px solid #cccccc;"
                    "}"
                    "QWidget:hover {"
                    "    border: 2px solid #0058a3;"
                    "    transform: scale(1.05);"
                    "}").arg(info.color)
        );
        
        // 添加标签显示颜色名称
        QVBoxLayout* layout = new QVBoxLayout(card);
        layout->setContentsMargins(10, 10, 10, 10);
        
        // 根据背景颜色确定文字颜色
        // 索引0: IKEA蓝 -> 白色文字
        // 索引1: 纯白 -> 黑色文字
        // 索引2: 浅灰 -> 白色文字
        // 索引3: 深灰 -> 白色文字
        // 索引4: 深黑 -> 白色文字
        QString textColor;
        if (i == 1) {
            // 纯白背景用黑色文字
            textColor = "#111111";
        }
        else if(i == 2) {
            textColor = "#111111";
        }
        else {
            // 其他背景（深色和灰色）用白色文字
            textColor = "#ffffff";
        }
        
        QLabel* nameLabel = new QLabel(info.name, card);
        nameLabel->setStyleSheet(
            QString("QLabel {"
                    "    color: %1;"
                    "    font-weight: bold;"
                    "    font-size: 14px;"
                    "    background-color: transparent;"
                    "}").arg(textColor)
        );
        nameLabel->setAlignment(Qt::AlignCenter);
        
        QLabel* colorLabel = new QLabel(info.color, card);
        colorLabel->setStyleSheet(
            QString("QLabel {"
                    "    color: %1;"
                    "    font-size: 12px;"
                    "    background-color: transparent;"
                    "}").arg(textColor)
        );
        colorLabel->setAlignment(Qt::AlignCenter);
        
        layout->addStretch();
        layout->addWidget(nameLabel);
        layout->addWidget(colorLabel);
        layout->addStretch();
        
        card->setLayout(layout);
        card->installEventFilter(this);
        
        // 存储颜色信息到 widget 属性
        card->setProperty("colorName", info.name);
        card->setProperty("colorValue", info.color);
        card->setProperty("colorDescription", info.description);
    }
}

void IkeaShowcaseWindow::setupIconGallery()
{
    // 图标列表（使用 dark/PNG 中的图标）
    struct IconInfo {
        QString name;
        QString iconPath;
    };
    
    QList<IconInfo> icons = {
        {"首页", ":/icons/dark/PNG/User interface/home.png"},
        {"搜索", ":/icons/dark/PNG/User interface/search.png"},
        {"设置", ":/icons/dark/PNG/User interface/settings.png"},
        {"收藏", ":/icons/dark/PNG/User interface/favorite.png"},
        {"购物车", ":/icons/dark/PNG/Shopping/shopping_cart.png"},
        {"用户", ":/icons/dark/PNG/User interface/profile.png"}
    };
    
    QList<QWidget*> iconWidgets = {
        ui->iconCard1, ui->iconCard2, ui->iconCard3,
        ui->iconCard4, ui->iconCard5, ui->iconCard6
    };
    
    QList<QLabel*> iconLabels = {
        ui->iconLabel1, ui->iconLabel2, ui->iconLabel3,
        ui->iconLabel4, ui->iconLabel5, ui->iconLabel6
    };
    
    QList<QLabel*> iconNames = {
        ui->iconName1, ui->iconName2, ui->iconName3,
        ui->iconName4, ui->iconName5, ui->iconName6
    };
    
    m_iconCards = iconWidgets;
    
    for (int i = 0; i < iconWidgets.size() && i < icons.size(); ++i) {
        QWidget* card = iconWidgets[i];
        QLabel* iconLabel = iconLabels[i];
        QLabel* nameLabel = iconNames[i];
        const IconInfo& info = icons[i];
        
        // 设置图标
        QPixmap pixmap(info.iconPath);
        if (!pixmap.isNull()) {
            pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            iconLabel->setPixmap(pixmap);
        } else {
            iconLabel->setText("📦");
            iconLabel->setStyleSheet("font-size: 48px;");
        }
        
        // 设置名称
        nameLabel->setText(info.name);
        nameLabel->setStyleSheet(
            "QLabel {"
            "    color: #111111;"
            "    font-size: 12px;"
            "    font-weight: 500;"
            "    background-color: transparent;"
            "}"
        );
        
        // 设置卡片样式
        card->setStyleSheet(
            "QWidget {"
            "    background-color: #ffffff;"
            "    border: 1px solid #e0e0e0;"
            "    border-radius: 0px;"
            "}"
            "QWidget:hover {"
            "    background-color: #f5f5f5;"
            "    border: 1px solid #0058a3;"
            "}"
        );
        
        card->installEventFilter(this);
        card->setProperty("iconName", info.name);
        card->setProperty("iconPath", info.iconPath);
    }
}

void IkeaShowcaseWindow::setupAnimations()
{
    // 创建淡入淡出动画
    m_fadeAnimation = new QPropertyAnimation(this);
    m_fadeAnimation->setPropertyName("windowOpacity");
    m_fadeAnimation->setDuration(500);
    m_fadeAnimation->setStartValue(0.0);
    m_fadeAnimation->setEndValue(1.0);
    m_fadeAnimation->start();
    
    // 设置定时器用于循环动画
    connect(m_animationTimer, &QTimer::timeout, this, &IkeaShowcaseWindow::updateAnimation);
    m_animationTimer->start(2000); // 每2秒切换一次
}

void IkeaShowcaseWindow::updateAnimation()
{
    // 循环高亮显示不同的颜色卡片
    if (m_colorCards.isEmpty()) return;
    
    static int direction = 1;
    m_currentAnimationIndex += direction;
    
    if (m_currentAnimationIndex >= m_colorCards.size()) {
        m_currentAnimationIndex = m_colorCards.size() - 1;
        direction = -1;
    } else if (m_currentAnimationIndex < 0) {
        m_currentAnimationIndex = 0;
        direction = 1;
    }
    
    // 重置所有卡片
    for (QWidget* card : m_colorCards) {
        QString color = card->property("colorValue").toString();
        card->setStyleSheet(
            QString("QWidget {"
                    "    background-color: %1;"
                    "    border: 2px solid #cccccc;"
                    "}").arg(color)
        );
    }
    
    // 高亮当前卡片
    if (m_currentAnimationIndex < m_colorCards.size()) {
        QWidget* currentCard = m_colorCards[m_currentAnimationIndex];
        QString color = currentCard->property("colorValue").toString();
        currentCard->setStyleSheet(
            QString("QWidget {"
                    "    background-color: %1;"
                    "    border: 3px solid #0058a3;"
                    "}").arg(color)
        );
        
        // 更新信息标签
        QString name = currentCard->property("colorName").toString();
        QString desc = currentCard->property("colorDescription").toString();
        ui->colorInfoLabel->setText(
            QString("<b>%1</b> (%2)<br>%3").arg(name, color, desc)
        );
    }
}

bool IkeaShowcaseWindow::eventFilter(QObject* obj, QEvent* event)
{
    // 处理颜色卡片点击
    if (m_colorCards.contains(qobject_cast<QWidget*>(obj))) {
        if (event->type() == QEvent::MouseButtonPress) {
            QWidget* card = qobject_cast<QWidget*>(obj);
            onColorCardClicked(m_colorCards.indexOf(card));
            return true;
        }
    }
    
    // 处理图标卡片点击
    if (m_iconCards.contains(qobject_cast<QWidget*>(obj))) {
        if (event->type() == QEvent::MouseButtonPress) {
            QWidget* card = qobject_cast<QWidget*>(obj);
            onIconCardClicked(m_iconCards.indexOf(card));
            return true;
        }
    }
    
    return QMainWindow::eventFilter(obj, event);
}

void IkeaShowcaseWindow::onColorCardClicked(int index)
{
    if (index < 0 || index >= m_colorCards.size()) return;
    
    QWidget* card = m_colorCards[index];
    QString name = card->property("colorName").toString();
    QString color = card->property("colorValue").toString();
    QString desc = card->property("colorDescription").toString();
    
    // 更新信息显示
    ui->colorInfoLabel->setText(
        QString("<b style='font-size: 16px;'>%1</b><br>"
                "<span style='font-size: 14px; color: %2;'>%2</span><br>"
                "<span style='font-size: 12px; color: #767676;'>%3</span>")
        .arg(name, color, desc)
    );
    
    // 创建点击动画效果
    QPropertyAnimation* anim = new QPropertyAnimation(card, "geometry");
    anim->setDuration(200);
    anim->setStartValue(card->geometry());
    QRect endValue = card->geometry();
    endValue.adjust(-5, -5, 5, 5);
    anim->setEndValue(endValue);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void IkeaShowcaseWindow::onIconCardClicked(int index)
{
    if (index < 0 || index >= m_iconCards.size()) return;
    
    QWidget* card = m_iconCards[index];
    QString name = card->property("iconName").toString();
    QString path = card->property("iconPath").toString();
    
    QMessageBox::information(this, "图标信息", 
        QString("图标名称: %1\n图标路径: %2").arg(name, path));
}

void IkeaShowcaseWindow::animateColorCards()
{
    // 已通过定时器实现
}

void IkeaShowcaseWindow::animateIconCards()
{
    // 图标卡片动画
    for (QWidget* card : m_iconCards) {
        QPropertyAnimation* anim = new QPropertyAnimation(card, "geometry");
        anim->setDuration(300);
        anim->setStartValue(card->geometry());
        QRect endValue = card->geometry();
        endValue.adjust(-2, -2, 2, 2);
        anim->setEndValue(endValue);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
