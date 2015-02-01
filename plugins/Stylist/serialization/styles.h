#ifndef STYLIST_SERIALIZATION_STYLES_H
#define STYLIST_SERIALIZATION_STYLES_H

#include "gui/../styles.h" //TODO hack fix this

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

template<typename Archive>
inline void
save_construct_data(Archive &ar, const stylist::Styles *styles,
                                 const unsigned int /*version*/)
{
    //qDebug() << "Styles" << __func__;
    ar << styles->m_directory
       << styles->m_styles;
}

template<typename Archive>
inline void
load_construct_data(Archive &ar, stylist::Styles *styles,
                                 const unsigned int /*version*/)
{
    //qDebug() << "Styles" << __func__;
    using stylist::Styles;

    Styles::Path path;           ar >> path;
    Styles::Container container; ar >> container;

    ::new(styles) Styles { std::move(path), std::move(container) };
}

}} // namespace boost::serialization

#endif // STYLIST_SERIALIZATION_STYLES_H
