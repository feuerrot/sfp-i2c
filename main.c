#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fh_i2c;
char i2c_bus[] = "/dev/i2c-2";

int sfp_info = 0x50;
int sfp_diag = 0x51;

uint8_t data_info[256];
uint8_t data_diag[256];

void _log(int level, char *log){
	printf("[");

	switch (level){
		case 0:
			printf("∀");
			break;
		case 1:
			printf("+");
			break;
		case 2:
			printf("!");
			break;
		case 3:
			printf("?");
			break;
	}

	printf("] ");
	printf(log);
	printf("\n");
}

float conv(uint8_t *data, uint16_t div, uint8_t sig){

	if (sig){
		return ((int16_t)  ((data[0] << 8) + data[1]))/(float) div;
	} else {
		return ((uint16_t) ((data[0] << 8) + data[1]))/(float) div;
	}
}

void open_i2c(void){
	fh_i2c = open(i2c_bus, O_RDWR);
	if (fh_i2c < 0){
		_log(2, "fh_i2c < 0");
		printf("Errno: %i", errno);
		exit(1);
	}
}

void close_i2c(void){
	close(fh_i2c);
}

void set_i2c_slave(int slave){
	if (ioctl(fh_i2c, I2C_SLAVE, slave) < 0){
		_log(2, "ioctl < 0");
		exit(1);
	}
}

void dump_i2c_slave(uint8_t *data){
	char buf[1] = {0x00};

	write(fh_i2c, buf, 1);

	for (int i=0; i <= 255; i++){
		read(fh_i2c, buf, 1);
		data[i] = buf[0];
	}
}	

void print_data(uint8_t *data){
	for (int i=0; i <= 255; i += 16){
		printf("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
		data[i], data[i+1], data[i+2], data[i+3], data[i+4],
		data[i+5], data[i+6], data[i+7], data[i+8], data[i+9],
		data[i+10], data[i+11], data[i+12], data[i+13], data[i+14],
		data[i+15]);
	}
}

void print_part(uint8_t *data, char *ptype, char *prefix, int start, int len){
	printf("%s: ", prefix);
	for (int i=start; i<start+len; i++){
		printf(ptype, data[i]);
	}
	printf("\n");
}

void interpret_data(uint8_t *data){
	print_part(&data[0], "%c",   "Hersteller", 20, 16);
	print_part(&data[0], "%02X", "OUI", 37, 3);
	print_part(&data[0], "%c",   "Rev", 56, 4);
	print_part(&data[0], "%c",   "PN", 40, 16);
	print_part(&data[0], "%c",   "SN", 68, 16);
	print_part(&data[0], "%c",   "DC", 84,  6);

	printf("Typ: 0x%02X\n", data[0]);
	printf("Connector: 0x%02X\n", data[2]);
	printf("Bitrate: %u MBd\n", data[12]*100);
	printf("Wavelength: %u nm\n", data[60] * 256 + data[61]);
	printf("             %-6s %-6s %-6s %-6s %-6s\n", "SM", "OM1", "OM2", "OM3", "OM4");
	printf("Max length: %3u km %4u m %4u m %4u m %4u m\n", data[14], data[17]*10, data[16]*10, data[19]*10, data[18]*10);

	printf("Datacheck : ");
	uint8_t foo = 0;
	for (int i=0; i<= 62; i++){
		foo += data[i];
	}
	if (foo == data[63]){
		printf("OK 0x%02X\n", data[63]);
	} else {
		printf("ERROR: 0x%02X vs. 0x%02X\n", foo, data[63]);
	}
}

void interpret_diag(uint8_t *diag){
	printf("\n");
	printf("            Temperatur   VCC    TX bias   TX power  RX power  Laser Temp  TEC\n");
	printf("+ Error  :  % 7.2f °C % 4.2f V  %5.2f mA % 2.3f mW  %.3f mW % 7.2f °C % 5.2f mA\n", conv(&diag[0], 256, 1), conv(&diag[8], 10000, 0), conv(&diag[16], 500, 0), conv(&diag[24], 10000, 0), conv(&diag[32], 10000, 0), conv(&diag[40], 256, 1), conv(&diag[48], 10, 1));
	printf("+ Warning:  % 7.2f °C % 4.2f V  %5.2f mA % 2.3f mW  %.3f mW % 7.2f °C % 5.2f mA\n", conv(&diag[4], 256, 1), conv(&diag[12], 10000, 0), conv(&diag[20], 500, 0), conv(&diag[28], 10000, 0), conv(&diag[36], 10000, 0), conv(&diag[44], 256, 1), conv(&diag[52], 10, 1));
	printf("  Value  :  % 7.2f °C % 4.2f V  %5.2f mA % 2.3f mW  %.3f mW % 7.2f °C % 5.2f mA\n", conv(&diag[96], 256, 1), conv(&diag[98], 10000, 0), conv(&diag[100], 500, 0), conv(&diag[102], 10000, 0), conv(&diag[104], 10000, 0), conv(&diag[106], 256, 1), conv(&diag[108], 10, 1));
	printf("- Warning:  % 7.2f °C % 4.2f V  %5.2f mA % 2.3f mW  %.3f mW % 7.2f °C % 5.2f mA\n", conv(&diag[6], 256, 1), conv(&diag[14], 10000, 0), conv(&diag[22], 500, 0), conv(&diag[30], 10000, 0), conv(&diag[38], 10000, 0), conv(&diag[46], 256, 1), conv(&diag[54], 10, 1));
	printf("- Error  :  % 7.2f °C % 4.2f V  %5.2f mA % 2.3f mW  %.3f mW % 7.2f °C % 5.2f mA\n", conv(&diag[2], 256, 1), conv(&diag[10], 10000, 0), conv(&diag[18], 500, 0), conv(&diag[26], 10000, 0), conv(&diag[34], 10000, 0), conv(&diag[42], 256, 1), conv(&diag[50], 10, 1));
}

int main(void){
	_log(0, "Ohai");
	open_i2c();
	set_i2c_slave(sfp_info);
	dump_i2c_slave(&data_info[0]);
	_log(1, "INFO");
	print_data(&data_info[0]);
	close_i2c();
	printf("\n");
	open_i2c();
	set_i2c_slave(sfp_diag);
	dump_i2c_slave(&data_diag[0]);
	_log(1, "DIAG");
	print_data(&data_diag[0]);
	printf("\n");
	_log(1, "Interpretiert:");
	interpret_data(&data_info[0]);
	interpret_diag(&data_diag[0]);

	return 0;
}
