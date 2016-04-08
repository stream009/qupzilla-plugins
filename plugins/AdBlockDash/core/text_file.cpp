#include "text_file.hpp"

#include "common/utility.hpp"

#include <algorithm>
#include <climits>
#include <cstdio>
#include <fstream>
#include <iterator>

#include <boost/algorithm/string.hpp>
#include <boost/make_unique.hpp>

#include <QtCore/QDebug> //TODO

namespace adblock_dash {

TextFile::
TextFile(const Path &path)
    : m_path { path }
{
    //TODO check whether file is too big to handle
    m_file.open(path);
    assert(m_file.is_open());

    auto &&data = boost::make_iterator_range(
                        m_file.data(), m_file.data() + m_file.size());
    if (data.back() == '\n') {
        data.drop_back();
    }

    const auto &lineCount =
                ::toSizeT(std::count(data.begin(), data.end(), '\n')) + 1;

    m_lineNoToId.reserve(lineCount);
    m_idToLineRef.reserve(lineCount);

    namespace ba = boost::algorithm;

    auto &&splitIt = ba::make_split_iterator(
                            data, ba::first_finder("\n", ba::is_equal()));

    for (auto id = 0u; !splitIt.eof(); ++splitIt, ++id) {
        m_lineNoToId.emplace_back(id);
        m_idToLineRef.emplace(id, *splitIt);
    }

    m_idBank = boost::make_unique<IdBank>(m_lineNoToId.back() + 1);

    assert(m_lineNoToId.size() == lineCount);
    assert(m_idToLineRef.size() == lineCount);
}

const TextFile::StringRef &TextFile::
line(const size_t lineNo) const
{
    assert(lineNo < m_lineNoToId.size());

    const auto &id = m_lineNoToId[lineNo];

    const auto &it = m_idToLineRef.find(id);
    assert(it != m_idToLineRef.end());
    const auto &result = it->second;

    return result;
}

bool TextFile::
setLine(const size_t lineNo, std::string &&line) //TODO exception safty
{
    assert(lineNo < m_lineNoToId.size());

    namespace ba = boost::algorithm;
    if (ba::equals(line, this->line(lineNo))) return false;

    const auto &id = m_idBank->withdraw();
    const auto oldId = m_lineNoToId[lineNo];
    m_lineNoToId[lineNo] = id;

    StringRef lineRef { line.data(), line.data() + line.size() };
    {
        const auto &rc = m_idToLineRef.emplace(id, lineRef);
        assert(rc.second);
    }
    {
        const auto &rc = m_idToLineStore.emplace(
                                std::move(id), std::move(line));
        assert(rc.second);
        assert(rc.first->second.data() == lineRef.begin());
        assert(rc.first->second.size() == lineRef.size());
    }

    m_idToLineRef.erase(oldId);
    m_idToLineStore.erase(oldId);
    m_idBank->deposit(oldId);

    m_edited = true;

    return true;
}

void TextFile::
newLine(const size_t lineNo)
{
    assert(lineNo <= m_lineNoToId.size());

    const auto &id = m_idBank->withdraw();
    const auto &it = m_lineNoToId.begin() + ::toSignedInt(lineNo);
    m_lineNoToId.emplace(it, id);

    std::string empty;
    StringRef lineRef { empty.data(), empty.data() + empty.size() };
    {
        const auto &rc = m_idToLineRef.emplace(id, lineRef);
        assert(rc.second);
    }
    {
        const auto &rc = m_idToLineStore.emplace(
                                std::move(id), std::move(empty));
        assert(rc.second);
        assert(rc.first->second.data() == lineRef.begin());
        assert(rc.first->second.size() == lineRef.size());
    }

    m_edited = true;
}

void TextFile::
deleteLine(const size_t lineNo)
{
    assert(lineNo < m_lineNoToId.size());

    const auto id = m_lineNoToId[lineNo];
    const auto &it = m_lineNoToId.begin() + ::toSignedInt(lineNo);
    assert(it != m_lineNoToId.end());
    m_lineNoToId.erase(it);

    m_idToLineRef.erase(id);
    m_idToLineStore.erase(id);
    m_idBank->deposit(id);

    m_edited = true;
}

void TextFile::
save()
{
    namespace bfs = boost::filesystem;

    const auto &tmp = bfs::unique_path();
    assert(!bfs::exists(tmp)); //TODO properly

    std::ofstream ofs { tmp.c_str() };

    for (const auto &id: m_lineNoToId) {
        const auto &it = m_idToLineRef.find(id);
        assert(it != m_idToLineRef.end());
        const auto &lineRef = it->second;

        ofs << lineRef << '\n';
    }
    ofs.close();

    bfs::rename(tmp, m_path);
}


TextFile::IdBank::
IdBank(const Id &start) : m_nextId { start } {}

TextFile::Id TextFile::IdBank::
withdraw()
{
    if (m_reusable.empty()) {
        assert(m_nextId < SIZE_MAX);
        return m_nextId++;
    }
    else {
        const auto id = m_reusable.front();
        m_reusable.pop();
        return id;
    }
}

void TextFile::IdBank::
deposit(const Id &id)
{
    m_reusable.push(id);
}

} // namespace adblock_dash
