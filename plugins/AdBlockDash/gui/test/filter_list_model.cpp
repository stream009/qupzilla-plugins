#include "../filter_list_model.hpp"

#include "common/path.hpp"
#include "common/test.hpp"
#include "core/settings.hpp"
#include "core/text_file.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <gtest/gtest.h>

namespace adblock_dash { namespace setting_dialog { namespace test {

namespace bfs = boost::filesystem;

class FilterListModelF : public ::testing::Test
{
public:
    FilterListModelF()
        : m_settings { "data" }
    {}

protected:
    void SetUp() override
    {
        bfs::remove(m_data_file_path);
        bfs::ofstream ofs { m_data_file_path };
        ofs << "[Adblock Plus 2.0]\n"
            << "first\n"
            << "second\n"
            << "third\n";
        ofs.close();

        assert(bfs::exists(m_data_file_path));
    }

protected:
    bfs::path m_data_file_path { "data/sample.txt" };
    Settings m_settings;
    TextFile m_file { m_data_file_path };
};

TEST_F(FilterListModelF, rowCount)
{
    FilterListModel model { m_settings, m_file };

    ASSERT_EQ(3, model.rowCount());
}

TEST_F(FilterListModelF, columnCount)
{
    FilterListModel model { m_settings, m_file };

    ASSERT_EQ(2, model.columnCount());
}

TEST_F(FilterListModelF, index)
{
    FilterListModel model { m_settings, m_file };

    EXPECT_TRUE( model.index(0, 0).isValid());
    EXPECT_TRUE( model.index(2, 0).isValid());
    EXPECT_FALSE(model.index(3, 0).isValid());
    EXPECT_TRUE( model.index(0, 1).isValid());
    EXPECT_FALSE(model.index(0, 2).isValid());
    EXPECT_TRUE( model.index(2, 1).isValid());
    EXPECT_FALSE(model.index(2, 2).isValid());
}

TEST_F(FilterListModelF, parent)
{
    FilterListModel model { m_settings, m_file };

    const auto &index = model.index(0, 0);
    ASSERT_TRUE(index.isValid());
    EXPECT_FALSE(model.parent(index).isValid());
    EXPECT_FALSE(model.parent({}).isValid());
}

TEST_F(FilterListModelF, flags)
{
    FilterListModel model { m_settings, m_file };
    const auto index = model.index(0, 0);

    EXPECT_EQ(
        Qt::ItemIsSelectable |
        Qt::ItemIsUserCheckable |
        Qt::ItemIsEnabled,
        model.flags(index));
}

TEST_F(FilterListModelF, data_normal)
{
    FilterListModel model { m_settings, m_file };

    auto value = model.data(model.index(0, 0), Qt::CheckStateRole);
    ASSERT_TRUE(value.isValid());
    ASSERT_EQ(QVariant::Int, value.type());
    EXPECT_EQ(2, value.toInt());

    value = model.data(model.index(0, 1), Qt::DisplayRole);
    ASSERT_TRUE(value.isValid());
    ASSERT_EQ(QVariant::String, value.type());
    EXPECT_EQ("first", value.toString());

    value = model.data(model.index(0, 0), Qt::DisplayRole);
    EXPECT_FALSE(value.isValid());
    value = model.data(model.index(0, 1), Qt::CheckStateRole);
    EXPECT_FALSE(value.isValid());
}

TEST_F(FilterListModelF, data_wrongRole)
{
    FilterListModel model { m_settings, m_file };

    auto value = model.data(model.index(0, 0), Qt::DisplayRole);
    EXPECT_FALSE(value.isValid());
    value = model.data(model.index(0, 1), Qt::CheckStateRole);
    EXPECT_FALSE(value.isValid());
}

TEST_F(FilterListModelF, headerData)
{
    FilterListModel model { m_settings, m_file };

    EXPECT_EQ("Enabled",
            model.headerData(0, Qt::Horizontal, Qt::DisplayRole));
    EXPECT_EQ("Filter",
            model.headerData(1, Qt::Horizontal, Qt::DisplayRole));
}

TEST_F(FilterListModelF, setData)
{
    FilterListModel model { m_settings, m_file };

    EXPECT_FALSE(model.setData(model.index(0, 0), "foo", Qt::EditRole));
    EXPECT_FALSE(
        model.setData(model.index(0, 1), Qt::Unchecked, Qt::CheckStateRole));

    const auto &index = model.index(0, 0);
    EXPECT_EQ(Qt::Checked, model.data(index, Qt::CheckStateRole));
    EXPECT_TRUE(model.setData(index, Qt::Unchecked, Qt::CheckStateRole));
    EXPECT_EQ(Qt::Unchecked, model.data(index, Qt::CheckStateRole));
}

}}} // namespace adblock_dash::setting_dialog::test
