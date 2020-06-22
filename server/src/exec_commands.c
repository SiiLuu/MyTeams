/*
** EPITECH PROJECT, 2020
** NWP_myteams_2019
** File description:
** exec_commands
*/

#include "server.h"

void help_client(server_t *server, int client, int id)
{
    (void)id;
    if (count_args(server, 0) == true)
        dprintf(client, "201 Functions aviable on the server\n\n\
            /login [“username”] : set the username used by client\n\
            /logout : disconnect the client from the server\n\
            /users : get the list of all users that exist on the domain\n\
            /user [“user_uuid”] : get information about a user\n\
            /send [“user_uuid”] [“message_body”] : send a message to a user\n\
            /messages [“user_uuid”]: list all messages exchange with an user\n\
            /subscribe[“team_uuid”]:subscribe to the event of a team and its\n\
            sub directories\n\
            /subscribed ?[“team_uuid”]:list all subscribed teams or list all\n\
            users subscribed to a team\n\
            /unsubscribe [“team_uuid”] : unsubscribe from a team\n\
            /use ?[“team_uuid”] ?[“channel_uuid”] ?[“thread_uuid”] : use\n\
            specify a context team/channel/thread\n\
            /create : based on what is being used create the sub resource\n\
            /list : based on what is being used list all the sub resources\n\
            /info : based on what is being used list the current\r\n");
    else
        dprintf(client, "501 Error syntax in parameters or arguments\r\n");
}

void command_not_found(server_t *server, int client, int id)
{
    (void)server;
    (void)id;
    dprintf(client, "500 Syntax error, command unrecognized.\r\n");
}

void wich_commands(server_t *server, int client, int id, cmds_t *ptr_command)
{
    char *cmd = format_cmd(server->command);
    bool found = false;

    for (int i = 0; i < 14; i++) {
        if (strcmp(ptr_command[i].command, cmd) == 0) {
            ptr_command[i].ptr(server, client, id);
            found = true;
            break;
        }
    }
    if (strlen(cmd) != 0) {
        (found == false) ? command_not_found(server, client, id) : (0);
    } else
        command_not_found(server, client, id);
    free(cmd);
}

void exec_commands(server_t *server, int client, int id)
{
    cmds_t ptr_command[14] = {{"/logout", remove_client},
        {"/help", help_client}, {"/messages", client_mess},
        {"/login", login_user}, {"/users", users_list},
        {"/send", send_messages}, {"/user", user}, {"/create", create},
        {"/use", use}, {"/list", list}, {"/info", info},
        {"/unsubscribe", unsubscribe}, {"/subscribed", subscribed},
        {"/subscribe", subscribe}};

    remove_chars(server->command, '\r');
    remove_chars(server->command, '\n');
    wich_commands(server, client, id, ptr_command);
}

void control_c(int __attribute__((unused)) contrl)
{
    keepRunning = false;
}
