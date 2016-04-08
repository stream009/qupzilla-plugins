#ifndef ADBLOCK_DASH_TEXT_FILE_HPP
#define ADBLOCK_DASH_TEXT_FILE_HPP

#include <ios>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock_dash {

class TextFile
{
    using Id = size_t;
    using Path = boost::filesystem::path;
    using StringRef = boost::iterator_range<const char*>;
public:
    TextFile(const Path&);

    // query
    const StringRef &line(const size_t lineNo) const;
    size_t lineCount() const { return m_lineNoToId.size(); }
    bool edited() const { return m_edited; }

    // modifier
    bool setLine(const size_t lineNo, std::string &&line);
    void newLine(const size_t lineNo);
    void deleteLine(const size_t lineNo);

    void save();

private:
    class IdBank
    {
    public:
        IdBank(const Id &start);

        Id withdraw();
        void deposit(const Id&);

    private:
        Id m_nextId;
        std::queue<Id> m_reusable;
    };

private:
    Path m_path;
    boost::iostreams::mapped_file_source m_file;

    std::vector<Id>                     m_lineNoToId;
    std::unordered_map<Id, StringRef>   m_idToLineRef;
    std::unordered_map<Id, std::string> m_idToLineStore;

    std::unique_ptr<IdBank> m_idBank;
    bool m_edited = false;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_TEXT_FILE_HPP
