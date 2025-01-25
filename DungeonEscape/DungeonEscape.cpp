#include <iostream>
#include <fstream>
using namespace std;

struct Point {
	size_t x;
	size_t y;
};

struct Player {
	Point position;
	int lives = 3;
	unsigned long long coins = 0;
	bool key = false;
};

char pause_screen[8][30] = {
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'a', 'u', 's', 'e', ' ', 'm', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '1', ']', ' ', 'R', 'e', 's', 'u', 'm', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '2', ']', ' ', 'S', 'a', 'v', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '3', ']', ' ', 'M', 'a', 'i', 'n', ' ', 'M', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '4', ']', ' ', 'E', 'x', 'i', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'}
};
char** map;
size_t map_width;
size_t map_height;
Point* portal_data;
size_t portal_data_size;
size_t display_width;
size_t display_height;
Player player;

void displayMap()
{
	for (size_t i = 0; i < map_height; i++)
	{
		cout << map[i] << endl;
	}
}

void switchMapElements(Point cell1, Point cell2, bool clearCell2)
{
	if (clearCell2) {
		map[cell2.y][cell2.x] = ' ';
	}
	char temp = map[cell1.y][cell1.x];
	map[cell1.y][cell1.x] = map[cell2.y][cell2.x];
	map[cell2.y][cell2.x] = temp;
}

void changePlayerPosition(long long x, long long y)
{
	if (player.position.x + x < 0)
	{
		x = player.position.x;
	}
	if (player.position.y + y < 0)
	{
		y = player.position.y;
	}
	if (player.position.x + x > map_width)
	{
		x = map_width - player.position.x;
	}
	if (player.position.y + y > map_height)
	{
		y = map_height - player.position.y;
	}
	Point new_position = { player.position.x + x,player.position.y + y };
	switch (map[new_position.y][new_position.x])
	{
		case '#':
		case 'S':
		case 'G':
			player.lives--;
			break;
		case 'C':
			//Update coins
			player.coins++;

			//Update map
			switchMapElements(player.position, new_position, true);

			//Update player position
			player.position = new_position;

			break;
		case '%':
			//Call a function that reads the map data using the position of the portal and calling this function again to change the player position
			break;
		case '&':
			//Update key status
			player.key = true;

			//Update map
			switchMapElements(player.position, new_position, true);

			//Update player position
			player.position = new_position;

			break;
		case 'X':
			if (player.key) {
				//End the sub-level
			}
			else {
				//Display message that you don't have the key
			}
			break;
		default:
			switchMapElements(player.position, new_position, false);
			player.position = new_position;
			break;
	}
}

void pauseMenu()
{
	char pause_input;
	char pause_message[100] = "\0";
	while (true)
	{
		displayPauseMenu();
		cout << endl << endl << pause_message << endl;
		cout << "\nEnter your option: ";
		cin >> pause_input;
		switch (pause_input)
		{
		case '1':
			return;
			break;
		case '2':
			//Save progress
			break;
		case '3':

		}
	}
}

//Returns
//0 - Everything went fine
//1 - Something went wrong
//2 - Invalid input
//3 - End Game
int step(char input)
{
	switch (input)
	{
	case 'w':
	case 'W':
		changePlayerPosition(0, -1);
		break;
	case 's':
	case 'S':
		changePlayerPosition(0, 1);
		break;
	case 'a':
	case 'A':
		changePlayerPosition(-1, 0);
		break;
	case 'd':
	case 'D':
		changePlayerPosition(1, 0);
		break;
	case 'p':
	case 'P':
		pauseMenu();
		break;
	default:
		return 2;
		//break;
	}
	//Make a step for enemies
	return 0; //Temporary
}

//Returns
//0 - Player was found and location is noted
//1 - Player was not found - map is invalid
int findPlayer()
{
	for (size_t i = 0; i < map_height; i++)
	{
		for (size_t j = 0; j < map_height; j++)
		{
			if (map[i][j] == '@')
			{
				player.position.x = j;
				player.position.y = i;
				player.coins = 0;
				player.key = false;
				player.lives = 3;
				return 0;
			}
		}
	}
	return 1;
}

//Returns
//0 - Verification successful
//1 - Verification failed
int verifyValidMapDimentions(char line[], size_t line_size)
{
	bool reading_height_data = false;
	for (size_t i=0;i<line_size;i++)
	{
		if (line[i] == '\0')
		{
			if (!reading_height_data)
			{
				return 1;
			}
			break;
		}
		if (!(line[i] >= '0' && line[i] <= '9'))
		{
			if ((line[i] == 'x' || line[i] == 'X') && !reading_height_data)
			{
				reading_height_data = true;
				continue;
			}
			return 1;
		}
	}
	return 0;
}

void extractMapDimentions(char line[])
{
	size_t position = 0;
	while (line[position] != 'x' && line[position] != 'X')
	{
		map_width *= 10;
		map_width += line[position] - '0';
		position++;
	}
	position++;
	while (line[position] != '\0' && line[position] != ' ')
	{
		map_height *= 10;
		map_height += line[position] - '0';
		position++;
	}
}

//Returns
//0 - Extraction was successful
//1 - Extraction failed
int extractMap(ifstream& file)
{
	map = new char* [map_height];
	for (size_t i = 0; i < map_height; i++)
	{
		map[i] = new char[map_width+1];
	}
	for (size_t i = 0; i < map_height; i++) {
		file.getline(map[i], map_width+1);
		cout << map[i];
		if (file.eof())
		{
			if (i < map_height - 1)
			{
				return 1;
			}
		}
	}
	if (findPlayer() == 1) {
		return 1;
	}
	return 0;
}

//Returns
//0 - Verification successful
//1 - Verification failed
int verifyValidPortalDataSize(char line[], size_t line_size)
{
	for (size_t i = 0; i < line_size; i++)
	{
		if (line[i] == '\0')
		{
			break;
		}
		if (!(line[i] >= '0' && line[i] <= '9'))
		{
			return 1;
		}
	}
	return 0;
}

void extractPortalDataSize(char line[])
{
	size_t position = 0;
	while (line[position] != '\0' && line[position] != ' ')
	{
		portal_data_size *= 10;
		portal_data_size += line[position] - '0';
		position++;
	}
}

//Returns
//0 - Extraction was successful
//1 - Extraction failed
int extractPortalData(ifstream& file)
{
	portal_data = new Point[portal_data_size * 2];
	bool calculating_y = false;
	bool point_1_finished = false;
	char* line = new char[map_width * 4 + 3];
	for (size_t i = 0; i < portal_data_size*2; i+=2)
	{
		calculating_y = false;
		point_1_finished = false;
		Point portal_location = { 0,0 };
		Point portal_destination = { 0,0 };
		file.getline(line, map_width * 4 + 3);
		for (size_t j = 0; j < map_width * 4 + 3; j++)
		{
			if (line[j] == '\0' && (!calculating_y || !point_1_finished))
			{
				return 1;
			}
			if (line[j] == 'x' || line[j] == 'X')
			{
				calculating_y = true;
				continue;
			}
			if (line[j] == '|')
			{
				calculating_y = false;
				point_1_finished = true;
			}
			if (point_1_finished)
			{
				if (calculating_y)
				{
					portal_destination.y *= 10;
					portal_destination.y += line[j] - '0';
				}
				portal_destination.x *= 10;
				portal_destination.x += line[j] - '0';
			}
			else
			{
				if (calculating_y)
				{
					portal_location.y *= 10;
					portal_location.y += line[j] - '0';
				}
				portal_location.x *= 10;
				portal_location.x += line[j] - '0';
			}
		}
		portal_data[i] = portal_location;
		portal_data[i + 1] = portal_destination;
	}
	return 0;
}

void freeMapData()
{
	for (size_t i = 0; i < map_height; i++)
	{
		delete(map[i]);
	}
	delete(map);
	map_width = 0;
	map_height = 0;
}

void freePortalData()
{
	delete(portal_data);
	portal_data_size = 0;
}

//Returns
//0 - Reading Map Data was successful
//1 - Reading Map Data has failed
int readMapData(char file_path[])
{
	ifstream map_file(file_path);

	if (!map_file.is_open()) {
		return 1;
	}

	size_t string_size = 100;
	char* first_line = new char[string_size];
	map_file.getline(first_line, string_size);
	if (verifyValidMapDimentions(first_line, string_size) == 1)
	{
		return 1;
	}
	extractMapDimentions(first_line);
	if (extractMap(map_file) == 1)
	{
		freeMapData();
		return 1;
	}
	first_line[0] = '\0';
	map_file.getline(first_line, string_size);
	if (verifyValidPortalDataSize(first_line, string_size) == 1)
	{
		return 1;
	}
	extractPortalDataSize(first_line);
	if (extractPortalData(map_file) == 1)
	{
		freeMapData();
		freePortalData();
		return 1;
	}
	//Extract note data
	delete(first_line);
	map_file.close();
	return 0;
}

int main()
{
	char file[] = "./Files/Maps/test_map.txt";
	if (readMapData(file) == 1)
	{
		cout << "Something went wrong while trying to open the Map Data file";
		return 0;
	}
	char input;

	while (true) {
		displayMap();
		cout << "\n\n\nEnter your input: ";
		cin >> input;
		switch (step(input))
		{
			case 1:
				cout << "Something major has gone wrong";
				break;
			case 2:
				return 0;
				break;
			case 3:
				freeMapData();
				//Go to menu cycle
			default:
				break;
		}
		system("cls");
	}
	return 0;
}