#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <termios.h>

#define SERVER_IP           "10.27.99.89"
#define SERVER_PORT         13003
#define DATA_BUFFER_SIZE    1024

static struct termios old, new;
int client_socket;

char getch(void);
char getch_(int echo);
char getch_(int echo);
void initTermios(int echo);

void handle_error(const char *error_string)
{
	printf("Press any key to exit from the program...\n");
}

void connect_to_server(int *client_sock, unsigned int server_port, char *server_ip)
{
	// Creating client socket
	(*client_sock) = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client_sock < 0)
		handle_error("socket() ");

	// Creating server address
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(server_port);
	addr_in.sin_addr.s_addr = inet_addr(server_ip);

	// Connecting the client socket to the server
	int connect_retval = connect(*client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
	if (connect_retval < 0)
		handle_error("connect() ");

	printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);
}

int send_message_up(void)
{
	// Get the message from the user
	uint8_t msg[4];
	printf("Enter the message to send: \n");
        msg[0] = 0;
		msg[1] = 1;
		msg[2] = 3;
		msg[3] = 1;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

int send_message_down(void)
{
	// Get the message from the user
	uint8_t msg[4];
	printf("Enter the message to send: \n");
        msg[0] = 0;
		msg[1] = 2;
		msg[2] = 3;
		msg[3] = 1;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

int send_message_left(void)
{
	// Get the message from the user
	uint8_t msg[4];
	printf("Enter the message to send: \n");
        msg[0] = 0;
		msg[1] = 3;
		msg[2] = 3;
		msg[3] = 1;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

int send_message_right(void)
{
	// Get the message from the user
	uint8_t msg[4];
	printf("Enter the message to send: \n");
        msg[0] = 0;
		msg[1] = 4;
		msg[2] = 3;
		msg[3] = 1;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

int send_message_stop(void)
{
	// Get the message from the user
	uint8_t msg[4];
	printf("Enter the message to send: \n");
        msg[0] = 0;
		msg[1] = 5;
		msg[2] = 3;
		msg[3] = 0;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

int main (int   argc, char *argv[])
{
	int sent_bytes;
	int received_bytes;
	char recv_buff[DATA_BUFFER_SIZE];
	connect_to_server(&client_socket, SERVER_PORT, SERVER_IP);

	GtkBuilder *builder;
	GObject *window;
	GObject *button;

	gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "builder.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	window = gtk_builder_get_object (builder, "window");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	button = gtk_builder_get_object (builder, "button_up");
	g_signal_connect (button, "clicked", G_CALLBACK (send_message_up), NULL);
	button = gtk_builder_get_object (builder, "button_down");
	g_signal_connect (button, "clicked", G_CALLBACK (send_message_down), NULL);
	button = gtk_builder_get_object (builder, "button_left");
	g_signal_connect (button, "clicked", G_CALLBACK (send_message_left), NULL);
	button = gtk_builder_get_object (builder, "button_right");
	g_signal_connect (button, "clicked", G_CALLBACK (send_message_right), NULL);
	button = gtk_builder_get_object (builder, "button_stop");
	g_signal_connect (button, "clicked", G_CALLBACK (send_message_stop), NULL);
	button = gtk_builder_get_object (builder, "quit");
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

	gtk_main ();    

    printf("Closing the socket...\n");
    close(client_socket);
    printf("Cleaning up memory...\n");

    return 0;
}

void initTermios(int echo)
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void)
{
	return getch_(0);
}