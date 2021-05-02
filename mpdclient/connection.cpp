#include "connection.h"

mpd::Connection::Connection(mpd_connection *connection)
    : m_connection{connection}
{
}

mpd::Connection::operator bool()
{
    return bool(m_connection);
}

mpd_error mpd::Connection::getError()
{
    return mpd_connection_get_error(m_connection);
};

const char *mpd::Connection::getErrorMessage()
{
    return mpd_connection_get_error_message(m_connection);
}

const unsigned *mpd::Connection::getServerVersion()
{
    return mpd_connection_get_server_version(m_connection);
}

bool mpd::Connection::clearError()
{
    return mpd_connection_clear_error(m_connection);
}

std::vector<std::unique_ptr<mpd::Song>> mpd::Connection::listQueueMeta()
{
    // Precondition: m_connection is not null

    std::vector<std::unique_ptr<mpd::Song>> songs;
    if (mpd_send_list_queue_meta(m_connection)) {
        while (auto entity = mpd_recv_entity(m_connection)) {
            if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
                songs.push_back(
                    std::make_unique<mpd::Song>(mpd_song_dup(mpd_entity_get_song(entity))));
            }
        }
    }
    return songs;

    // Postcondition: getError() will have the success or failure status. The vector will be
    // empty if there was a problem.
}

int mpd::Connection::getFD() {
    return mpd_connection_get_fd(m_connection);
}

mpd::Connection::Connection(mpd::Connection &&other)
    : m_connection(other.m_connection)
{
    other.m_connection = nullptr;
}

mpd::Connection &mpd::Connection::operator=(mpd::Connection &&other)
{
    if (this != &other) {
        if (m_connection) {
            mpd_connection_free(m_connection);
        }

        m_connection = other.m_connection;
        other.m_connection = nullptr;
    }
    return *this;
}

mpd::Connection::~Connection()
{
    if (m_connection) {
        mpd_connection_free(m_connection);
    }
}

