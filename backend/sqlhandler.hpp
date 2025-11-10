/*
 * Font Color
 * Copyright (c) 2019-present fdar0536
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
