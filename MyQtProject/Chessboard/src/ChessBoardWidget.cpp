#include "ChessBoardWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QFont>

ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
}

void ChessBoardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int width = this->width();
    int height = this->height();
    
    int cellSize = qMin(width, height) / BOARD_SIZE;

    int boardWidth = cellSize * BOARD_SIZE;
    int boardHeight = cellSize * BOARD_SIZE;
    int startX = (width - boardWidth) / 2; //居中
    int startY = (height - boardHeight) / 2;
    
    QColor lightColor(255, 255, 255); 
    QColor darkColor(0, 0, 0); 
    QColor lightTextColor(0, 0, 0); 
    QColor darkTextColor(255, 255, 255); 
    
    int queenPositions[] = {0, 4, 7, 5, 2, 6, 1, 3};
    QString queenNumbers = "15863724";

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            bool isLight = (row + col) % 2 == 0;
            QColor cellColor = isLight ? lightColor : darkColor;

            int x = startX + col * cellSize;
            int y = startY + row * cellSize;
            painter.fillRect(x, y, cellSize, cellSize, cellColor); //格子
            
            //边框
            painter.setPen(QPen(QColor(139, 115, 85), 2));
            painter.drawRect(x, y, cellSize, cellSize);

            if (queenPositions[col] == row) {
                QColor textColor = isLight ? lightTextColor : darkTextColor;
                painter.setPen(textColor);
    
                QFont font = painter.font();
                font.setPointSize(cellSize / 3); //字体动态大小
                font.setBold(true);
                painter.setFont(font);
                
                QString number = queenNumbers.mid(col, 1);
                
                //文字居中
                QRect textRect(x, y, cellSize, cellSize);
                painter.drawText(textRect, Qt::AlignCenter, number);
            }
        }
    }
}
