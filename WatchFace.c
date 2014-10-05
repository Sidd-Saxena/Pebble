#include <pebble.h>

Window *window;
TextLayer *text_layer;
TextLayer *date_layer;
InverterLayer *inverter_layer;

void handle_timechanges(struct tm *tick_time, TimeUnits units_changed){
  static char time_buffer[10];
  static char date_buffer[10];
  
  //give time( buffer, size of buffer, format of time, actual time)
  strftime(time_buffer,sizeof(time_buffer), "%H:%M:%S",tick_time);
  strftime(date_buffer,sizeof(date_buffer), "%b %e",tick_time);
  //update the text layer
  //time buffer still needs to be availiable when system restores
  text_layer_set_text(text_layer,time_buffer);
  text_layer_set_text(date_layer,date_buffer); 
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 154));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "Hello World");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

	// Push the window
	window_stack_push(window, true);
  
  //call the time function manually to avoid a delay lag
  time_t now = time(NULL);
  handle_timechanges(localtime(&now),SECOND_UNIT);
  
  //time handler
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechanges);
	
  //initiliaze the new text layer
  date_layer=text_layer_create(GRect(0,112,144,56));
  text_layer_set_text_alignment(date_layer,GTextAlignmentCenter);
  text_layer_set_font(date_layer,fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  
  //Add into the window
  layer_add_child(window_get_root_layer(window),text_layer_get_layer(date_layer));
    
  //initialize the inverter layer
  inverter_layer=inverter_layer_create(GRect(0,112,144,56));
  layer_add_child(window_get_root_layer(window),inverter_layer_get_layer(inverter_layer));

	// App Logging
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Hello world from the app logs..!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	text_layer_destroy(date_layer);
  inverter_layer_destroy(inverter_layer);
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
