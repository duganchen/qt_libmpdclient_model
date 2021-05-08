# Qt libmpdclient Model Demo

![screenshot](screenshot.png)

This is a technology test for a potential [Quetzalcoatl](https://github.com/duganchen/quetzalcoatl) refactor. The
advantage is the ability to unit-test the item models.

Quetzalcoatl makes heavy use of [libmpdclient](https://www.musicpd.org/libs/libmpdclient/). It's structured like
most C libraries, with a bunch of functions that take pointers to opaque struct. A connection to MPD is handled
by a [mpd_connection](https://www.musicpd.org/doc/libmpdclient/structmpd__connection.html) pointer.

One important properties of pointers is that they can be null. Quetzalcoatl considers a null mpd_connection pointer
to mean "not connected to MPD."

The program here, *queueview* supports connecting to MPD. When connected, it fetches the "queue" (playlist). Edit the
queue in another MPD client, and you'll see the queue here change instantly. Shut MPD down, and queueview will handle
losing the connection.

In both programs, the state of the queue is represented by a single QAbstractItemModel subclass that holds an
mpd_connection pointer. Obviously, if we want to unit test that model, we need to wrap the mpd_connection pointer in
a class. That would allow us to dependency-inject a mock object.

We'll use QTest (because we're using Qt) to set up test cases, CTest (because we're using CMake) to run them, and
Google Mock (which has good support for move-only types) for the mocks. The design of the wrapper classes was
designed with these (particularly Google Mock) in mind.

Okay...

We begin by implementing the basic operations. If we're wrapping something that has a copy API, then the wrapper
supports copies. If we're wrapping something that does not (such as mpd_connection, which represents a socket), then
the wrapper is move-only.

Each constructor takes an optional pointer of the typed to be wrapped. If the argument is not provided (the default
constructor is called), then the class instance wraps a null pointer.

To allow clients to check for nulllity, we reimplement the bool operator.

The wrapping of the other operations assumes that, as in C, the client is responsible for checking both the
precondition that the pointer is not null and the postcondition of a possible error.

Here's some libmpdclient example code:

```
struct mpd_connection *conn = mpd_connection_new(NULL, 0, 0);

// error handling
if (conn == NULL) {
    fprintf(stderr, "Out of memory\n");
    return;
}
if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
    fprintf(stderr, "%s\n", mpd_connection_get_error_message(c));
    mpd_connection_free(c);
    return;
}

// we can now use the connection
mpd_run_next(conn);

// close the connection and free memory
mpd_connection_free(conn);
```

The equivalent, and note the improved memory safety, would be:

```
mpd::Connection conn(mpd_connection_new(nullptr, 0, 0));

// error handling
if (!conn) {
    fprintf(stderr, "Out of memory\n");
    return;
}
if (conn.get_error() != MPD_ERROR_SUCCESS) {
    fprintf(stderr, "%s\n", conn.get_error_message());
    return;
}

// we can now use the connection
conn.run_next();
```

The equivalent to:

```
if (conn) {
    mpd_connection_free(conn);
}
conn = nullptr;
```

is:


```
mpd::Connection disconnected;
conn = std::move(disconnected);
```
Note that both set up the same (if !conn) checks.

Each class that needs to use this then takes a *reference*.

```
class QueueModel {
public:
    QueueModel(mpd::Connection &);
};

class DBModel {
public:
    DBModel(mpd::Connection &);
};

```

Due to the way C++ references work, this solves the architectural issue of how to notify multiple classes when a
connection is gained or lost. If multiple objects hold a reference to the same mpd::Connection, then an assignment
operation in one class will update it for all of them.

At this point, it's obvious how to set up a QueueModel unit test. We inject a mock mpd::Connection object, and we
manipulate its API, calling its *index* method when appropriate.