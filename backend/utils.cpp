#include "QMessageBox"

#include "utils.hpp"

Utils::Utils(QObject *parent) : QObject(parent)
{}

QString Utils::convertToAssColor(QString rgb)
{
    QString r, g, b;
    r = rgb.mid(1, 2);
    g = rgb.mid(3, 2);
    b = rgb.mid(5, 2);
    return (b + g + r).toUpper();
}

void Utils::aboutQt()
{
    QWidget *tmp = new QWidget(nullptr);
    QMessageBox::aboutQt(tmp);
    delete tmp;
}
