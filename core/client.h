// Copyright (c) 2015, Louis Opter <kalessin@kalessin.fr>
//
// This file is part of lighstd.
//
// lighstd is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// lighstd is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with lighstd.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

enum { LGTD_CLIENT_JSMN_TOKENS_NUM = 48 };
enum { LGTD_CLIENT_MAX_REQUEST_BUF_SIZE = 2048 };

struct lgtd_client {
    LIST_ENTRY(lgtd_client)  link;
    struct bufferevent      *io;
    char                    ip_addr[INET6_ADDRSTRLEN];
    uint16_t                port;
    jsmn_parser             jsmn_ctx;
    jsmntok_t               jsmn_tokens[LGTD_CLIENT_JSMN_TOKENS_NUM];
};
LIST_HEAD(lgtd_client_list, lgtd_client);

#define LGTD_CLIENT_WRITE_STRING(client, s) do {        \
    bufferevent_write((client)->io, s, strlen((s)));    \
} while(0)

struct lgtd_client *lgtd_client_open(evutil_socket_t, const struct sockaddr_storage *);
void lgtd_client_close_all(void);