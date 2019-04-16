#ifndef FONTDATABASE_HPP
#define FONTDATABASE_HPP

#include <vector>

#include "QObject"

using namespace std;

class FontDatabase : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(int fontCount READ fontCount)
public:
    
    explicit FontDatabase(QObject *parent = nullptr);
    
    ~FontDatabase();
    
    int fontCount() const;
    
    Q_INVOKABLE QString getFontFamily(int);
    
    Q_INVOKABLE QString getFontFilePath(int);
    
    Q_INVOKABLE QString getFontFileName(int);
    
    Q_INVOKABLE QString getFontStyle(int);
    
private:
    
    int m_fontCount;
    
    vector<QString> m_nameList;
    
    vector<QString> m_fileList;
    
    vector<QString> m_styleList;
    
    int main_process(vector<QString> &, vector<QString> &, vector<QString> &);
};

#endif // FONTDATABASE_HPP
