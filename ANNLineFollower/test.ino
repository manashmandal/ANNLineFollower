int sensor [] = {A0, A1, A2, A3, A4};
int digital_reading[] = {0, 0, 0, 0, 0};

int threshold = 500;

int active_sensors = 0;

const int set_point = 3000;

double kp = 0;
double kd = 0;
double ki = 0;

void read_line(void){
	active_sensors = 0;
	for (int i = 0; i < 5; i++){
		if (analogRead(sensor[i]) > threshold){
			digital_reading[i] = 1;
			active_sensors++;
		}
	} else digital_reading[i] = 0;
}

int get_position(void){
	read_line();
	int weight = 0;
	for (int i = 0; i < 5; i++){
		weight += digital_reading[i] * (i + 1) * 1000;
	}
	return (weight / active_sensors);
}

double error = 0;
double previous_error = 0;

int left_add_speed;
int right_add_speed;

void PID(void){
	previous_error = error;
	int current_position = get_position();
	error = current_position - set_point;
	int power = Kp * error + kd * (error - previous_error);

	//if we want to go to right
	if (power < 0){
		power = -power;
		if (power > 255) {
			power = 255;
		}
	} else {
		if (power > 255){
			power = 255;
		}
	}
}

