#ifndef connection_h
#define connection_h

#include "song.h"
#include "status.h"
#include <memory>
#include <mpd/client.h>
#include <utility>
#include <vector>

namespace mpd {

class Connection
{
public:
    Connection(mpd_connection * = nullptr);

    virtual explicit operator bool();
    virtual mpd_error getError();
    virtual const char *getErrorMessage();
    virtual const unsigned *getServerVersion();
    virtual bool clearError();
    virtual std::vector<std::unique_ptr<Song>> listQueueMeta();
    virtual int getFD();
    virtual bool sendIdle();
    virtual mpd_idle recvIdle(bool);
    virtual std::unique_ptr<Status> runStatus();
    virtual std::vector<std::pair<unsigned, unsigned>> runPlChangesPosId(unsigned);

    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;
    Connection(Connection &&);
    Connection &operator=(Connection &&);
    virtual ~Connection();

private:
    mpd_connection *m_connection;
};

} // namespace mpd

#endif