/*
 * Program to automatically obtain the hardware address of a network device (MAC address)
 *
 * Copyright 2012 Fahlstrom Research LLC
 *
 * http://www.fahlstrom-research.com
 *
 * Author : Carl A. Fahlstrom
 *
 * This software is provided without warranty of any kind.
 *
 * This software is provided under the terms of the Creative Commons Attribution-NoDerivs 3.0 Unported
 * this license is available from http://creativecommons.org/licenses/by-nd/3.0/legalcode
 *
 * The following terms are added to the CC-BY-ND license:
 *
 * Attribution is to include a message viable to end users of the program.
 * Fahlstrom Research grants permission to make modifications only for personal use or within your organization.
 * You may not distribute modified versions or derivative works.
 * You may distribute the code unmodified.  Those you redistribute to are also bound by all terms of this license.
 * You are not to remove, comment, modify, or cause not to be compile by other means code between:
 *
 * 	//=======Do not remove==========
 * 	code not to be removed
 *	//==============================
 *
 * The software is currently under active development, use with caution.
 *
 */

#include"get_mac_add.h"

// We will define these variables globally because they need to be seen by
// both the main function that runs the GUI and the callback function that is
// called when the submit button is pushed.
gchar* name_first_t = NULL;
gchar* name_last_t = NULL;
gchar* group_t = NULL;
gchar* room_t = NULL;
gchar* phone_t = NULL;
gboolean laptop_b, desktop_b, visitor_b, member_b;
GtkWidget *name_first_f, *name_last_f, *group_f, *room_f, *phone_f;
GtkWidget *laptop_cb, *desktop_cb, *visitor_cb, *member_cb;
//GtkWidget *laptop_tb, *desktop_tb, *visitor_tb, *member_tb;

static void hello(GtkWidget *widget, gpointer data)
{
	// Change this string to the title you wish the main title to have
	//
	const char* win_main_title = "";
	FILE* ifconfig_out;
	FILE* ifconfig_outd;
	FILE* ifconfig_outu;
	#ifdef MACOSX
		const char* ifconfig_command="ifconfig -a";
	#endif
	#ifdef LINUX
		const char* ifconfig_command="ifconfig -a";
	#endif
	#ifdef WIN
		const char* ifconfig_command="ipconfig -a";
	#endif
	const char* ifconfig_commandu="ifconfig en0 up";
	const char* ifconfig_commandd="ifconfig en0 down";
	const char* ifconfig_mode="r";
	const char* comp_str1 = "en0:";
	#ifdef MACOSX
		const char* comp_str2 = "ether";
	#endif
	#ifdef LINUX
		const char* comp_str2 = "HWaddr";
	#endif
	#ifdef WIN
	#endif
	gboolean thisismacadd = FALSE;
	char ifconfig_line[80];
	GtkWidget *end_window;
	GtkWidget *end_button;
	GtkWidget *end_box;
	GtkWidget *end_label;
	GtkWidget *error_window1;
	GtkWidget *error_button1;
	GtkWidget *error_box1;
	GtkWidget *error_label1;
	GtkWidget *error_window2;
	GtkWidget *error_button2;
	GtkWidget *error_box2;
	GtkWidget *error_label2;

	char* member_t = "0";
	char* visitor_t = "0";
	char* laptop_t = "0";
	char* desktop_t = "0";
	char* mac_add_t;

	MYSQL* mysql_obj;
	mysql_obj = mysql_init(NULL);
//	const char* my_hostname = "";
	const char* my_hostname = "";
	const char* my_uname = "";
//	const char* my_passwd = "";
	const char* my_passwd = "";
	const char* my_db = "mac_add";
	char* my_q_tmp = "insert into computers values(\'";
	char my_query_v[256];

	name_first_t = gtk_entry_get_text(name_first_f);
	name_last_t = gtk_entry_get_text(name_last_f);
	group_t = gtk_entry_get_text(group_f);
	room_t = gtk_entry_get_text(room_f);
	phone_t = gtk_entry_get_text(phone_f);

	laptop_b = gtk_toggle_button_get_active(laptop_cb);
	desktop_b = gtk_toggle_button_get_active(desktop_cb);
	visitor_b = gtk_toggle_button_get_active(visitor_cb);
	member_b = gtk_toggle_button_get_active(member_cb);

	const char* sp = "\', \'";
	const char* sq = ", ";

	// This part of the code is currently not platform independent.
	// Here we access administrator privileges in order to reset the 
	// Network after sending the MAC address to the MySQL server.
	//
	#ifdef MACOSX_AUTH
		OSStatus auth_code;
		const AuthorizationRights *ipconfig_rights;
		const AuthorizationEnvironment *ipconfig_env;
		AuthorizationFlags ipconfig_flags = kAuthorizationFlagExtendRights;
		AuthorizationRef *ipconfig_auth;
	#endif

	#ifdef LINUX_AUTH
	#endif

	#ifdef WIN_AUTH
	#endif

	mysql_real_connect(mysql_obj, my_hostname, my_uname, my_passwd, my_db, 0, NULL, 0);

	if (name_first_t == "\0")
	{
		g_print("You must enter data in all required fields");
	}

	if (laptop_b)
	{
		g_print("This is a laptop\n");
		laptop_t = "1";
	}

	if (desktop_b)
	{
		g_print("This is a desktop\n");
		desktop_t = "1";
	}

	if (visitor_b)
	{
		g_print("I am a Visitor\n");
		visitor_t = "1";
	}

	if (member_b)
	{
		g_print("I am a Member\n");
		member_t = "1";
	}

	if (laptop_b && desktop_b)
	{
		g_print("This computer cannot be both a laptop and desktop\n");
		error_window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		error_box1 = gtk_vbox_new(TRUE, 0);
		error_label1 = gtk_label_new("This computer cannot be both a laptop and desktop");
//		error_button1 = gtk_button_new_with_label("Retry");
		
		gtk_container_add(GTK_CONTAINER(error_window1), error_box1);

		gtk_box_pack_start(GTK_BOX(error_box1), error_label1, TRUE, TRUE, 0);
//		gtk_box_pack_start(GTK_BOX(error_box1), error_button1, TRUE, TRUE, 0);

//		g_signal_connect (error_button1, "clicked", G_CALLBACK (), NULL);

//		gtk_widget_show(error_button1);
		gtk_widget_show(error_label1);
		gtk_widget_show(error_box1);
	}

	if (visitor_b && member_b)
	{
		g_print("You cannot be both a visitor and a member");
		error_window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		error_box2 = gtk_vbox_new(TRUE, 0);
		error_label2 = gtk_label_new("You cannot be both a visitor and a member");
//		error_button2 = gtk_button_new_with_label("Retry");
		
		gtk_container_add(GTK_CONTAINER(error_window2), error_box2);

		gtk_box_pack_start(GTK_BOX(error_box2), error_label2, TRUE, TRUE, 0);
//		gtk_box_pack_start(GTK_BOX(error_box2), error_button2, TRUE, TRUE, 0);

//		g_signal_connect (error_button2, "clicked", G_CALLBACK (), NULL);

//		gtk_widget_show(error_button2);
		gtk_widget_show(error_label2);
		gtk_widget_show(error_box2);
	}

	g_print("%s\n%s\n%s\n%s\n%s\n", name_first_t, name_last_t, group_t, room_t, phone_t);


	end_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	end_box = gtk_vbox_new(TRUE, 0);
	end_label = gtk_label_new("Please wait about 5 min for your computer to be added to the network");
	end_button = gtk_button_new_with_label("Exit");

	gtk_container_add(GTK_CONTAINER(end_window), end_box);

	gtk_box_pack_start(GTK_BOX(end_box), end_label, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(end_box), end_button, TRUE, TRUE, 0);

	g_signal_connect (end_button, "clicked", G_CALLBACK (endoftheroad), NULL);

	gtk_widget_show(end_button);
	gtk_widget_show(end_label);
	gtk_widget_show(end_box);
	gtk_widget_show(end_window);

	ifconfig_out = popen(ifconfig_command, ifconfig_mode);

	while(!feof(ifconfig_out))
	{
		fscanf(ifconfig_out, "%s", &ifconfig_line);
//		if (*ifconfig_line == "en0 ")
		if (thisismacadd == TRUE)
		{
			g_print("%s\n", ifconfig_line);
			mac_add_t = ifconfig_line;
			break;
		}
		if (!strcmp(comp_str2, ifconfig_line))
		{
			thisismacadd = TRUE;
		}
//		g_print("%s\n", ifconfig_line);
	}

	snprintf(my_query_v, sizeof(my_query_v), "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", my_q_tmp, name_first_t, sp, name_last_t, sp, group_t, sp, room_t, sp, phone_t, "\', ", laptop_t, sq, desktop_t, sq, visitor_t, sq, member_t, ", \'", mac_add_t, "\');");

	g_print("%s\n", my_query_v);
//	my_query = my_query_v;

	#ifdef MACOSX_AUTH
		auth_code = AuthorizationCreate(ipconfig_rights, ipconfig_env, ipconfig_flags, ipconfig_auth);
	#endif

//	ifconfig_outd = popen(ifconfig_commandd, ifconfig_mode);

//	ifconfig_outu = popen(ifconfig_commandu, ifconfig_mode);

	mysql_query(mysql_obj, my_query_v);

}

static void endoftheroad(GtkWidget *widget, gpointer data)
{
	exit(0);
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	return FALSE;
}

static void destory(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_main_quit();
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *name_first_l, *name_last_l, *group_l, *room_l, *phone_l, *frllc_credit;
	GtkWidget *name_first_f, *name_last_f, *group_f, *room_f, *phone_f;
	GtkWidget *laptop_cb, *desktop_cb, *visitor_cb, *member_cb;
	GtkWidget *box1, *box2, *box3, *box4, *box5, *box6;
	GtkWidget *submit_button;
	GtkWidget *head_image;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
	g_signal_connect (window, "destroy", G_CALLBACK (destory), NULL);

	#ifdef SET_TITLE
	gtk_window_set_title(GTK_WINDOW(window), win_main_title);
	#endif

	gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	box1 = gtk_vbox_new(TRUE, 0);
	box2 = gtk_hbox_new(TRUE, 0);
	box3 = gtk_hbox_new(TRUE, 0);
	box4 = gtk_hbox_new(TRUE, 0);
	box5 = gtk_hbox_new(TRUE, 0);
	box6 = gtk_hbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), box1);

	#ifdef ADD_BANNER
		head_image = gtk_image_new_from_file("banner.png");
	#endif

	laptop_cb = gtk_check_button_new_with_label("This computer is a laptop");
	desktop_cb = gtk_check_button_new_with_label("This computer is a desktop");
	visitor_cb = gtk_check_button_new_with_label("I am a visitor to the Chemistry Department");
	member_cb = gtk_check_button_new_with_label("I am a member of the Chemistry Department");

	name_first_f = gtk_entry_new();
	name_last_f = gtk_entry_new();
	group_f = gtk_entry_new();
	room_f = gtk_entry_new();
	phone_f = gtk_entry_new();

	name_first_l = gtk_label_new("First Name");
	name_last_l = gtk_label_new("Last Name");
	group_l = gtk_label_new("Research Group");
	room_l = gtk_label_new("Room number");
	phone_l = gtk_label_new("Phone number");

	//
	// =======Do Not Remove========
	//  you may not cause this message to not be displayed to end users.
	frllc_credit = gtk_label_new("This software developed by Fahlstrom Research LLC.\nhttp://www.fahlstrom-research.com");
	// ============================

	submit_button = gtk_button_new_with_label("Submit");

	#ifdef ADD_BANNER
		gtk_box_pack_start(GTK_BOX(box1), head_image, TRUE, TRUE, 0);
	#endif

	gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(box2), name_first_l, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box2), name_first_f, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box1), box3, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box3), name_last_l, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box3), name_last_f, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box1), box4, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box4), group_l, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box4), group_f, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box1), box5, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box5), room_l, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box5), room_f, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box1), box6, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box6), phone_l, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box6), phone_f, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box1), laptop_cb, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box1), desktop_cb, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box1), visitor_cb, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box1), member_cb, TRUE, TRUE, 0);
	//
	// =======Do Not Remove========
	//
	gtk_box_pack_start(GTK_BOX(box1), frllc_credit, TRUE, TRUE, 0);
	// ============================
	gtk_box_pack_start(GTK_BOX(box1), submit_button, TRUE, TRUE, 0);

	#ifdef ADD_BANNER
		gtk_widget_show(head_image);
	#endif
	gtk_widget_show(name_first_l);
	gtk_widget_show(name_last_l);
	gtk_widget_show(group_l);
	gtk_widget_show(room_l);
	gtk_widget_show(phone_l);

	gtk_widget_show(name_first_f);
	gtk_widget_show(name_last_f);
	gtk_widget_show(group_f);
	gtk_widget_show(room_f);
	gtk_widget_show(phone_f);

	gtk_widget_show(laptop_cb);
	gtk_widget_show(desktop_cb);
	gtk_widget_show(visitor_cb);
	gtk_widget_show(member_cb);
	//
	// =======Do Not Remove========
	gtk_widget_show(frllc_credit);
	// ============================

	gtk_widget_show(submit_button);

	gtk_widget_show(box6);
	gtk_widget_show(box5);
	gtk_widget_show(box4);
	gtk_widget_show(box3);
	gtk_widget_show(box2);

	gtk_widget_show(box1);	

	gtk_widget_show(window);

	g_signal_connect (submit_button, "clicked", G_CALLBACK (hello), NULL);
//	g_signal_connect (submit_button, "clicked", G_CALLBACK (get_names), NULL);

	gtk_main();

	return 0;

}
