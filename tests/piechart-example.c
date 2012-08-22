#include <stdlib.h>
#include <math.h>
#include <gfw.h>
 
gboolean rotate_chart_tcb( GfwPieChart *pie ) {
	static unsigned long count = 0;
	double angle;

	angle = (M_PI*2.0/100.0)*(count%100) - M_PI;
	g_object_set( pie, "start_angle", angle, NULL );

	count++;

	gtk_widget_queue_draw( (GtkWidget *)pie );

	return( TRUE );
}


void quit () {
	gtk_main_quit();
}


int main( int argc, char *argv[] ) {
	GtkWidget *win, *vbox, *pie;
	float rgba[4] = { 1.0, 1.0, 0.5, 1.0 };

	gtk_init( &argc, &argv );

	win = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_widget_set_name( win, "Pie Chart Test" );

	vbox = gtk_vbox_new( FALSE, 0 );
	gtk_container_add( GTK_CONTAINER(win), vbox );
	gtk_widget_show( vbox );

	g_signal_connect(win, "destroy", G_CALLBACK(quit), NULL );

	// Create the drawing area
	pie = gfw_pie_chart_new();
	gfw_pie_chart_add_segment(GFW_PIE_CHART(pie), 0.5, "First中国", rgba );
	rgba[0] = 0.5;
	gfw_pie_chart_add_segment(GFW_PIE_CHART(pie), 0.20, "Second", rgba );
	rgba[1] = 0.5;
	rgba[2] = 1.0;
	gfw_pie_chart_add_segment(GFW_PIE_CHART(pie), 0.10, "Third", rgba );
	rgba[1] = 0.3;
	rgba[2] = 0.9;
	/*rgba[3] = 0.5;*/
	gfw_pie_chart_add_segment(GFW_PIE_CHART(pie), 0.20, "Four", rgba );

	gtk_box_pack_start(GTK_BOX(vbox), pie, TRUE, TRUE, 0 );

	gtk_widget_show( pie );

	gtk_widget_show_all( win );

	/*g_timeout_add( 100, (GSourceFunc)rotate_chart_tcb, pie );*/

	gtk_main();

	exit( 0 );
}
