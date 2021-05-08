#include "mpdclient/connection.h"
#include "mpdclient/song.h"
#include "queuemodel.h"
#include "gmock/gmock.h"
#include <mpd/client.h>
#include <vector>
#include <QObject>
#include <QTest>

using ::testing::ByMove;
using ::testing::Return;

class MockSong : public mpd::Song
{
public:
    MOCK_METHOD(const char *, get_tag, (mpd_tag_type, unsigned), (override));
};

class MockConnection : public mpd::Connection
{
public:
    MOCK_METHOD(std::vector<std::unique_ptr<mpd::Song>>, list_queue_meta, (), (override));
    MOCK_METHOD(mpd_error, get_error, (), (override));
    explicit operator bool() override { return true; };
};

class TestQueueModel : public QObject
{
    Q_OBJECT
private slots:
    void queueModelTest()
    {
        std::cout << "aaaaa\n";
        std::unique_ptr<MockSong> song(new MockSong());
        EXPECT_CALL(*song, get_tag(MPD_TAG_TITLE, 0)).WillOnce(Return("Song title"));
        std::vector<std::unique_ptr<mpd::Song>> songs;
        songs.push_back(std::move(song));

        MockConnection connection;
        EXPECT_CALL(connection, get_error()).WillOnce(Return(MPD_ERROR_SUCCESS));
        EXPECT_CALL(connection, list_queue_meta()).WillOnce(Return(ByMove(std::move(songs))));

        QueueModel model(connection);
        model.refresh();
        QCOMPARE(model.data(model.index(0, 0)), QString("Song title"));
    }
};

QTEST_MAIN(TestQueueModel)
#include "tst_queueModelTest.moc"