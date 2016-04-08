#ifndef ADBLOCK_DASH_SETTINGS_H
#define ADBLOCK_DASH_SETTINGS_H

#include "common/color_setting.hpp"
#include "common/setting.hpp"
#include "common/vector_setting.hpp"
#include "filter_set.hpp"
#include "subscription.hpp"

#include <cassert>
#include <memory>

#include <QtCore/QSettings>

namespace adblock_dash {

class Settings
{
public:
    using FilterSets = VectorSetting<FilterSet, FilterSetsNotifier>;
    using Subscriptions = VectorSetting<Subscription, SubscriptionNotifier>;

public:
    explicit Settings(const QString &settingsPath);
    ~Settings();

    StringSetting &directory()
    {
        return *m_directory;
    }

    const StringSetting &directory() const
    {
        return *m_directory;
    }

    Subscriptions &subscriptions()
    {
        return *m_subscriptions;
    }

    FilterSets &customFilterSets()
    {
        return *m_customLists;
    }

    const StringSetting &acceptableAdsUrl() const
    {
        return *m_acceptableAdsUrl;
    }

    const StringSetting &newFilterSetName() const
    {
        return *m_newFilterSetName;
    }

    const ColorSetting &commentColor() const
    {
        return *m_commentColor;
    }

    const ColorSetting &elementHidingRuleColor() const
    {
        return *m_elementHidingRuleColor;
    }

    const ColorSetting &exceptionRuleColor() const
    {
        return *m_exceptionRuleColor;
    }

    const StringSetting &knownSubscriptionsUrl() const
    {
        return *m_knownSubscriptionsUrl;
    }

    Subscription &exceptionRules()
    {
        return *m_exceptionRules;
    }

    const Subscription &exceptionRules() const
    {
        return *m_exceptionRules;
    }

private:
    void loadExceptionRules();
    void saveExceptionRules();

    void validate() const
    {
        assert(m_directory);
        assert(m_subscriptions);
        assert(m_customLists);
        assert(m_acceptableAdsUrl);
        assert(m_newFilterSetName);
        assert(m_commentColor);
        assert(m_elementHidingRuleColor);
        assert(m_exceptionRuleColor);
        assert(m_knownSubscriptionsUrl);
        assert(m_exceptionRules);
    }

private:
    QSettings m_settings;
    std::unique_ptr<StringSetting> m_directory;
    std::unique_ptr<Subscriptions> m_subscriptions;
    std::unique_ptr<FilterSets> m_customLists;
    std::unique_ptr<StringSetting> m_acceptableAdsUrl;
    std::unique_ptr<StringSetting> m_newFilterSetName;
    std::unique_ptr<ColorSetting> m_commentColor;
    std::unique_ptr<ColorSetting> m_elementHidingRuleColor;
    std::unique_ptr<ColorSetting> m_exceptionRuleColor;
    std::unique_ptr<StringSetting> m_knownSubscriptionsUrl;
    std::unique_ptr<Subscription> m_exceptionRules;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_SETTINGS_H
