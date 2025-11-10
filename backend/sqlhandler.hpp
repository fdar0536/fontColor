#ifndef SQLHANDLER_HPP
#define SQLHANDLER_HPP

#include <mutex>

#include "QRunnable"
#include "QObject"

#include "sqlite3.h"

class SQLiteInfo
{
public:

    ~SQLiteInfo();

    sqlite3 *db = nullptr;

    sqlite3_stmt *stmt = nullptr;

    std::mutex mutex;

}; // end class SQLiteToken

class SQLHandler : public QObject, public QRunnable
{
    Q_OBJECT

public:

    SQLHandler();

    ~SQLHandler() override;

    void setqueryString(QString &);

    void setfontFamilyList(QList<QString> &);

    void setfontIndex(QList<int> &);

    void setfontCount(int &);

    bool getRes() const;

    QString getlastError() const;

    SQLiteInfo *getSqlInfo();

    void run() override; //for sort

    void exec_string();

signals:

    void done(QList<QString> &, QList<int> &);

private:

    void db_init();

    SQLiteInfo m_sql;

    std::string m_queryString;

    QString m_lastError;

    bool m_res;

    QList<QString> m_fontFamilyList;

    QList<int> m_fontIndex;

    int m_fontCount;
};

#endif // SQLHANDLER_HPP
