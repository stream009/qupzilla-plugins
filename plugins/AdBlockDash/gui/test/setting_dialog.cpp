#include "../setting_dialog.hpp"
#include "../new_subscription_dialog.hpp"

#include "common/test.hpp"
#include "adblock_dash/settings.hpp"

#include <functional>

#include <boost/filesystem.hpp>

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

#include <gtest/gtest.h>

namespace adblock_dash { namespace test {

static constexpr size_t DEFAULT_TIMEOUT = 5 * 1000;

template<typename Predicate>
bool
waitFor(Predicate &&pred, const size_t timeoutMs = DEFAULT_TIMEOUT)
{
    bool result = false;
    auto waited = 0u;

    while (waited < timeoutMs) {
        if (pred()) {
            result = true;
            break;
        }

        static const auto tic = 100u;
        QTest::qWait(tic);
        waited += tic;
    }

    return result;
}

bool
waitForWidgetToBeActive(const QWidget &widget,
                        const size_t timeoutMs = DEFAULT_TIMEOUT)
{
    return waitFor([&widget] {
        return widget.isActiveWindow();
    }, timeoutMs);
}

template<typename Widget>
bool
waitForWidgetToBeActive(const size_t timeoutMs = DEFAULT_TIMEOUT)
{
    return waitFor([] {
        auto* const widget = QApplication::activeWindow();
        return dynamic_cast<Widget*>(widget);
    }, timeoutMs);
}

class Callbacker : public QObject
{
    Q_OBJECT
public:
    template<typename Function>
    Callbacker(Function &&func)
        : m_callback { std::forward<Function>(func) } {}

    void later() const
    {
        QTimer::singleShot(
            0, const_cast<Callbacker*>(this), SLOT(callBack()));
    }

private Q_SLOTS:
    void callBack() const { m_callback(); }

private:
    std::function<void()> m_callback;
};

static const char*
prepareDataDirectory()
{
    namespace bfs = boost::filesystem;

    static const auto &dirName = "data";
    bfs::remove_all(dirName);
    bfs::create_directory(dirName);
    assert(bfs::exists(dirName));

    return dirName;
}

class SettingDialog : public ::testing::Test
{
protected:
    SettingDialog()
        : m_settings { prepareDataDirectory() }
    {
        auto &subscriptions = m_settings.subscriptions();

        subscriptions.append("First" , "path/to/first");
        subscriptions.append("Second", "path/to/second");
        subscriptions.append("Third" , "path/to/thired");

        m_dialog.reset(new adblock_dash::SettingDialog {
            m_settings, m_dummyParent
        });
    }

protected:
    Settings m_settings;
    QWidget m_dummyParent;
    std::unique_ptr<adblock_dash::SettingDialog> m_dialog;
};


TEST_F(SettingDialog, Elementary)
{
    m_dialog->show();
    ASSERT_TRUE(waitForWidgetToBeActive(*m_dialog));

    auto &button = m_dialog->appendSubscription();
    EXPECT_TRUE(button.isEnabled());

    Callbacker callMeBack {
        [] {
            ASSERT_TRUE(waitForWidgetToBeActive<NewSubscriptionDialog>());
            ::QTest::keyClick(QApplication::activeWindow(), Qt::Key_Escape);
        }
    };
    callMeBack.later();

    QTest::mouseClick(&button, Qt::LeftButton);
}

TEST_F(SettingDialog, RemoveButton_EnableStatus)
{
    m_dialog->show();
    ASSERT_TRUE(waitForWidgetToBeActive(*m_dialog));

    auto &treeView = m_dialog->subscriptions();
    auto &button = m_dialog->removeSubscription();

    auto* const item = treeView.topLevelItem(0);
    assert(item);

    item->setSelected(false);
    EXPECT_FALSE(button.isEnabled());

    item->setSelected(true);
    EXPECT_TRUE(button.isEnabled());
}

TEST_F(SettingDialog, RemoveButton_Push)
{
    m_dialog->show();
    ASSERT_TRUE(waitForWidgetToBeActive(*m_dialog));

    auto &treeView = m_dialog->subscriptions();
    auto &button = m_dialog->removeSubscription();

    ASSERT_EQ(3, treeView.topLevelItemCount());
    auto* const item = treeView.topLevelItem(0);
    assert(item);
    item->setSelected(true);

    ASSERT_TRUE(button.isEnabled());

    QSignalSpy spy {
        &m_settings.subscriptions(), SIGNAL(removed(const QString&)) };
    ASSERT_EQ(0, spy.count());

    QTest::mouseClick(&button, Qt::LeftButton);

    ASSERT_EQ(1, spy.size());
    ASSERT_EQ(2, treeView.topLevelItemCount());

    const auto &args = spy.front();
    const auto &path = item->data(1, 0).toString();
    ASSERT_EQ(1               , args.size());
    EXPECT_EQ(QVariant::String, args[0].type());
    EXPECT_EQ(path            , args[0].toString());

    auto* const firstItem = treeView.topLevelItem(0);
    EXPECT_NE(path, firstItem->data(1, 0).toString());
}

}} // namespace adblock_dash::test

#include "setting_dialog.moc"
