#include "../downloader.hpp"
#include "adblock_dash/settings.hpp"

#include "utility.hpp"

#include <chrono>
#include <fstream>
#include <thread>

#include <boost/filesystem.hpp>

#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include <gtest/gtest.h>

namespace adblock_dash { namespace test {

namespace bfs = boost::filesystem;

static bfs::path
prepareDownloadDirectory()
{
    namespace bfs = boost::filesystem;
    static const char name[] = "download";
    bfs::remove_all(name);
    assert(!bfs::exists(name));

    bfs::create_directory(name);
    assert(bfs::exists(name));

    return bfs::absolute(name);
}


class Downloader : public ::testing::Test
{
protected:
    Downloader()
        : m_settings { "data" },
          m_downloader { m_settings, m_manager },
          m_finished { &m_downloader,
            SIGNAL(finished(const QString&, const QUrl&, const QString&)) },
          m_error { &m_downloader,
            SIGNAL(error(const QString&, const QUrl&, const QString&)) }
    {
        assert(m_finished.isValid());
        assert(m_error.isValid());

        m_settings.directory = prepareDownloadDirectory().c_str();
    }

    QSignalSpy *waitForSignal()
    {
        auto retry = 40u; // 250ms * 40 = 10s
        while (m_finished.isEmpty() && m_error.isEmpty() && --retry > 0) {
            QTest::qWait(250);
        }
        return !m_finished.isEmpty() ? &m_finished :
               !m_error.isEmpty() ? &m_error : nullptr;
    }

protected:
    QNetworkAccessManager m_manager;
    Settings m_settings;
    adblock_dash::Downloader m_downloader;
    QSignalSpy m_finished, m_error;
};


TEST_F(Downloader, Elementary)
{
    const QUrl &url { "http://localhost:8080/easylist.txt" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3                                     , args.size());
    ASSERT_EQ(QVariant::String                      , args.at(0).type());
    EXPECT_EQ("EasyList"                            , args.at(0).toString());
    ASSERT_EQ(QVariant::Url                         , args.at(1).type());
    EXPECT_EQ(url                                   , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String                      , args.at(2).type());
    EXPECT_EQ(m_settings.directory + "/easylist.txt", args.at(2).toString());
}

TEST_F(Downloader, WrongURL)
{
    const QUrl &url { "http://localhost:8080/nonexistent.txt" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr , signal);
    EXPECT_EQ(&m_error, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3               , args.size());
    ASSERT_EQ(QVariant::String, args.at(0).type());
    EXPECT_EQ("EasyList"      , args.at(0).toString());
    ASSERT_EQ(QVariant::Url   , args.at(1).type());
    EXPECT_EQ(url             , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String, args.at(2).type());
}

TEST_F(Downloader, WrongExtension)
{
    const QUrl &url { "http://localhost:8080/easylist.lst" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3                                     , args.size());
    ASSERT_EQ(QVariant::String                      , args.at(0).type());
    EXPECT_EQ("EasyList"                            , args.at(0).toString());
    ASSERT_EQ(QVariant::Url                         , args.at(1).type());
    EXPECT_EQ(url                                   , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String                      , args.at(2).type());
    EXPECT_EQ(m_settings.directory + "/easylist.txt", args.at(2).toString());
}

TEST_F(Downloader, NoExtension)
{
    const QUrl &url { "http://localhost:8080/easylist" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3                                     , args.size());
    ASSERT_EQ(QVariant::String                      , args.at(0).type());
    EXPECT_EQ("EasyList"                            , args.at(0).toString());
    ASSERT_EQ(QVariant::Url                         , args.at(1).type());
    EXPECT_EQ(url                                   , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String                      , args.at(2).type());
    EXPECT_EQ(m_settings.directory + "/easylist.txt", args.at(2).toString());
}

TEST_F(Downloader, DirectoryOnly)
{
    const QUrl &url { "http://localhost:8080/foo/" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3                        , args.size());
    ASSERT_EQ(QVariant::String         , args.at(0).type());
    EXPECT_EQ("EasyList"               , args.at(0).toString());
    ASSERT_EQ(QVariant::Url            , args.at(1).type());
    EXPECT_EQ(url                      , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String         , args.at(2).type());
    EXPECT_EQ(m_settings.directory +
                        "/EasyList.txt", args.at(2).toString());
}

TEST_F(Downloader, NoPath)
{
    const QUrl &url { "http://localhost:8080" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3                                     , args.size());
    ASSERT_EQ(QVariant::String                      , args.at(0).type());
    EXPECT_EQ("EasyList"                            , args.at(0).toString());
    ASSERT_EQ(QVariant::Url                         , args.at(1).type());
    EXPECT_EQ(url                                   , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String                      , args.at(2).type());
    EXPECT_EQ(m_settings.directory + "/EasyList.txt", args.at(2).toString());
}

TEST_F(Downloader, NoSimultaniousDownload)
{
    const QUrl &url { "http://localhost:8080/easylist.txt" };
    m_downloader("EasyList", url);

    EXPECT_ANY_THROW(m_downloader("EasyList2", url));

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3                                     , args.size());
    ASSERT_EQ(QVariant::String                      , args.at(0).type());
    EXPECT_EQ("EasyList"                            , args.at(0).toString());
    ASSERT_EQ(QVariant::Url                         , args.at(1).type());
    EXPECT_EQ(url                                   , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String                      , args.at(2).type());
    EXPECT_EQ(m_settings.directory + "/easylist.txt", args.at(2).toString());
}

TEST_F(Downloader, OverWriteExistingFile)
{
    const auto &destination = m_settings.directory + "/easylist.txt";
    const auto &destinationUtf8 = destination.toUtf8();
    std::ofstream ofs { destinationUtf8 };
    ofs << "something irrelevant";
    ofs.close();
    ASSERT_TRUE(bfs::exists(destinationUtf8.constData()));
    const auto &oldSize = bfs::file_size(destinationUtf8.constData());

    const QUrl url { "http://localhost:8080/easylist.txt" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_finished, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3               , args.size());
    ASSERT_EQ(QVariant::String, args.at(0).type());
    EXPECT_EQ("EasyList"      , args.at(0).toString());
    ASSERT_EQ(QVariant::Url   , args.at(1).type());
    EXPECT_EQ(url             , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String, args.at(2).type());
    EXPECT_EQ(destination     , args.at(2).toString());

    ASSERT_TRUE(bfs::exists(destinationUtf8.constData()));
    EXPECT_NE(oldSize, bfs::file_size(destinationUtf8.constData()));
}

TEST_F(Downloader, CantWriteFile)
{
    const auto &destination = m_settings.directory + "/easylist.txt";
    ASSERT_TRUE(bfs::create_directory(destination.toUtf8().constData()));

    const QUrl &url { "http://localhost:8080/easylist.txt" };
    m_downloader("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_NE(nullptr, signal);
    EXPECT_EQ(&m_error, signal);

    const auto &args = signal->takeFirst();

    ASSERT_EQ(3               , args.size());
    ASSERT_EQ(QVariant::String, args.at(0).type());
    EXPECT_EQ("EasyList"      , args.at(0).toString());
    ASSERT_EQ(QVariant::Url   , args.at(1).type());
    EXPECT_EQ(url             , args.at(1).toUrl());
    ASSERT_EQ(QVariant::String, args.at(2).type());
}

}} // namespace adblock_dash::test
