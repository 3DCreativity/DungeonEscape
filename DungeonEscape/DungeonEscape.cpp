#include <iostream>
#include <fstream>
using namespace std;

#pragma region struct Definitions

struct User {
	bool new_user = true;
	char ID[200] = "Undefined\0";
	size_t coins = 0;
	size_t last_unlocked_level = 0;
	bool save_file_created = false;
	bool save_file_used = false;
};

struct Point {
	size_t x;
	size_t y;
};

struct Player {
	Point position;
	int lives = 3;
	size_t coins = 0;
	bool key = false;
};

#pragma endregion 

#pragma region Menus set-up

const size_t pause_screen_width = 30; //For Debugging Purposes
const size_t pause_screen_height = 8; //For Debugging Purposes
char pause_screen[pause_screen_height][pause_screen_width] = {
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'a', 'u', 's', 'e', ' ', 'm', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '1', ']', ' ', 'R', 'e', 's', 'u', 'm', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '2', ']', ' ', 'S', 'a', 'v', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '3', ']', ' ', 'M', 'a', 'i', 'n', ' ', 'M', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '4', ']', ' ', 'E', 'x', 'i', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'}
};

const size_t main_menu_screen_width = 30; //For Debugging Purposes
const size_t main_menu_screen_height = 10; //For Debugging Purposes
char main_menu_screen[main_menu_screen_height][main_menu_screen_width] = {
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'D', 'u', 'n', 'g', 'e', 'o', 'n', ' ', 'e', 's', 'c', 'a', 'p', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '1', ']', ' ', 'N', 'e', 'w', ' ', 'g', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '2', ']', ' ', 'C', 'o', 'n', 't', 'i', 'n', 'u', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '3', ']', ' ', 'L', 'e', 'v', 'e', 'l', ' ', 's', 'e', 'l', 'e', 'c', 't', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '4', ']', ' ', 'L', 'e', 'a', 'd', 'e', 'r', 'b', 'o', 'a', 'r', 'd', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '5', ']', ' ', 'D', 'e', 'l', 'e', 't', 'e', ' ', 'u', 's', 'e', 'r', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '6', ']', ' ', 'E', 'x', 'i', 't', ' ', 'g', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'}
};

const size_t level_select_screen_width = 30; //For Debugging Purposes
const size_t level_select_screen_height = 12; //For Debugging Purposes
char level_select_screen[level_select_screen_height][level_select_screen_width] = {
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'e', 'v', 'e', 'l', ' ', 's', 'e', 'l', 'e', 'c', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', 'v', 'a', 'i', 'l', 'a', 'b', 'l', 'e', ' ', 'L', 'e', 'v', 'e', 'l', 's', ':', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', 'D', ' ', ' ', ' ', 'N', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', 'T', 'e', 's', 't', ' ', 'l', 'e', 'v', 'e', 'l', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'}
};

#pragma endregion

//Reminder: 9 white spaces {ID} (5 - ID length) spaces {Name}

#pragma region Variables

char** map;
char map_name[100];
size_t map_width;
size_t map_height;

Point* portal_data;
size_t portal_data_size;

size_t display_width;
size_t display_height;

Player player;

User user;

#pragma endregion

#pragma region rewriteCharArray function (4 ovedrloads)

void rewriteCharArray(char* arr, const char line[])
{
	size_t index = 0;
	for (; line[index] != '\0'; index++)
	{
		arr[index] = line[index];
	}
	arr[index] = '\0';
}

void rewriteCharArray(char* arr, char line[])
{
	size_t index = 0;
	for (; line[index] != '\0'; index++)
	{
		arr[index] = line[index];
	}
	arr[index] = '\0';
}

void rewriteCharArray(char* arr, const char line[], size_t max_size)
{
	size_t index = 0;
	for (; line[index] != '\0'; index++)
	{
		if (index == max_size - 2)
		{
			break;
		}
		arr[index] = line[index];
	}
	arr[index] = '\0';
}

void rewriteCharArray(char* arr, char line[], size_t max_size)
{
	size_t index = 0;
	for (; line[index] != '\0'; index++)
	{
		if (index == max_size - 2)
		{
			break;
		}
		arr[index] = line[index];
	}
	arr[index] = '\0';
}

#pragma endregion

void waitUserInput()
{
	char temp;
	cin >> temp;
	cin.ignore();
}

void displayMainMenu()
{
	cout << main_menu_screen[0] << endl << main_menu_screen[1] << endl;
	if (user.new_user)
	{
		cout << "        Welcome, " << user.ID << "!" << endl;
	}
	else
	{
		cout << "    Welcome back, " << user.ID << "!" << endl;
	}
	for (size_t i = 2; i < main_menu_screen_height; i++)
	{
		cout << main_menu_screen[i] << endl;
	}
}

void displayMap()
{
	for (size_t i = 0; i < map_height; i++)
	{
		cout << map[i] << endl;
	}
}

void displayPauseMenu() {
	for (size_t i = 0; i < pause_screen_height; i++)
	{
		cout << pause_screen[i] << endl;
	}
}

void displayLeaderboard()
{
	system("cls");
	char filepath[] = "./Files/leaderboard.txt\0";
	ifstream leaderboard(filepath);

	if (!leaderboard.is_open())
	{
		cout << "Error while trying to load leaderboard.";
		cout << endl << "Enter any input to go back to the Main Menu\n";
		waitUserInput();
		return;
	}

	char line[222];
	while (!leaderboard.eof())
	{
		leaderboard.getline(line, 222);
		cout << line << endl;
	}
	leaderboard.close();

	cout << endl << "Enter any input to go back to the Main Menu\n";

	waitUserInput();

}

void switchMapElements(Point cell1, Point cell2, bool clearCell2)
{
	if (clearCell2) {
		map[cell2.y][cell2.x] = ' ';
	}

	map[cell1.y][cell1.x] += map[cell2.y][cell2.x];
	map[cell2.y][cell2.x] = map[cell1.y][cell1.x] - map[cell2.y][cell2.x];
	map[cell1.y][cell1.x] = map[cell1.y][cell1.x] - map[cell2.y][cell2.x];
}

void teleportPlayer(Point& teleporter_location);

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

			//Read portal data and change the player location accordingly
			teleportPlayer(new_position);

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

void teleportPlayer(Point& teleporter_location)
{
	size_t index = 0;
	for (; index < portal_data_size*2; index += 2)
	{
		if ((portal_data[index].x == teleporter_location.x) && (portal_data[index].y == teleporter_location.y))
		{
			changePlayerPosition(portal_data[index + 1].x - player.position.x, portal_data[index + 1].y - player.position.y);
			return;
		}
	}
	//Do something that lets the developer know that something went wrong
}

//Returns
//0 - Resume
//1 - Exit to Main Menu
//2 - Exit Game
int pauseMenu()
{
	char pause_input;
	const size_t max_message_index = 200;
	char pause_message[max_message_index] = "\0";
	char input;
	int return_status = 0;
	do
	{
		system("cls");
		displayPauseMenu();
		cout << endl << pause_message << endl << endl;
		cout << "Enter input: ";
		cin >> input;
		switch (input)
		{
			case '1':
				return_status = 0;
				break;
			case '2':
				//Run save function - saveProgress(pause_message);
				rewriteCharArray(pause_message, "Save functionality is still under construction\0", max_message_index);
				break;
			case '3':
				//Display warning
				return_status = 1;
				input = '1';
				break;
			case '4':
				//Display warning
				return_status = 2;
				input = '1';
				break;
			default:
				//Change message
				rewriteCharArray(pause_message, "Invalid input. Try again: \0", max_message_index);
				break;

		}
	} while (input != '1');
	return return_status;
}

//Returns
//0 - Everything went fine
//1 - Something went wrong
//2 - Invalid input
//3 - Main Menu
//4 - End Game
//5 - Exit Game
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
			switch (pauseMenu())
			{
			case 0:
				//Do nothing
				break;
			case 1:
				//Pass Main Menu signal
				return 3;
				break;
			case 2:
				//Pass Exit Game signal
				return 5;
				break;
			default:
				return 1;
				break;
			}
			break;
		default:
			return 2;
			break;
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
			if (line[j] == '\0')
			{
				if ((!calculating_y || !point_1_finished))
				{
					return 1;
				}
				break;
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
				continue;
			}
			if (point_1_finished)
			{
				if (calculating_y)
				{
					portal_destination.y *= 10;
					portal_destination.y += line[j] - '0';
					continue;
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
					continue;
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

void extractLevelName(char* name, char* path, size_t max_name_size)
{
	size_t index_path = 0;
	size_t index_name = 0;
	while (path[index_path] != '\0' && (path[index_path] != '.' || index_path==0))
	{
		if (path[index_path] == '/' || path[index_path] == '\\')
		{
			index_path++;
			index_name = 0;
			continue;
		}
		if (index_name == max_name_size - 2)
		{
			index_path++;
			continue;
		}
		name[index_name] = path[index_path];
		index_name++;
		index_path++;
	}
	name[index_name] = '\0';
}

//Returns
//0 - End of level
//1 - Return to Main Menu
//2 - Close the game entirely
int levelCycle(char filepath[])
{
	//Try to read map
	if (readMapData(filepath) == 1)
	{
		//TODO: Change to a screen message
		cout << "Something went wrong while trying to open the Map Data file";
		return 1;
	}

	//Define level name
	size_t level_name_max_size = 100;
	char* level_name = new char[level_name_max_size];
	extractLevelName(level_name,filepath, level_name_max_size);
	char input ;
	const size_t max_message_index = 200;
	char message[max_message_index] = "\0";
	while (true)
	{
		//Display level, player and map data
		cout << "Level: " << level_name << endl;
		//displayPlayerInfo();
		displayMap();

		//Print message
		cout << "\n" << message;
		cout << "\n\n\nEnter your input: ";
		cin >> input;
		switch (step(input))
		{
			case 1:
				cout << "Something major has gone wrong";
				break;
			case 2:
				rewriteCharArray(message, "Invalid input. Try again: \0", max_message_index);
				break;
			case 3:
				freeMapData();
				freePortalData();
				delete(level_name);
				return 1;
				break;
			case 4:
				freeMapData();
				freePortalData();
				delete(level_name);
				return 0;
				break;
			case 5:
				freeMapData();
				freePortalData();
				delete(level_name);
				return 2;
				break;
			default:
				break;
		}
		system("cls");
	}
}


//Returns
//0 - Run Level
//1 - Log out
//2 - Exit Game
int mainMenuCycle(char* filepath)
{
	char input;
	const size_t max_message_index = 200;
	char message[max_message_index] = "\0";
	while (true)
	{
		system("cls");
		displayMainMenu();
		cout << "\n" << message;
		rewriteCharArray(message, "\0");
		cout << "\n\n\nEnter your input: ";
		cin >> input;
		switch (input)
		{
		case '1':
			//New Game
			user.save_file_used = false;
			rewriteCharArray(filepath, "./Files/Maps/test_map.txt\0");
			return 0;
			break;
		case '2':
			//Continue;
			if (user.save_file_used)
			{
				//if(loadSaveFile(filepath) == 0)
				//{
				//		return 0;
				//}	
				rewriteCharArray(message, "Error: Couldn't currently read save data\0", max_message_index);
				waitUserInput();
				break;
			}
			rewriteCharArray(message, "No save data associated to this user was found\0", max_message_index);
			break;
		case '3':
			//Level Select
			//if(levelSelectCycle(filepath) == 0)
			//{
			//		return 0;
			//}
			break;
		case '4':
			//Leaderboard;
			displayLeaderboard();
			break;
		case '5':
			//Delete;
			//removeUser();
			//return 1;
			break;
		case '6':
			//Log out
			return 1;
			break;
		case '7':
			//Exit the game
			return 2;
			break;
		default:
			rewriteCharArray(message, "Invalid input. Try again: \0", max_message_index);
			break;
		}
	}
}

int main()
{
	char* file = new char[200];
	rewriteCharArray(file, "./Files/Maps/test_map.txt\0");
	//cout << file
	bool ignore_main_menu = false;
	while (true)
	{
		if (!ignore_main_menu)
		{
			switch (mainMenuCycle(file))
			{
			case 0:
				break;
			case 1:
				//Return to login cycle
				return 0;
				break;
			case 2:
				return 0;
				break;
			default:
				system("clr");
				cout << "Something major happened in Main Menu Cycle\0Ending program";
				waitUserInput();
				return 0;
			}
		}
		system("cls");
		switch (levelCycle(file))
		{
		case 0:
			system("cls");
			cout << "Sub level finished";
			waitUserInput();
			ignore_main_menu = true;
			//Change the filepath and load the next sub-level
			break;
		case 1:
			system("cls");
			ignore_main_menu = false;
			break;
		case 2:
			return 0;
			//Exit the game
			break;
		}
	}
	return 0;
}