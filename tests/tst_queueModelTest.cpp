#include "mpdclient/connection.h"
#include "queuemodel.h"
#include "gmock/gmock.h"
#include <QObject>
#include <QTest>

class TestQueueModel : public QObject
{
    Q_OBJECT
private slots:
    void aTest() { QCOMPARE(1, 1); }
};

QTEST_MAIN(TestQueueModel)
#include "tst_queueModelTest.moc"