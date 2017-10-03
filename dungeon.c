#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <endian.h>

#define window_width 80
#define window_height 21
#define rock_char ' '
#define room_char '.'
#define hall_char '#'
#define folder_string "/.rlg327/"
#define file_name "dungeon"

char dungeon[window_height][window_width];
int hardness[window_height][window_width];


char *semantic;
int file_version;
int file_size;	
int n_rooms;

typedef struct room{
	int x;
	int y;
	int width;
	int height;
} room_t;

room_t *rooms;

int get_distance(int a, int b) {
	return a - b;
}

int create_paths(void) {
	int n;
	for (n = 0; n < n_rooms; n++) {
		room_t r1 = rooms[n];
		room_t r2 = n == n_rooms - 1 ? rooms[0] : rooms[n+1];
		int x1 = r1.x + r1.width / 2;
		int y1 = r1.y + r1.height / 2;
		int x2 = r2.x + r2.width / 2;
		int y2 = r2.y + r2.height / 2;

		// int d_x, d_y;

		/* if the x distance is greater than the y distance that means we want to exit from side */
		while (x1 != x2 && y1 != y2) {

			// d_x = (rand() % 3) + 1;
			// d_y = (rand() % 3) + 1;

			/* move bottom right */
			if (get_distance(x2, x1) > 0 && get_distance(y2, y1) > 0) {
				while (x1 != x2) {
					x1++;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
				while (y1 != y2) {
					y1++;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
			} /* move top right */ 
			else if (get_distance(x2, x1) > 0 && get_distance(y2, y1) < 0) {
				while (x1 != x2) {
					x1++;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
				while (y1 != y2) {
					y1--;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
			} /* move top left */
			else if (get_distance(x2, x1) < 0 && get_distance(y2, y1) < 0) {
				while (x1 != x2) {
					x1--;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
				while (y1 != y2) {
					y1--;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
			} /* move bottom left */
			else if (get_distance(x2, x1) < 0 && get_distance(y2, y1) > 0) {
				while (x1 != x2) {
					x1--;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
				while (y1 != y2) {
					y1++;
					if (dungeon[y1][x1] == rock_char) {
						dungeon[y1][x1] = hall_char;
						hardness[y1][x1] = 0;
					}
				}
			}

		}


	}


	return 0;
}

int create_room(int n){
	int width = (rand() % 13) + 4;
	int height = (rand() % 6) + 3;
	int x = (rand() % window_width - 2) + 1;
	int y = (rand() % window_height - 2) + 1;
	/* check if within bounds */
	if (x + width > window_width - 1 || y + height > window_height - 1) {
		return 1;
	}

	/* check if touching other box */

	int i, j;
	for(i = y - 1; i <= y + height; i++) {
		for(j = x - 1; j <= x + width; j++) {
			if (dungeon[i][j] != rock_char) {
				return 1;
			}
		}
	}

	for(i = y; i < y + height; i++) {
		for(j = x; j < x + width; j++) {
			dungeon[i][j] = room_char;
			hardness[i][j] = 0;
		}
	}
	room_t r;
	r.x = x;
	r.y = y;
	r.width = width;
	r.height = height;

	rooms[n] = r;

	return 0;
}

int print_board(char board[window_height][window_width]) {
	int i,j;

	for(i = 0; i < window_height; i++) {
	 	for(j=0; j < window_width; j++) {

	 		printf("%c", board[i][j]);
	 	}
	 	printf("\n");
	 }
	 return 0;
}

int create_dungeon() {
	int i, j, n;
	n_rooms = 6;
	rooms = malloc(6);
	semantic = "rlg327";
	file_version = 0;

	for(i = 0; i < window_height; i++){
		/* the first and last rows are dashes*/
	 	if (i == 0 || i == window_height - 1) {
	 		for(j = 0; j < window_width; j++){
	 			dungeon[i][j] = '-';
	 			hardness[i][j] = 255;
	 		}
	 	} else {
	 		/* first and last columns are pipes with space inbetween */
	 		dungeon[i][0] = '|';
	 		hardness[i][0] = 255;
	 		dungeon[i][window_width - 1] = '|';
	 		hardness[i][window_width - 1] = 255;
	 		for(j = 1; j < window_width - 1; j++) {
	 			dungeon[i][j] = rock_char;
	 			hardness[i][j] = (rand() % 254) + 1;
	 		}
	 	}
	 }
	 n = 0;
	 while (n < n_rooms) {
	 	if (!create_room(n)) {
	 		n++;
	 	}
	 }
	 create_paths();

	 file_size = 14 + sizeof(hardness) + n_rooms * 4;

	return 0;
}

int load_dungeon(void) {
	int i, j, n;
	/* border */
	for(i = 0; i < window_height; i++){
		/* the first and last rows are dashes*/
	 	if (i == 0 || i == window_height - 1) {
	 		for(j = 0; j < window_width; j++){
	 			dungeon[i][j] = '-';
	 		}
	 	} else {
	 		/* first and last columns are pipes with space inbetween */
	 		dungeon[i][0] = '|';
	 		dungeon[i][window_width - 1] = '|';
	 		for(j = 1; j < window_width - 1; j++) {
	 			dungeon[i][j] = rock_char;
	 		}
	 	}
	 }

	 /* add in rooms */
	 for(n = 0; n < n_rooms; n++) {
	 	room_t r = rooms[n];
	 	for(i = r.y; i < r.y + r.height; i++) {
			for(j = r.x; j < r.x + r.width; j++) {
				dungeon[i][j] = room_char;
			}
		}
	 }

	 /* add in paths */
	 for(i = 1; i < window_height - 1; i++) {
	 	for(j = 1; j < window_width - 1; j++) {
	 		if (hardness[i][j] == 0 && dungeon[i][j] != room_char) {
	 			dungeon[i][j] = hall_char;
	 		}
	 	}
	 }

	return 0;
}

int main(int argc, char *argv[]) {
	int n;
	int load = 0;
	int save = 0;
	char *home = getenv("HOME");

	srand(time(NULL));

	
	/* If there is a command line argument */
	for(n = 1; n < argc; n++) {
		if (!(strcmp(argv[n], "--save"))) {
			save = 1;
		} else if (!(strcmp(argv[n], "--load"))) {
			load = 1;
		} else {
			printf("Invalid parameter\n");
			return 1;
		}
	}

	char *address = malloc(sizeof(home) + sizeof(folder_string) + sizeof(file_name));
	strcpy(address, home);
	strcat(address, folder_string);
	mkdir(address, S_IRUSR | S_IWUSR | S_IXUSR);
	strcat(address, file_name);
	

	if (load) {
		FILE *f = fopen(address, "r");
		fread(semantic, 1, 6, f);
		fread(&file_version, 4, 1, f);
		file_version = be32toh(file_version);
		fread(&file_size, 4, 1, f);
		file_size = be32toh(file_size);
		fread(hardness, 1, window_width*window_height, f);
		
		n_rooms = (file_size - 1694) / 4;
		rooms = malloc(n_rooms);
		int i;
		for (i = 0; i < n_rooms; i++) {
			room_t r;
			fread(&r.x, 1, 1, f);
			fread(&r.width, 1, 1, f);
			fread(&r.y, 1, 1, f);
			fread(&r.height, 1, 1, f);
			rooms[n] = r;
		}


		load_dungeon();

	} else {
		create_dungeon();
	}

	print_board(dungeon);

	if (save) {
		int i;
		FILE *f = fopen(address, "w");
		fwrite(semantic, 1, 6, f);
		file_version = htobe32(file_version);
		fwrite(&file_version, 4, 1, f);
		file_size = htobe32(file_size);
		fwrite(&file_size, 4, 1, f);
		fwrite(hardness, 1, window_width*window_height, f);
		for(i = 0; i < n_rooms; i++) {
			fwrite(&rooms[i].x, 1, 1, f);
			fwrite(&rooms[i].width, 1, 1, f);
			fwrite(&rooms[i].y, 1, 1, f);
			fwrite(&rooms[i].height, 1, 1, f);
		}
		fclose(f);
	}


	
	 

	return 0;
}