#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

#include "QObject"
#include "QClipboard"
#include "QMimeData"

class Clipboard : public QObject
{
    Q_OBJECT
    
public:
    
    explicit Clipboard(QObject *parent = nullptr);
    
    Q_INVOKABLE void copyString(QString);
    
    Q_INVOKABLE void copyFile(QString);
};

#endif // CLIPBOARD_HPP
