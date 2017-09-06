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
#include <math.h>

#define SERVER_IP           "10.27.99.111"
#define SERVER_PORT         13003
#define DATA_BUFFER_SIZE    1024

static struct termios old, new;
int client_socket;

char getch(void);
char getch_(int echo);
char getch_(int echo);
void initTermios(int echo);
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

int32_t sqrt_measure(int32_t x)
{
	int32_t op, res, one;
	op = x;
	res = 0;
	one = 1 << 30;
	while (one > op) one >>= 2;
	while (one != 0) {
		if (op >= res + one) {
			op = op - (res + one);
			res = res +  2 * one;
		}
		res /= 2;
		one /= 4;
	}
	return(res);
}

static void information_win (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Team Liwiro - car project");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

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

int send_message(uint8_t x, uint8_t y, uint8_t length)
{
	// Get the message from the user
	uint8_t msg[4];
	printf("Enter the message to send: \n");
        msg[0] = 1;
		msg[1] = x;
		msg[2] = y;
		msg[3] = length;
	// Send the message to the servers
	int sent_bytes = send(client_socket, msg, 4, 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

static void clear_surface (GtkWidget *widget)
{
	cairo_t *cr;
	cr = cairo_create (surface);
	cairo_set_source_rgb (cr, 0.1, 0.51, 0.41);
	cairo_paint (cr);
	cairo_set_line_width(cr, 9);  
	cairo_set_source_rgb(cr, 1, 1, 0);
	cairo_translate(cr, 100, 100);
	cairo_arc(cr, 0, 0, 95, 0, 2 * M_PI);
	cairo_stroke_preserve(cr);
	cairo_fill (cr);
	
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width (cr, 1);
	cairo_translate(cr, 0, 0);
	int i, j;
	for (i = -100; i <= 100 - 1; i++ ) {
		for (j = -100; j <= 100 - 1; j++ ) {
			cairo_move_to(cr, 0, i);
			cairo_line_to(cr, 0, j);
		}
	}
	for (i = -100; i <= 100 - 1; i++ ) {
		for (j = -100; j <= 100 - 1; j++ ) {
			cairo_move_to(cr, i, 0);
			cairo_line_to(cr, j, 0);
		}
	}
	cairo_stroke(cr);
	
	cairo_destroy (cr);
	gtk_widget_queue_draw_area (widget, 0, 0, 200, 200);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
	if (surface)
		cairo_surface_destroy (surface);	
	surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));
	/* Initialize the surface to white */
	clear_surface (widget);
	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_surface (cr, surface, 0, 0);
	cairo_paint (cr);
	return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
	printf("x=%lf\ty=%lf\n", x, y);
	int32_t length2 = ((int32_t)x-100)*((int32_t)x-100) + ((int32_t)y-100)*((int32_t)y-100);
	int32_t length = sqrt_measure(length2);	
	if(length <= 100){
		send_message((uint8_t)x+36, (uint8_t)y+36, (uint8_t)length);	
		clear_surface(widget);
		cairo_t *cr;
		/* Paint to the surface, where we store our state */
		cr = cairo_create (surface);
		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_rectangle (cr, x - 3, y - 3, 6, 6);
		cairo_fill (cr);
		cairo_destroy (cr);
		/* Now invalidate the affected region of the drawing area. */
		gtk_widget_queue_draw_area (widget, x - 3, y - 3, 6, 6);
	}
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL)
		return FALSE;
	if (event->button == GDK_BUTTON_PRIMARY) {
		draw_brush (widget, event->x, event->y);
	} else if (event->button == GDK_BUTTON_SECONDARY) {
		clear_surface (widget);
		gtk_widget_queue_draw (widget);
    }
	/* We've handled the event, stop processing */
	return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL)
		return FALSE;
	if (event->state & GDK_BUTTON1_MASK)
		draw_brush (widget, event->x, event->y);
	/* We've handled it, stop processing */
	return TRUE;
}

static void close_window (void)
{
	if (surface)
		cairo_surface_destroy (surface);
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
	GObject *event_box;
	GObject *drawing_area;

	gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "builder.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	window = gtk_builder_get_object (builder, "window");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	event_box = gtk_builder_get_object (builder, "eventboxup");
	g_signal_connect (event_box, "button_press_event", G_CALLBACK (send_message_up), NULL);
	g_signal_connect (event_box, "composited-changed", G_CALLBACK (send_message_up), NULL);
	event_box = gtk_builder_get_object (builder, "eventboxdown");
	g_signal_connect (event_box, "button_press_event", G_CALLBACK (send_message_down), NULL);
	g_signal_connect (event_box, "composited-changed", G_CALLBACK (send_message_down), NULL);
	event_box = gtk_builder_get_object (builder, "eventboxleft");
	g_signal_connect (event_box, "button_press_event", G_CALLBACK (send_message_left), NULL);
	g_signal_connect (event_box, "composited-changed", G_CALLBACK (send_message_left), NULL);
	event_box = gtk_builder_get_object (builder, "eventboxright");
	g_signal_connect (event_box, "button_press_event", G_CALLBACK (send_message_right), NULL);
	g_signal_connect (event_box, "composited-changed", G_CALLBACK (send_message_right), NULL);
	event_box = gtk_builder_get_object (builder, "eventboxstop");
	g_signal_connect (event_box, "button_press_event", G_CALLBACK (send_message_stop), NULL);
	g_signal_connect (event_box, "composited-changed", G_CALLBACK (send_message_stop), NULL);
	button = gtk_builder_get_object (builder, "quit");
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	button = gtk_builder_get_object (builder, "information");
	g_signal_connect (button, "clicked", G_CALLBACK (information_win), NULL);
	
	drawing_area = gtk_builder_get_object (builder, "drawingarea1");
	g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
	g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
	/* Event signals */
	g_signal_connect (drawing_area, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), NULL);
	g_signal_connect (drawing_area, "button-press-event", G_CALLBACK (button_press_event_cb), NULL);

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