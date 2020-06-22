/*
** EPITECH PROJECT, 2020
** server
** File description:
** teams gestion
*/

#include "server.h"

bool user_subscribed(server_t *server, int i, int id, char *state)
{
    for (int k = 0; strcmp(server->teams[i].members[k].name, "NULL"); k++)
        if (!strcmp(server->teams[i].members[k].name,
            server->clients[id].user_name))
            return (true);
    dprintf(server->clients[id].fd_client,
        "533 You can't create a %s if you aren't in the team\r\n", state);
    delay(1);
    dprintf(server->clients[id].fd_client, "128|\r\n");
    return (false);
}

bool channel_exist(server_t *server, int i, char *name, int id)
{
    int k = 0;

    for (; (strcmp(server->teams[i].channel[k].channel_id, "NULL") != 0);
        k++)
        if ((strcmp(server->teams[i].channel[k].channel_name, name) == 0)) {
            dprintf(server->clients[id].fd_client,
                "511 This channel already exist\r\n");
            delay(1);
            dprintf(server->clients[id].fd_client, "129|\r\n");
            return (false);
        }
    return (true);
}

void init_new_channel(server_t *server, int i, int id, int k)
{
    init_first_thread(server, i, k);
    init_next_channel(server, i, k + 1);
    dprintf(server->clients[id].fd_client,
        "123|%s|%s|%s|\r\n",
        server->teams[i].channel[k].channel_id,
        server->teams[i].channel[k].channel_name,
        server->teams[i].channel[k].channel_desc);
}

void send_notif_channel(server_t *server, int i, int id, int k)
{
    int count = 0;

    delay(1);
    for (int a = 0; strcmp(server->teams[i].members[a].name, "NULL") != 0;
        a++) {
        for (count = 0; strcmp(server->clients[count].user_name,
            server->teams[i].members[a].name) != 0; count++);
        dprintf(server->clients[count].fd_client, "106|%s|%s|%s|\r\n",
            server->teams[i].channel[k].channel_id,
            server->teams[i].channel[k].channel_name,
            server->teams[i].channel[k].channel_desc);
    }
    server->teams[i].nb_channel++;
    server->teams[i].channel[k].nb_thread = 0;
    dprintf(server->clients[id].fd_client,
        "226 You succesfully created the channel \"%s\"\r\n",
        server->teams[i].channel[k].channel_name);
    server_event_channel_created(server->teams[i].team_id,
        server->teams[i].channel[k].channel_id,
        server->teams[i].channel[k].channel_name);
}

void create_new_channel(server_t *server, int id, char *name, char *desc)
{
    int i = 0;
    int k = 0;
    char *id_generate = generate_id();

    for (i = 0; strcmp(server->teams[i].team_id,
        server->clients[id].use_state[0]); i++);
    if (!channel_exist(server, i, name, id) ||
        !user_subscribed(server, i, id, "channel"))
        return;
    for (; strcmp(server->teams[i].channel[k].channel_id, "NULL"); k++);
    server->teams[i].channel = realloc(server->teams[i].channel,
        sizeof(channel_t) * (k + 2));
    memset(&server->teams[i].channel[k], 0, sizeof(channel_t));
    server->teams[i].channel[k].thread = malloc(sizeof(thread_t));
    strcpy(server->teams[i].channel[k].channel_name, name);
    strcpy(server->teams[i].channel[k].channel_id, id_generate);
    free(id_generate);
    strcpy(server->teams[i].channel[k].channel_desc, desc);
    init_new_channel(server, i, id, k);
    send_notif_channel(server, i, id, k);
}
