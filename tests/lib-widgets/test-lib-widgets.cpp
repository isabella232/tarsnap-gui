#include "warnings-disable.h"

WARNINGS_DISABLE
#include <QCoreApplication>
#include <QDateTime>
#include <QLineEdit>
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QRect>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QVariant>
#include <QWidget>
#include <Qt>
WARNINGS_ENABLE

#include "../qtest-platform.h"

#include "LogEntry.h"
#include "TBusyLabel.h"
#include "TElidedLabel.h"
#include "TOkLabel.h"
#include "TPathComboBrowse.h"
#include "TPathLineBrowse.h"
#include "TTabWidget.h"
#include "TTextView.h"
#include "TWizard.h"
#include "TWizardPage.h"

class TestLibWidgets : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void elidedLabel();
    void elidedLabel_status();
    void okLabel();
    void busylabel();
    void busylabel_on_off();
    void pathlinebrowse();
    void pathcombobrowse();
    void ttabwidget_basic();
    void ttabwidget_colours();
    void ttabwidget_logo();
    void twizard();
    void textview();

private:
    void helper_tabwidget(TTabWidget *tabwidget);
};

void TestLibWidgets::initTestCase()
{
    QCoreApplication::setOrganizationName(TEST_NAME);

    // Use a custom message handler to filter out unwanted messages
    IF_NOT_VISUAL { qInstallMessageHandler(offscreenMessageOutput); }
}

void TestLibWidgets::cleanupTestCase()
{
    // Wait up to 5 seconds to delete objects scheduled with ->deleteLater()
    WAIT_FINAL;
}

void TestLibWidgets::elidedLabel()
{
    // Initialize variables.
    const QString text("this is a long long long piece of text");
    TElidedLabel *elidedlabel = new TElidedLabel();

    VISUAL_INIT(elidedlabel);

    // Set up ElidedText.
    elidedlabel->setElide(Qt::ElideRight);
    elidedlabel->setText(text);

    // The text should be fully visible if there's enough room.
    elidedlabel->setGeometry(100, 100, 1000, 20);
    QVERIFY(elidedlabel->text() == text);
    QVERIFY(elidedlabel->elideText(text).count() == text.count());
    VISUAL_WAIT;

    // The text should be elided if the widget is too narrow.
    elidedlabel->setGeometry(100, 100, 100, 20);
    QVERIFY(elidedlabel->text() == text);
    QVERIFY(elidedlabel->elideText(text).count() < text.count());
    VISUAL_WAIT;

    delete elidedlabel;
}

void TestLibWidgets::elidedLabel_status()
{
    TElidedLabel *el = new TElidedLabel();
    el->setMinimumWidth(200);

    VISUAL_INIT(el);

    el->messageNormal("normal message");
    VISUAL_WAIT;

    el->messageError("error message");
    VISUAL_WAIT;

    delete el;
}

void TestLibWidgets::okLabel()
{
    TOkLabel *ok = new TOkLabel();

    VISUAL_INIT(ok);

    ok->setStatus(TOkLabel::Ok);
    VISUAL_WAIT;

    ok->setStatus(TOkLabel::Error);
    VISUAL_WAIT;

    ok->setStatus(TOkLabel::Unset);
    VISUAL_WAIT;

    delete ok;
}

void TestLibWidgets::busylabel()
{
    TBusyLabel *bw = new TBusyLabel();

    VISUAL_INIT(bw);

    delete bw;
}

void TestLibWidgets::busylabel_on_off()
{
    TBusyLabel *bw = new TBusyLabel();

    VISUAL_INIT(bw);

    // Before starting
    VISUAL_WAIT;

    // Animation
    bw->animate(true);
    QTest::qWait(100);
    VISUAL_WAIT;

    // After animation
    bw->animate(false);
    QTest::qWait(100);
    VISUAL_WAIT;

    delete bw;
}

void TestLibWidgets::pathlinebrowse()
{
    TPathLineBrowse *plb = new TPathLineBrowse();
    QSignalSpy       sig_changed(plb, SIGNAL(textChanged(QString)));

    VISUAL_INIT(plb);

    plb->setLabel("label");
    QVERIFY(plb->label() == "label");
    VISUAL_WAIT;

    plb->setPlaceholderText("placeholder");
    QVERIFY(plb->placeholderText() == "placeholder");
    VISUAL_WAIT;

    plb->setDialogTitle("text");
    QVERIFY(plb->dialogTitle() == "text");
    VISUAL_WAIT;

    QVERIFY(sig_changed.count() == 0);
    plb->setText("text");
    QVERIFY(plb->text() == "text");
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "text");
    sig_changed.clear();
    VISUAL_WAIT;

    plb->setStatusOk("");
    VISUAL_WAIT;

    plb->setStatusError("error message");
    VISUAL_WAIT;

    plb->setStatusOk("ok message");
    VISUAL_WAIT;

    plb->clear();
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "");
    VISUAL_WAIT;

    // Test setText as a slot
    QLineEdit *le = new QLineEdit();
    connect(le, &QLineEdit::textChanged, plb, &TPathLineBrowse::setText);

    plb->clear();
    le->setText("indirect text");
    QVERIFY(plb->text() == "indirect text");
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "indirect text");
    VISUAL_WAIT;

    delete le;
    delete plb;
}

void TestLibWidgets::pathcombobrowse()
{
    TPathComboBrowse *pcb = new TPathComboBrowse();
    QSignalSpy        sig_changed(pcb, SIGNAL(textChanged(QString)));

    VISUAL_INIT(pcb);

    pcb->setLabel("label");
    QVERIFY(pcb->label() == "label");
    VISUAL_WAIT;

    pcb->setPlaceholderText("placeholder");
    QVERIFY(pcb->placeholderText() == "placeholder");
    VISUAL_WAIT;

    pcb->setDialogTitle("text");
    QVERIFY(pcb->dialogTitle() == "text");
    VISUAL_WAIT;

    pcb->setDialogFilter("Text files (*.txt *.text)");
    QVERIFY(pcb->dialogFilter() == "Text files (*.txt *.text)");
    VISUAL_WAIT;

    QVERIFY(sig_changed.count() == 0);
    pcb->setText("text");
    QVERIFY(pcb->text() == "text");
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "text");
    sig_changed.clear();
    VISUAL_WAIT;

    QVERIFY(pcb->count() == 1);
    pcb->addItem("another");
    QVERIFY(sig_changed.count() == 0);
    QVERIFY(pcb->count() == 2);
    VISUAL_WAIT;

    // Go back to the previous text
    pcb->setText("another");
    QVERIFY(sig_changed.count() == 1);
    pcb->setText("text");
    QVERIFY(sig_changed.count() == 2);
    VISUAL_WAIT;

    sig_changed.clear();
    pcb->setText("third");
    QVERIFY(pcb->text() == "third");
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "third");
    VISUAL_WAIT;

    pcb->setStatusOk("");
    VISUAL_WAIT;

    pcb->setStatusError("error message");
    VISUAL_WAIT;

    pcb->setStatusOk("ok message");
    VISUAL_WAIT;

    sig_changed.clear();
    pcb->clear();
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "");
    VISUAL_WAIT;

    // Test setText as a slot
    QLineEdit *le = new QLineEdit();
    connect(le, &QLineEdit::textChanged, pcb, &TPathComboBrowse::setText);

    sig_changed.clear();
    pcb->clear();
    le->setText("indirect text");
    QVERIFY(pcb->text() == "indirect text");
    QVERIFY(sig_changed.count() == 1);
    QVERIFY(sig_changed.takeFirst().at(0).value<QString>() == "indirect text");
    VISUAL_WAIT;

    delete le;
    delete pcb;
}

void TestLibWidgets::helper_tabwidget(TTabWidget *tabwidget)
{
    QRect g;
    tabwidget->addTab(new QWidget(), "one-q");
    VISUAL_WAIT;
    tabwidget->addTab(new QWidget(), "TWO-p");
    VISUAL_WAIT;

    // Make it narrow
    g = tabwidget->geometry();
    tabwidget->setGeometry(g.x(), g.y(), 200, g.height());
    VISUAL_WAIT;

    // Increase the width of tabs
    tabwidget->setStyleSheet(tabwidget->styleSheet()
                             + "QTabBar::tab { min-width: 80px; }");
    VISUAL_WAIT;

    // Select a different tab
    tabwidget->setCurrentIndex(1);
    VISUAL_WAIT;

    // Make it wider
    g = tabwidget->geometry();
    tabwidget->setGeometry(g.x(), g.y(), 360, g.height());
    VISUAL_WAIT;

    // Select a different tab
    tabwidget->setCurrentIndex(0);
    VISUAL_WAIT;

    // Remove the two tabs
    tabwidget->removeTab(0);
    VISUAL_WAIT;
    tabwidget->removeTab(0);
    VISUAL_WAIT;
}

void TestLibWidgets::ttabwidget_basic()
{
    IF_MACOS_PRE_5_15_SKIP;

    TTabWidget *tabwidget = new TTabWidget();
    VISUAL_INIT(tabwidget);

    helper_tabwidget(tabwidget);
    VISUAL_WAIT;

    delete tabwidget;
}

void TestLibWidgets::ttabwidget_colours()
{
    IF_MACOS_PRE_5_15_SKIP;

    TTabWidget *tabwidget = new TTabWidget();
    VISUAL_INIT(tabwidget);

    // Notes for the effect of each setting are in order.  This is based
    // on reading the Qt stylesheet reference, the Qt stylesheet examples for
    // QTabWidget, and experimenting.
    tabwidget->setStyleSheet(
        // Overall widget.  This will also apply a dark pink colour to the
        // unused space of the QTabBar (to the right of the tabs).
        "QTabWidget { background-color: pink; }"
        // The frame around the QTabWidget pages (only a small border).
        // Setting this will add a black border between the active tab and
        // the pane (?).
        "QTabWidget::pane { background-color: green; }"
        // Default colour for tabs area.  Setting this will make all tabs
        // appear to have the same height, rather than the selected tab
        // appearing to be higher than the unselected tabs (?).
        "QTabBar::tab { background-color: yellow; }"
        // The selected tab.
        "QTabBar::tab::selected { background-color: lightblue; }");

    helper_tabwidget(tabwidget);

    delete tabwidget;
}

void TestLibWidgets::ttabwidget_logo()
{
    IF_MACOS_PRE_5_15_SKIP;

    TTabWidget *tabwidget = new TTabWidget();

    // Set up the logos and show it.
    tabwidget->setLargeLogoUrl(QUrl("qrc:/logos/tarsnap-header-h29.png"));
    tabwidget->setSmallLogoUrl(QUrl("qrc:/logos/tarsnap-icon-h29.png"));
    VISUAL_INIT(tabwidget);

    // Increase the height
    tabwidget->setStyleSheet("QTabBar::tab { height: 32px; }");
    VISUAL_WAIT;

    helper_tabwidget(tabwidget);

    delete tabwidget;
}

void TestLibWidgets::twizard()
{
    TWizard *wizard = new TWizard();

    // Add logo
    QPixmap pix(32, 32);
    pix.fill(Qt::blue);
    wizard->setLogo(pix);

    // Create pages with titles
    TWizardPage *p1 = new TWizardPage();
    TWizardPage *p2 = new TWizardPage();
    p1->setTitle("page 1");
    p2->setTitle("page 2");

    // Add a "proceed" button to each page
    QPushButton *pb1 = new QPushButton(p1);
    pb1->setText("Next");
    pb1->setObjectName("nextButton");
    QPushButton *pb2 = new QPushButton(p2);
    pb2->setText("Finish");
    pb2->setObjectName("finishButton");

    // Add pages to the wizard; must be after the buttons have been added
    wizard->addPages(QList<TWizardPage *>() << p1 << p2);

    // Navigate through the pages.
    VISUAL_INIT(wizard);

    wizard->currentPage()->button(TWizardPage::NextButton)->click();
    VISUAL_WAIT;
    wizard->currentPage()->button(TWizardPage::FinishButton)->click();
    VISUAL_WAIT;

    // We should have quit the wizard
    QVERIFY(wizard->isVisible() == false);

    delete wizard;
}

void TestLibWidgets::textview()
{
    TTextView *tv = new TTextView();

    VISUAL_INIT(tv);

    tv->appendLog(LogEntry{QDateTime::currentDateTime(), "one"});
    tv->appendLog(LogEntry{QDateTime::currentDateTime(), "two"});
    tv->appendLog(LogEntry{QDateTime::currentDateTime(), "three"});
    tv->appendLog(LogEntry{QDateTime::currentDateTime(), "four"});
    VISUAL_WAIT;

    delete tv;
}

QTEST_MAIN(TestLibWidgets)
WARNINGS_DISABLE
#include "test-lib-widgets.moc"
WARNINGS_ENABLE
