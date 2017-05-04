extern String receive_buf;
extern String str_to_parse; 
extern boolean stringComplete;

extern char c_sensor_data[6][3][10];
extern double sensor_data[6][3];
extern double sensor_data_prev[6][3];

void print_float();
void char2float_sensor_data();
void print_c_sensor_data();
void fill_data();
void get_one_line();
void blink();
