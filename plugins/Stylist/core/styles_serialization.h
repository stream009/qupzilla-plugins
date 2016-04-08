#ifndef STYLIST_SERIALIZATION_STYLES_H
#define STYLIST_SERIALIZATION_STYLES_H

#include "core/styles.h"

/// For stylist::Style
namespace boost { namespace serialization {

template<typename Archive>
inline void
serialize(Archive &, stylist::Style&, unsigned int /*version*/)
{}

template<typename Archive>
inline void
save_construct_data(Archive &ar, const stylist::Style *style,
                                 const unsigned int /*version*/)
{
    //qDebug() << "Style" << __func__;
    const stylist::Styles* styles = &style->m_parent;
    ar << styles
       << style->m_name
       << style->m_path
       << style->m_enabled;
}

template<typename Archive>
inline void
load_construct_data(Archive &ar, stylist::Style *style,
                                 const unsigned int /*version*/)
{
    //qDebug() << "Style" << __func__;
    using namespace stylist;

    Styles *parent;     ar >> parent;
    std::string name;   ar >> name;
    Style::Path path;   ar >> path;
    bool enabled;       ar >> enabled;

    ::new(style) Style { name, path, enabled, *parent };
}

}} // namespace boost::serialization


/// stylist::Styles
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace boost { namespace serialization {

template<typename Archive>
inline void
serialize(Archive &, stylist::Styles&, unsigned int /*version*/)
{}

#ifndef __GNUC__
template<typename Archive>
inline void
save_construct_data(Archive &ar, const stylist::Styles *styles,
                                 const unsigned int /*version*/)
{
    //qDebug() << "Styles" << __func__;
    const auto &container = styles->m_styles;

    // ar << container.size();  //TODO why the f--- does this fail?
    const auto count = container.size();
    ar << count;

    for (const auto &item: container) {
        //ar << &item; //TODO ditto
        const stylist::Styles::Container::value_type *ptr = &item;
        ar << ptr;
    }
}

template<typename Archive>
inline void
load_construct_data(Archive &ar, stylist::Styles *styles,
                                 const unsigned int /*version*/)
{
    //qDebug() << "Styles" << __func__;
    using stylist::Styles;

    Styles::Container container; //ar >> container;
    size_t count; ar >> count;
    for (auto i = 0u; i < count; ++i) {
        Styles::Container::value_type *style;
        try {
            ar >> style;
        }
        catch (const std::exception &) { //TODO more specific exception
            continue;
        }
        container.push_back(std::move(*style));
    }

    ::new(styles) Styles { std::move(container) };
}
#endif // __GNUC__
}} // namespace boost::serialization

#endif // STYLIST_SERIALIZATION_STYLES_H
