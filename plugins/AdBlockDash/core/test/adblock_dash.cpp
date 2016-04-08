#include "../adblock_dash.hpp"

#include "core/settings.hpp"
#include "utility.hpp"

#include <cassert>

#include <boost/filesystem.hpp>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>
#include <QtWebKit/QWebElement>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>

#include <gtest/gtest.h>

namespace adblock_dash { namespace test {

namespace bfs = boost::filesystem;

static QString
prepareIniDirectory()
{
    namespace bfs = boost::filesystem;
    bfs::remove("data/extensions.ini");
    return "data";
}


class AdBlockDash : public ::testing::Test
{
protected:
    AdBlockDash()
        : m_subscrided { &m_adBlockDash,
            SIGNAL(subscribed(const QString&, const QUrl&))
          }
        , m_subscribeError { &m_adBlockDash,
            SIGNAL(subscribeError(const QString&, const QUrl&, const QString&))
          }
    {}

    static void SetUpTestCase()
    {
        auto &subscriptions = m_settings.subscriptions();
        const auto &path = bfs::current_path() / "data" / "small.txt";
        const QUrl location { "http://www.google.com" };
        subscriptions.emplace_back("EasyList", path, location);
    }

    QSignalSpy *waitForSignal()
    {
        auto retry = 40u; // 250ms * 40 = 10s
        while (m_subscrided.isEmpty() && m_subscribeError.isEmpty()
                                      && --retry > 0)
        {
            QTest::qWait(250);
        }
        return !m_subscrided.isEmpty() ? &m_subscrided :
               !m_subscribeError.isEmpty() ? &m_subscribeError : nullptr;
    }

protected:
    static QNetworkAccessManager m_manager;
    static Settings m_settings;
    static adblock_dash::AdBlockDash m_adBlockDash;

    QSignalSpy m_subscrided, m_subscribeError;
};

QNetworkAccessManager AdBlockDash::m_manager;
Settings AdBlockDash::m_settings { prepareIniDirectory() };
adblock_dash::AdBlockDash
    AdBlockDash::m_adBlockDash { m_settings, m_manager };

TEST_F(AdBlockDash, ShouldBlockByElement)
{
    QWebPage page;
    auto* const frame = page.mainFrame();
    assert(frame);
    frame->setHtml(R"(
        <html>
        <body>
            <img src="http://www.adblock.org/image.png"/>
            <script src="http://www.google.com/ad.js"/>
        </body>
        </html>
    )");
    {
        auto &&element = frame->findFirstElement("img");
        ASSERT_EQ("IMG", element.tagName()) << element.tagName();

        const auto &url = element.attribute("src");
        EXPECT_TRUE(m_adBlockDash.shouldBlock(url, element));
    }
    {
        auto &&element = frame->findFirstElement("script");
        ASSERT_EQ("SCRIPT", element.tagName()) << element.tagName();

        const auto &url = element.attribute("src");
        EXPECT_FALSE(m_adBlockDash.shouldBlock(url, element));
    }
}

TEST_F(AdBlockDash, ShouldBlockByRequest)
{
    QWebPage page;
    auto* const frame = page.mainFrame();
    assert(frame);

    QNetworkRequest request;
    request.setOriginatingObject(frame);

    request.setUrl({ "http://www.adblock.org/image.png" });
    EXPECT_TRUE(m_adBlockDash.shouldBlock(request));

    request.setUrl({ "http://www.google.org/ad.js" });
    EXPECT_FALSE(m_adBlockDash.shouldBlock(request));
}

TEST_F(AdBlockDash, ElementHideCss)
{
    {
        const auto &css =
            m_adBlockDash.elementHideCss({ "http://www.adblock.org" });
        EXPECT_EQ("div { display: none !important }", css);
    }
    {
        const auto &css =
            m_adBlockDash.elementHideCss({ "http://www.google.com" });
        EXPECT_EQ("div, table { display: none !important }", css);
    }
}

TEST_F(AdBlockDash, Subscribe)
{
    const QUrl url { "http://localhost:8080/easylist.txt" };
    m_adBlockDash.subscribe("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_EQ(&m_subscrided, signal);

    const auto &args = signal->takeFirst();
    ASSERT_EQ(2               , args.size());
    ASSERT_EQ(QVariant::String, args[0].type());
    EXPECT_EQ("EasyList"      , args[0].toString());
    ASSERT_EQ(QVariant::Url   , args[1].type());
    EXPECT_EQ(url             , args[1].toUrl());
}

TEST_F(AdBlockDash, SubscribeError)
{
    const QUrl url { "http://localhost:8080/404.txt" };
    m_adBlockDash.subscribe("EasyList", url);

    auto* const signal = this->waitForSignal();
    ASSERT_EQ(&m_subscribeError, signal);

    const auto &args = signal->takeFirst();
    ASSERT_EQ(3               , args.size());
    ASSERT_EQ(QVariant::String, args[0].type());
    EXPECT_EQ("EasyList"      , args[0].toString());
    ASSERT_EQ(QVariant::Url   , args[1].type());
    EXPECT_EQ(url             , args[1].toUrl());
}

TEST_F(AdBlockDash, SimultaneousSubscribe)
{
    QUrl url { "http://localhost:8080/easylist.txt" };
    m_adBlockDash.subscribe("EasyList", url);

    url = "http://localhost:8080/small.txt";
    EXPECT_ANY_THROW(m_adBlockDash.subscribe("SmallList", url));
}

}} // namespace adblock_dash::test
