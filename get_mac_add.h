/*
 *
 *
 *
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
//#include<wx/wxprec.h>
#include<gtk/gtk.h>
#include <my_global.h>
#include <mysql.h>

#ifdef MACOSX_AUTH
	#include<Authorization.h>
#endif

#ifdef LINUX_AUTH
	#include<>
#endif

#ifdef WIN_AUTH
	#include<>
#endif

static void hello(GtkWidget*, gpointer);
static void endoftheroad(GtkWidget*, gpointer);
static gboolean delete_event(GtkWidget*,GdkEvent*, gpointer);
