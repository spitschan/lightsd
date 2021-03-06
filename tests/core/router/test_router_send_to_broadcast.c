#include "router.c"

#include "mock_daemon.h"
#include "mock_log.h"
#include "mock_timer.h"
#include "tests_utils.h"

#include "tests_router_utils.h"

int
main(void)
{
    lgtd_lifx_wire_setup();

    lgtd_tests_insert_mock_gateway(2);
    lgtd_tests_insert_mock_gateway(1);

    struct lgtd_lifx_packet_power_state payload = {
        .power = LGTD_LIFX_POWER_ON
    };
    struct lgtd_proto_target_list *targets;
    targets = lgtd_tests_build_target_list("*", NULL);
    lgtd_router_send(targets, LGTD_LIFX_SET_POWER_STATE, &payload);

    if (lgtd_tests_gw_pkt_queue_size != 2) {
        lgtd_errx(1, "2 packets should have been sent");
    }

    for (int i = lgtd_tests_gw_pkt_queue_size; i--;) {
        struct lgtd_lifx_gateway *gw = lgtd_tests_gw_pkt_queue[i].gw;
        if (gw->socket != i + 1) {
            lgtd_errx(
                1, "packet was sent to wrong gateway (expected %d, got %d)",
                i + 1, gw->socket
            );
        }
        const struct lgtd_lifx_packet_header *hdr;
        hdr = lgtd_tests_gw_pkt_queue[i].hdr;
        int expected_flags =
            LGTD_LIFX_ADDRESSABLE|LGTD_LIFX_TAGGED|LGTD_LIFX_RES_REQUIRED;
        if (!lgtd_tests_lifx_header_has_flags(hdr, expected_flags)) {
            lgtd_errx(1, "packet header doesn't have the right bits set");
        }
        if (hdr->target.tags != 0) {
            lgtd_errx(1, "tags should be 0 for broadcast");
        }
        if (memcmp(gw->site.as_array, hdr->site, sizeof(hdr->site))) {
            lgtd_errx(1, "sites don't match");
        }
        if (lgtd_tests_gw_pkt_queue[i].pkt != &payload) {
            lgtd_errx(1, "the payload has been improperly set");
        }
        if (lgtd_tests_gw_pkt_queue[i].pkt_size != sizeof(payload)) {
            lgtd_errx(
                1, "unexpected pkt size %d (expected %ju)",
                lgtd_tests_gw_pkt_queue[i].pkt_size, (uintmax_t)sizeof(payload)
            );
        }
    }

    return 0;
}
