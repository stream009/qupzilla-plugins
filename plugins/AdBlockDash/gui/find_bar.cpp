#include "find_bar.hpp"

#include "filter_list_actions.hpp"

namespace adblock_dash { namespace setting_dialog {

FindBar::
FindBar(FilterListActions &actions, QWidget &parent)
    : Base { &parent }
    , m_text { new LineEdit { this } }
    , m_next { new QToolButton { this } }
    , m_prev { new QToolButton { this } }
    , m_matchCase { new QPushButton { this } }
    , m_message { new QLabel { this } }
    , m_close { new QToolButton { this } }
    , m_actions { actions }
{
    assert(m_text);
    assert(m_next);
    assert(m_prev);
    assert(m_matchCase);
    assert(m_message);
    assert(m_close);

    initWidgets();
    layoutWidgets();
    connectWidgets();
}

void FindBar::
focusInEvent(QFocusEvent* const)
{
    m_text->setFocus();
}

void FindBar::
search(const QString &keyword,
       const int step/*= +1*/, const int startOffset/*= 0*/) const
{
    const auto &caseSensitivity =
        m_matchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    const auto &result =
            m_actions.search(keyword, caseSensitivity, step, startOffset);
    if (result) {
        m_message->setText("");
        m_text->setStyleSheet("");
    }
    else {
        m_message->setText(tr("Not found"));
        m_text->setStyleSheet("background-color: #ff6666");
    }
}

void FindBar::
searchNext() const
{
    search(m_text->text(), +1, +1);
}

void FindBar::
searchPrevious() const
{
    search(m_text->text(), -1, -1);
}

void FindBar::
initWidgets()
{
    this->setGeometry(0, 0, 0, 20);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    m_text->setPlaceholderText(tr("Find in filters"));
    m_text->setMinimumSize(200, 0);

    m_prev->setArrowType(Qt::LeftArrow);
    m_prev->setToolTip(tr("Find previous occurence of the phrase"));

    m_next->setArrowType(Qt::RightArrow);
    m_next->setToolTip(tr("Find next occurence of the phrase"));

    m_matchCase->setText(tr("Mat&ch Case"));
    m_matchCase->setToolTip(tr("Search with case sensitivity"));

    m_message->setText("");

    m_close->setText("X");
    m_close->setArrowType(Qt::NoArrow);
    m_close->setToolTip(tr("Close find bar"));
}

void FindBar::
layoutWidgets()
{
    auto* const left = new QHBoxLayout;
    assert(left);
    left->setContentsMargins(0, 0, 0, 0);
    left->setSpacing(0);
    left->addWidget(m_text);
    left->addWidget(m_prev);
    left->addWidget(m_next);

    auto* const layout = new QHBoxLayout { this };
    assert(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addLayout(left, 0);
    layout->addWidget(m_matchCase, 0);
    layout->addWidget(m_message, 0);
    layout->addStretch(1);
    layout->addWidget(m_close, 0);

    assert(left->parent() == layout);
}

void FindBar::
connectWidgets()
{
    this->connect(m_prev, SIGNAL(clicked()),
                    this,   SLOT(searchPrevious()));

    this->connect(m_next, SIGNAL(clicked()),
                    this,   SLOT(searchNext()));

    this->connect(m_text, SIGNAL(returnPressed()),
                  m_next, SIGNAL(clicked()));
    this->connect(m_text, SIGNAL(textChanged(const QString&)),
                    this,   SLOT(search(const QString&)));

    this->connect(m_close, SIGNAL(clicked(bool)),
                     this,   SLOT(hide()));
}

}} // namespace adblock_dash::setting_dialog
