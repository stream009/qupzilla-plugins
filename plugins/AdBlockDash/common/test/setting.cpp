#include "../setting.hpp"
#include "../vector_setting.hpp"

#include <boost/filesystem.hpp>

#include <QtCore/QSettings>
#include <QtTest/QSignalSpy>

#include <gtest/gtest.h>

const char*
prepareTestFile()
{
    namespace bfs = boost::filesystem;
    static const char filename[] = "test.ini";
    bfs::remove(filename);
    assert(!bfs::exists(filename));

    return filename;
}

struct TestSettings : public QSettings
{
    TestSettings()
        : QSettings { prepareTestFile(), QSettings::IniFormat }
    {
        this->beginGroup("test");
    }
};

TEST(BoolSetting, Elementrary)
{
    TestSettings settings;

    BoolSetting bool1 { settings, "test", false };
    ASSERT_EQ(false, bool1.value());

    bool1.setValue(true);
    ASSERT_EQ(true, bool1.value());
}

TEST(BoolSetting, Operator)
{
    TestSettings settings;

    BoolSetting boolSetting { settings, "test", false };
    ASSERT_EQ(false, boolSetting);

    boolSetting = true;
    ASSERT_EQ(true, boolSetting);
}

class BoolWatcher : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void onChange(const bool value)
    {
        m_signalReceived = true;
        m_value = value;
    }

public:
    bool m_signalReceived = false;
    bool m_value = false;
};

TEST(BoolSetting, Signal)
{
    TestSettings settings;
    BoolWatcher watcher;

    BoolSetting setting { settings, "test", false };
    ASSERT_EQ(false, setting);

    QObject::connect(&setting, SIGNAL(changed(bool)),
                     &watcher,   SLOT(onChange(bool)));
    ASSERT_EQ(false, watcher.m_signalReceived);
    ASSERT_EQ(false, watcher.m_value);

    setting = true;
    ASSERT_EQ(true, watcher.m_signalReceived);
    EXPECT_EQ(true, watcher.m_value);

    watcher.m_signalReceived = false;
    setting = false;
    ASSERT_EQ(true, watcher.m_signalReceived);
    EXPECT_EQ(false, watcher.m_value);
}

TEST(StringSetting, Elementary)
{
    TestSettings settings;

    StringSetting setting { settings, "test", "default" };
    ASSERT_EQ("default", setting.value());

    setting.setValue("brah brah brah");
    EXPECT_EQ("brah brah brah", setting.value());
}

TEST(StringSetting, Operator)
{
    TestSettings settings;

    StringSetting setting { settings, "test", "default" };
    ASSERT_EQ("default", setting);

    setting = "brah brah brah";
    EXPECT_EQ("brah brah brah", setting);
}

class StringWatcher : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void onChange(const QString &value)
    {
        m_signalReceived = true;
        m_value = value;
    }

public:
    bool m_signalReceived = false;
    QString m_value;
};

TEST(StringSetting, Signal)
{
    TestSettings settings;
    StringWatcher watcher;

    StringSetting setting { settings, "test", "default" };
    ASSERT_EQ("default", setting);

    QObject::connect(&setting, SIGNAL(changed(const QString&)),
                     &watcher,   SLOT(onChange(const QString&)));
    ASSERT_EQ(false, watcher.m_signalReceived);
    ASSERT_EQ(true, watcher.m_value.isNull());

    setting = "whatever";
    ASSERT_EQ(true, watcher.m_signalReceived);
    EXPECT_EQ("whatever", watcher.m_value);

    watcher.m_signalReceived = false;
    watcher.m_value = "";
    setting = "chill";
    ASSERT_EQ(true, watcher.m_signalReceived);
    EXPECT_EQ("chill", watcher.m_value);
}

TEST(VectorSetting, ConvertibleToQVariant)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    EXPECT_TRUE(v.empty());
}

TEST(VectorSetting, push_back)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    ASSERT_TRUE(v.empty());

    v.push_back(100);
    EXPECT_EQ(1, v.size());
}

TEST(VectorSetting, at)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    ASSERT_TRUE(v.empty());

    v.push_back(100);
    EXPECT_EQ(1, v.size());
    EXPECT_EQ(100, v.at(0));
    EXPECT_EQ(100, v[0]);
}

TEST(VectorSetting, insert)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    ASSERT_TRUE(v.empty());

    v.insert(v.begin(), 100);
    v.insert(v.begin(), 200);

    EXPECT_EQ(2, v.size());
    EXPECT_EQ(200, v.at(0));
    EXPECT_EQ(100, v.at(1));
}

TEST(VectorSetting, emplace)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    ASSERT_TRUE(v.empty());

    v.emplace(v.begin(), 100);
    v.emplace(v.begin(), 200);

    EXPECT_EQ(2, v.size());
    EXPECT_EQ(200, v.at(0));
    EXPECT_EQ(100, v.at(1));
}

TEST(VectorSetting, erase)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    ASSERT_TRUE(v.empty());
    v.emplace_back(100);
    ASSERT_EQ(1, v.size());

    v.erase(v.begin(), v.end());
    ASSERT_TRUE(v.empty());
}

TEST(VectorSetting, Signal)
{
    TestSettings settings;
    VectorSetting<int> v { settings, "values" };

    QSignalSpy spy { &v.notifier(), SIGNAL(appended(const QVariant&)) };
    ASSERT_EQ(0, spy.size());

    v.push_back(1);

    ASSERT_EQ(1, spy.size());
    const auto &args  = spy.at(0);

    ASSERT_EQ(1, args.size());
    ASSERT_EQ(QMetaType::QVariant, args.at(0).type());

    const auto &value = args.at(0).value<QVariant>();
    ASSERT_TRUE(value.isValid());
    ASSERT_EQ(QVariant::Int, value.type());
    EXPECT_EQ(1, value.toInt());
}

struct CustomValue {
    int a;
    int b;

    CustomValue(const int a = 0, const int b = 0) : a { a }, b { b } {}
};

namespace vector_setting {

template<>
CustomValue
load(QSettings &s)
{
    const auto a = s.value("a").toInt();
    const auto b = s.value("b").toInt();
    return { a, b };
}

template<>
void
save(QSettings &s, const CustomValue &v)
{
    s.setValue("a", v.a);
    s.setValue("b", v.b);
}

} // namespace vector_setting

class CustomNotifier : public QObject
{
    Q_OBJECT
public:
    void emitAppended(const CustomValue &v) { Q_EMIT appended(v); }
    void emitRemoved(const CustomValue v, const size_t index) { Q_EMIT removed(v, index); }

Q_SIGNALS:
    void appended(const CustomValue &v);
    void removed(const CustomValue v, const size_t index);
};

TEST(VectorSetting, CustomType)
{
    TestSettings settings;
    VectorSetting<CustomValue, CustomNotifier> v { settings, "values" };

    v.emplace_back(1, 2);
    v.emplace_back(2, 3);
    v.emplace_back(3, 4);

    EXPECT_EQ(3, v.size());
}

#include "setting.moc"
