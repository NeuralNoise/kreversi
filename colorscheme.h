#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QDeclarativeItem>
#include <KColorScheme>
#include <QColor>

class ColorScheme : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QColor background READ background NOTIFY placeHolder)
    Q_PROPERTY(QColor foreground READ foreground NOTIFY placeHolder)
    Q_PROPERTY(QColor border READ border NOTIFY placeHolder)

public:
    ColorScheme(QDeclarativeItem *parent = 0);

    QColor background() const;
    QColor foreground() const;
    QColor border() const;
    
signals:
    void placeHolder();
};

#endif // COLORSCHEME_H
