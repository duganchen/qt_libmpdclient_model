#include "mpdclient/song.h"
#include "queuemodel.h"
#include "gmock/gmock.h"
#include <QObject>
#include <QTest>

class MockSong : public mpd::Song
{
public:
    MOCK_METHOD(const char *, get_tag, (mpd_tag_type, unsigned), (override));
};

class TestQueueModel : public QObject
{
    Q_OBJECT
private slots:
    void aTest() { QCOMPARE(1, 1); }
};

QTEST_MAIN(TestQueueModel)
#include "tst_queueModelTest.moc"