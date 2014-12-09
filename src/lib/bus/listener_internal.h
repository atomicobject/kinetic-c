#ifndef LISTENER_INTERNAL_H
#define LISTENER_INTERNAL_H

typedef enum {
    MSG_NONE,
    MSG_ADD_SOCKET,
    MSG_CLOSE_SOCKET,
    MSG_EXPECT_RESPONSE,
    MSG_SHUTDOWN,
} MSG_TYPE;

/* Queue message. */
typedef struct listener_msg {
    const uint8_t id;
    MSG_TYPE type;
    struct listener_msg *next;
    union {
        struct {
            connection_info *info;
            int notify_fd;
        } add_socket;
        struct {
            int fd;
        } close_socket;
        struct {
            boxed_msg *box;
        } expect;
        struct {
            int unused;
        } shutdown;
    } u;
} listener_msg;

/* Record in table for partially processed messages. */
typedef struct rx_info_t {
    const uint8_t id;
    struct rx_info_t *next;
    bool active;
    time_t timeout_sec;
    rx_error_t error;
    boxed_msg *box;
} rx_info_t;

/* Max number of sockets to monitor. */
#define MAX_FDS 1000

/* Max number of partially processed messages.
 * TODO: We may want this significantly higher. */
//#define MAX_PENDING_MESSAGES (16*32)
#define MAX_PENDING_MESSAGES (1024)

/* Max number of unprocessed queue messages */
#define MAX_QUEUE_MESSAGES 32
typedef uint32_t msg_flag_t;

/* Receiver of responses */
typedef struct listener {
    struct bus *bus;
    struct casq *q;
    bool shutdown;

    rx_info_t rx_info[MAX_PENDING_MESSAGES];
    int info_available;
    rx_info_t *rx_info_freelist;
    //rx_info_t *rx_info_usedlist;
    uint16_t rx_info_in_use;

    listener_msg msgs[MAX_QUEUE_MESSAGES];
    listener_msg *msg_freelist;
    int16_t msgs_in_use;

    size_t upstream_backpressure;

    uint8_t tracked_fds;
    struct pollfd fds[MAX_FDS];
    connection_info *fd_info[MAX_FDS];
} listener;

static void tick_handler(listener *l);

static rx_info_t *get_free_rx_info(listener *l);
static void release_rx_info(struct listener *l, rx_info_t *info);
static listener_msg *get_free_msg(listener *l);
static bool push_message(struct listener *l, listener_msg *msg);
static void release_msg(struct listener *l, listener_msg *msg);
static void attempt_recv(listener *l, int avaliable);
static void process_unpacked_message(listener *l,
    connection_info *ci, bus_unpack_cb_res_t result);
static void notify_message_failure(listener *l,
    rx_info_t *info, bus_send_status_t status);
static void clean_up_completed_info(listener *l, rx_info_t *info);
static void observe_backpressure(listener *l, size_t backpressure);

static void msg_handler(listener *l, listener_msg *msg);
static void add_socket(listener *l, connection_info *ci, int notify_fd);
static void forget_socket(listener *l, int fd);
static void expect_response(listener *l, boxed_msg *box);
static void shutdown(listener *l);
static void free_ci(connection_info *ci);

#endif
