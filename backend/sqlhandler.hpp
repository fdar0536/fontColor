#ifndef SQLHANDLER_HPP
#define SQLHANDLER_HPP

#include "QRunnable"
#include "QSqlQuery"
#include "QSqlDatabase"
#include "QObject"

class SQLHandler : public QObject, public QRunnable
{
    Q_OBJECT
    
public:
    
    SQLHandler();
    
    ~SQLHandler() override;
    
    void setqueryString(QString &);
    
    void setQuery(QSqlQuery &);
    
    void setfontFamilyList(QList<QString> &);
    
    void setfontIndex(QList<int> &);
    
    void setfontCount(int &);
    
    bool getRes() const;
    
    QSqlQuery getQuery() const;
    
    QString getlastError() const;
    
    void run() override; //for sort
    
    void exec_string();
    
    void exec_prepared();
    
signals:
    
    void done(QList<QString> &, QList<int> &);
    
private:
    
    void db_init();
    
    QSqlDatabase m_db;
    
    QSqlQuery m_query;
    
    QString m_queryString;
    
    QString m_lastError;
    
    bool m_res;
    
    QList<QString> m_fontFamilyList;
    
    QList<int> m_fontIndex;
    
    int m_fontCount;
};

#endif // SQLHANDLER_HPP
