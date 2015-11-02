#include <pebble.h>

static Window *s_window;
static TextLayer *s_text;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_ana;

static void update_time()
{
  //get tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //create a long-lived buffer
  static char buffer[] = "00:00";
  
  //write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true)
  {
    //use 24h format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  }
  else
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  
  text_layer_set_text(s_text, buffer);
}

static void main_window_load(Window *window)
{
  //create gbitmap, then set to created bitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_yoru);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //create text
  s_text = text_layer_create(GRect(5, 5, 139, 50));
  text_layer_set_background_color(s_text, GColorClear);
  text_layer_set_text_color(s_text, GColorBlack);
  
  s_ana = text_layer_create(GRect(5, 55, 139, 60));
  text_layer_set_background_color(s_ana, GColorClear);
  text_layer_set_text_color(s_ana, GColorBlack);
    
  //set font ant alignment
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_perfect_font_32));
  text_layer_set_font(s_text, s_time_font);
  text_layer_set_text_alignment(s_text, GTextAlignmentCenter);
  
  text_layer_set_font(s_ana, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_ana, GTextAlignmentCenter);
  
  //link to the parent
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_ana));  
}

static void main_window_unload(Window *window)
{
  //destroy text
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(s_text);
  text_layer_destroy(s_ana);
  
  //destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  //destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time();
}

static void init() 
{
  //Create window
  s_window = window_create();
  
  //set handlers
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_window, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit()
{
  window_destroy(s_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}

