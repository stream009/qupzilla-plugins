#include "styles.h"

namespace stylist {

Style::
Style(Style &&rhs) noexcept
    : m_parent { rhs.m_parent },
      m_name { std::move(rhs.m_name) },
      m_path { std::move(rhs.m_path) },
      m_enabled { rhs.m_enabled },
      m_styleSheet { std::move(rhs.m_styleSheet) }
{}

// We have to write this manually, since libstd++ doesn't give noexcept
// move assignment operator to std::string
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58265
Style &Style::
operator=(Style &&rhs) noexcept
{
    m_name = std::move(rhs.m_name);
    m_path = std::move(rhs.m_path);
    m_styleSheet = std::move(rhs.m_styleSheet);
    m_enabled = rhs.m_enabled;

    return *this;
}

void Style::
setEnabled(const bool enabled)
{
    m_enabled = enabled;
    Q_EMIT m_parent.changed();
}

} // namespace stylist

#include "styles_serialization.h"

#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <QtCore/QDebug>

namespace stylist {

using Path = boost::filesystem::path;

Path Styles::m_directory;

static Path
dataPath()
{
    static const auto &path = Styles::directory() / "styles.dat";
    return path;
}

Styles &Styles::
instance()
{
    static std::unique_ptr<Styles> instance = create();
    return *instance;
}

Styles::
~Styles() //TODO noexcept
{
    namespace bfs = boost::filesystem;

    bfs::ofstream ofs { dataPath() };
    assert(ofs.good()); //TODO better
    boost::archive::text_oarchive dat { ofs };

    Styles *ptr = this; //TODO why does this necessary?
    dat << ptr; //TODO handle exception
}

std::string Styles::
query(const Url &url) const
{
    struct Filter {
        bool operator()(const Container::value_type &value) const {
            return value.enabled();
        }
    };
    struct Extractor {
        std::string operator()(const Container::value_type &value) const {
            return value.styleFor(*m_url);
        }
        Extractor(const Url &url) : m_url { &url } {}
        const Url *m_url;
    };

    namespace ba = boost::algorithm;
    namespace bad = boost::adaptors;
    return ba::join(m_styles | bad::filtered(Filter {})
                             | bad::transformed(Extractor { url }),
                    "\n");
}

Styles::Container::reference Styles::
at(const Path &path)
{
    const auto it = std::find_if(m_styles.begin(), m_styles.end(),
        [&path] (const Style &style) {
            return style.path() == path;
        }
    );
    if (it == m_styles.end()) {
        throw "something"; //TODO better std::out_of_range?
    }
    return *it;
}

void Styles::
import(const Path &path)
{
    namespace bfs = boost::filesystem;

    assert(bfs::exists(path)); //TODO better
    assert(bfs::is_regular_file(path)); //TODO better

    const auto &destination = m_directory / path.filename();
    //TODO check if destination already exists

    bfs::copy_file(path, destination); //TODO exception

    // Directory watcher will take care rest of the process.
}

void Styles::
remove(const Path &path)
{
    namespace bfs = boost::filesystem;

    //qDebug() << __func__ << path;
    if (!bfs::exists(path)) {
        //TODO something
        return;
    }
    bfs::remove(path);

    // Directory watcher will take care rest of the process.
}

const Path &Styles::
directory()
{
    assert(!m_directory.empty());
    return m_directory;
}

void Styles::
setDirectory(const Path &path)
{
    namespace bfs = boost::filesystem;
    assert(bfs::exists(path));
    m_directory = path;
}

Styles::
Styles()
    : m_dirWatcher { m_directory }
{
    init();
}

std::unique_ptr<Styles> Styles::
create()
{
    namespace bfs = boost::filesystem;

    Styles *ptr;

    if (bfs::exists(dataPath())) {
        try {
            bfs::ifstream ifs { dataPath() };
            assert(ifs.good()); //TODO better
            boost::archive::text_iarchive dat { ifs };

            dat >> ptr; //TODO handle exception
        }
        catch (const boost::archive::archive_exception &) {
            qDebug() << "Stylist: data file" << dataPath() << "is corrupted";
            ptr = new Styles {};
        }
    }
    else {
        ptr = new Styles {};
    }

    assert(ptr);
    return std::unique_ptr<Styles>(ptr);
}

void Styles::
init()
{
    namespace bfs = boost::filesystem;

    assert(!m_directory.empty());
    bfs::create_directory(m_directory);

    scanDirectory();

    this->connect(&m_dirWatcher, SIGNAL(fileAdded(const Path&)),
                  this,          SLOT(addFile(const Path&)));
    this->connect(&m_dirWatcher, SIGNAL(fileDeleted(const Path&)),
                  this,          SLOT(deleteFile(const Path&)));
    this->connect(&m_dirWatcher, SIGNAL(fileModified(const Path&)),
                  this,          SLOT(slotFileModified(const Path&)));
}

void Styles::
scanDirectory()
{
    namespace bfs = boost::filesystem;

    struct Filter {
        bool operator()(const bfs::directory_entry &e) const {
            return e.path().extension() == ".css";
        }
    };

    const auto &range = boost::make_iterator_range(
        boost::make_filter_iterator(
            Filter {},
            bfs::directory_iterator { m_directory }
        ),
        {}
    );

    const auto &isAlreadyRegistered =
        [this] (const Path &path) {
            return std::any_of(m_styles.begin(), m_styles.end(),
                [&path] (const Container::value_type &s) {
                    return s.path() == path;
                }
            );
        };

    for (const auto &entry: range) {
        const auto &path = entry.path();
        if (!isAlreadyRegistered(path)) {
            addFile(path);
        }
    }

    //qDebug() << m_styles.size() << "styles are loaded";
}

void Styles::
addFile(const Path &path)
{
    if (path.extension() != ".css") return;

    const auto &name = path.filename();
    m_styles.emplace_back(name.c_str(), path, true, *this);

    Q_EMIT changed();
}

void Styles::
deleteFile(const Path &path)
{
    boost::range::remove_erase_if(m_styles,
        [&path] (const decltype(m_styles)::value_type &style) {
            return style.path() == path;
        }
    );
    Q_EMIT changed();
}

void Styles::
slotFileModified(const Path &)
{
    Q_EMIT changed();
}

} // namespace stylist
