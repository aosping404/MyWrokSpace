#pragma once

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class IkeaShowcaseWindow; }
QT_END_NAMESPACE

class IkeaShowcaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IkeaShowcaseWindow(QWidget *parent = nullptr);
    ~IkeaShowcaseWindow();

private slots:
    void onColorCardClicked(int index);
    void onIconCardClicked(int index);
    void animateColorCards();
    void animateIconCards();
    void updateAnimation();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Ui::IkeaShowcaseWindow *ui;
    void setupColorPalette();
    void setupIconGallery();
    void setupAnimations();
    void setupIkeaStyle();
    
    QList<QWidget*> m_colorCards;
    QList<QWidget*> m_iconCards;
    QPropertyAnimation* m_fadeAnimation;
    QTimer* m_animationTimer;
    int m_currentAnimationIndex;
};
