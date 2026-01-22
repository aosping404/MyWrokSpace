#pragma once

#include <QWidget>

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoardWidget(QWidget *parent = nullptr);
    
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    static const int BOARD_SIZE = 8; // 8x8 棋盘
};
