#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
using namespace std;

#pragma region struct Definitions

struct User {
	bool new_user = true;
	char ID[51] = "Undefined\0";
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
	char played_sub_levels[3] = { 0,0,0 };
	int played_sub_levels_count = 0;
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
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '4', ']', ' ', 'D', 'e', 'l', 'e', 't', 'e', ' ', 'u', 's', 'e', 'r', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '[', '5', ']', ' ', 'L', 'o', 'g', ' ', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
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
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', ' ', ' ', ' ', 'L', 'e', 'v', 'e', 'l', ' ', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', 'L', 'e', 'v', 'e', 'l', ' ', '2', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
			{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'L', 'e', 'v', 'e', 'l', ' ', '3', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
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

int levelIndex = -1;//Used from save files

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

#pragma region stringContains function (3 overloads)

bool stringContains(char* arr, char a)
{
	size_t index = 0;
	while (arr[index] != '\0')
	{
		if (arr[index] == a)
		{
			return true;
		}
		index++;
	}
	return false;
}

bool stringContains(char* arr, char a[])
{
	size_t index = 0;
	while (a[index] != '\0')
	{
		if (!stringContains(arr, a[index]))
		{
			return false;
		}
		index++;
	}
	return true;
}

bool stringContains(char* arr, const char a[])
{
	size_t index = 0;
	while (a[index] != '\0')
	{
		if (!stringContains(arr, a[index]))
		{
			return false;
		}
		index++;
	}
	return true;
}

#pragma endregion

#pragma region stringEquals function (2 overloads)

bool stringEquals(char* arr, const char string[])
{
	size_t index = 0;
	while (arr[index] != '\0')
	{
		if (arr[index] != string[index])
		{
			return false;
		}
		index++;
	}
	if (string[index] != '\0')
	{
		return false;
	}
	return true;
}

bool stringEquals(char* arr, char string[])
{
	size_t index = 0;
	while (arr[index] != '\0')
	{
		if (arr[index] != string[index])
		{
			return false;
		}
		index++;
	}
	if (string[index] != '\0')
	{
		return false;
	}
	return true;
}

#pragma endregion

long long fromString(char* arr)
{
	size_t index = 0;
	long long result = 0;
	while (arr[index] >= '0' && arr[index] <= '9' && arr[index] != '\0')
	{
		result *= 10;
		result += arr[index] - '0';
		index++;
	}
	return result;
}

size_t stringLength(const char* arr)
{
	size_t result = 0;
	while (arr[result] != '\0')
	{
		result++;
	}
	return result;
}
size_t stringLength(char* arr)
{
	size_t result = 0;
	while (arr[result] != '\0')
	{
		result++;
	}
	return result;
}

void combineCharArray(const char* one,const char* two, char* result)
{
	//result = new char[stringLength(one) + stringLength(two)];
	size_t index_result = 0;
	size_t index = 0;
	while (one[index] != '\0')
	{
		result[index_result] = one[index];
		index_result++;
		index++;
	}
	index = 0;
	while (two[index] != '\0')
	{
		result[index_result] = two[index];
		index_result++;
		index++;
	}
	result[index_result] = '\0';
}

void combineCharArray(char* one, const char* two, char* result)
{
	//result = new char[stringLength(one) + stringLength(two)];
	size_t index_result = 0;
	size_t index = 0;
	while (one[index] != '\0')
	{
		result[index_result] = one[index];
		index_result++;
		index++;
	}
	index = 0;
	while (two[index] != '\0')
	{
		result[index_result] = two[index];
		index_result++;
		index++;
	}
	result[index_result] = '\0';
}

void combineCharArray(const char* one, char* two, char* result)
{
	//result = new char[stringLength(one) + stringLength(two)];
	size_t index_result = 0;
	size_t index = 0;
	while (one[index] != '\0')
	{
		result[index_result] = one[index];
		index_result++;
		index++;
	}
	index = 0;
	while (two[index] != '\0')
	{
		result[index_result] = two[index];
		index_result++;
		index++;
	}
	result[index_result] = '\0';
}

void combineCharArray(char* one, char* two, char* result)
{
	//result = new char[stringLength(one) + stringLength(two)];
	size_t index_result = 0;
	size_t index = 0;
	while (one[index] != '\0')
	{
		result[index_result] = one[index];
		index_result++;
		index++;
	}
	index = 0;
	while (two[index] != '\0')
	{
		result[index_result] = two[index];
		index_result++;
		index++;
	}
	result[index_result] = '\0';
}

void waitUserInput()
{
	char temp;
	cin >> temp;
	cin.ignore();
}

#pragma region Display functions

void displayLevelSelect()
{
	for (size_t i = 0; i < level_select_screen_height; i++)
	{
		cout << level_select_screen[i] << endl;
	}
}

void displayMessage(const char* arr)
{
	system("cls");
	cout << arr << endl;
	waitUserInput();
}

//Returns
//1
//2
int displayPrompt(const char* arr)
{
	const size_t max_index = 200;
	char message[max_index] = "\0";
	char input;
	while (true)
	{
		system("cls");
		cout << arr << endl << endl << message << endl << "Enter input: ";
		cin >> input;
		switch (input)
		{
		case '1':
			return 1;
			break;
		case '2':
			return 2;
			break;
		default:
			rewriteCharArray(message, "Invalid input. Try again: \n Hint: 1 or 2\n\0", max_index);
			break;
		}
	}
}

void displayPlayerStats()
{
	cout << "Lives: ";
	for (size_t i = 0; i < player.lives; i++)
	{
		cout << "<3 ";
	}
	cout << endl << "Coins: " << player.coins << endl << "Key: ";
	if (player.key)
	{
		cout << "Found";
	}
	else
	{
		cout << "Not found";
	}
	cout << endl;
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

//void displayLeaderboard()
//{
//	system("cls");
//	char filepath[] = "./Files/leaderboard.txt\0";
//	ifstream leaderboard(filepath);
//
//	if (!leaderboard.is_open())
//	{
//		cout << "Error while trying to load leaderboard.\n\n";
//		cout << endl << "Enter any input to go back to the Main Menu\n";
//		waitUserInput();
//		return;
//	}
//
//	char line[222];
//	while (!leaderboard.eof())
//	{
//		leaderboard.getline(line, 222);
//		cout << line << endl;
//	}
//	leaderboard.close();
//
//	cout << endl << "Enter any input to go back to the Main Menu\n";
//
//	waitUserInput();
//
//}

#pragma endregion

#pragma region User functions



void deleteUser()
{

	ifstream reader("./Files/users.txt");
	size_t current_row_count = 0;
	while (!reader.eof())
	{
		reader.ignore(51,'\n');
		current_row_count++;
	}
	reader.close();
	reader.open("./Files/users.txt",ios::in);
	char** lines = new char* [current_row_count];
	size_t index = 0;
	while (!reader.eof())
	{
		lines[index] = new char[51];
		reader.getline(lines[index], 51);
		if (stringEquals(lines[index], user.ID))
		{
			reader.ignore(51, '\n');
			reader.ignore(51, '\n');
			reader.ignore(51, '\n');
			reader.ignore(51, '\n');
			current_row_count -= 5;
			delete(lines[index]);
			continue;
		}
		index++;
	}
	reader.close();
	index = 0;
	fstream users;
	users.open("./Files/users.txt", ios::out);
	/*if (!users)
	{
		return 1;
	}*/
	for (; index < current_row_count; index++)
	{
		users << (index == 0 ? "" : "\n") << lines[index];
	}
	users.close();
}

void logOffUser()
{
	user.coins = 0;
	user.last_unlocked_level = 0;
	user.new_user = true;
	user.save_file_created = false;
	user.save_file_used = false;
}

//Returns
//0 - Everything is OK
//1 - something went wrong
int appendUser(char* ID,size_t current_row_count)
{
	//Get the rows from file
	ifstream reader("./Files/users.txt");
	char** lines = new char*[current_row_count];
	size_t index = 0;
	while (!reader.eof())
	{
		lines[index] = new char[51];
		reader.getline(lines[index], 51);
		index++;
	}
	reader.close();

	//Erase and rewrite it
	index = 0;
	fstream users;
	users.open("./Files/users.txt", ios::out);
	if (!users)
	{
		return 1;
	}
	for (; index < current_row_count; index++)
	{
		users << (index == 0 ? "" : "\n") << lines[index];
	}

	//Add the additional user data
	users << endl << ID << endl << user.coins << endl << user.last_unlocked_level << endl << (user.save_file_created ? "true" : "false") << endl << (user.save_file_used ? "true" : "false");
	users.close();
	index = 0;
	while (ID[index] != '\0')
	{
		user.ID[index] = ID[index];
		index++;
	}
	user.ID[index] = '\0';
	return 0;
}

//Returns
//0 - Everything is OK
//1 - something went wrong
int logInUser(char* ID)
{
	ifstream users("./Files/users.txt");
	if (!users.is_open())
	{
		return 1;
	}

	size_t line_size = 51;
	char line[51];
	bool user_found = false;
	size_t row_count = 0;
	while (!users.eof())
	{
		users.getline(line, line_size);
		row_count++;
		if (stringEquals(line, ID))
		{
			//Copy the data
			users.getline(line, line_size);
			user.coins = fromString(line);
			users.getline(line, line_size);
			user.last_unlocked_level = line[0] - '0';
			users.getline(line, line_size);
			user.save_file_created = stringEquals(line, "true\0");
			users.getline(line, line_size);
			user.save_file_used = stringEquals(line, "true\0");
			user.new_user = false;
			size_t index = 0;
			while (ID[index] != '\0')
			{
				user.ID[index] = ID[index];
				index++;
			}
			user.ID[index] = '\0';
			user_found = true;
			break;
		}
	}
	users.close();
	if (!user_found)
	{
		user.coins = 0;
		user.last_unlocked_level = 0;
		user.save_file_created = false;
		user.save_file_used = false;
		if (appendUser(ID, row_count) != 0)
		{
			return 1;
		}
		logOffUser();
	}
	return 0;
}

void saveUserData()
{
	deleteUser();
	ifstream reader("./Files/users.txt");
	size_t current_row_count = 0;
	while (!reader.eof())
	{
		reader.ignore(51, '\n');
		current_row_count++;
	}
	reader.close();
	appendUser(user.ID, current_row_count);
}

#pragma endregion


int saveProgress(char* level_name)
{
	char* filepath = new char[stringLength("./Files/Saves/\0") + stringLength(user.ID) + stringLength("_map.txt\0")];
	char* temporaryFilepath = new char[stringLength("./Files/Saves/\0") + stringLength(user.ID)];
	combineCharArray("./Files/Saves/\0", user.ID, temporaryFilepath);
	combineCharArray(temporaryFilepath, "_map.txt\0", filepath);
	fstream mapData;
	mapData.open(filepath, ios::out);
	if (!mapData)
	{
		return 1;
	}
	mapData << map_width << "x" << map_height;
	for (size_t i = 0; i < map_height; i++)
	{
		mapData << endl << map[i];
	}
	mapData << endl << portal_data_size << endl;
	for (size_t i = 0; i < portal_data_size; i += 2)
	{ 
		mapData << portal_data[i].x << "x" << portal_data[i].y;
		mapData << "|" << portal_data[i + 1].x << "x" << portal_data[i + 1].y << endl;
	}
	mapData.close();
	combineCharArray(temporaryFilepath, "_data.txt\0", filepath);
	fstream playerData;
	playerData.open(filepath, ios::out);
	if (!playerData)
	{
		return 1;
	}
	playerData << player.coins << endl << player.lives << endl << (player.key ? "true" : "false") << endl << level_name[6] << endl << level_name[10];
	playerData.close();
	user.save_file_created = true;
	user.save_file_used = true;
	saveUserData();
	return 0;
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

//Returns
//0 - Everything is alright
//1 - End level
int changePlayerPosition(long long x, long long y)
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
				return 1;
			}
			else {
				//Display message that you don't have the key
				displayMessage("Treasure chest is locked. Find the key.\nHint: &\n\0");
			}

			break;
		default:
			switchMapElements(player.position, new_position, false);
			player.position = new_position;
			break;
	}
	return 0;
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
//0 - Verification successful
//1 - Verification failed
int verifyValidMapDimentions(char line[], size_t line_size)
{
	bool reading_height_data = false;
	for (size_t i = 0; i < line_size; i++)
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
		map[i] = new char[map_width + 1];
	}
	for (size_t i = 0; i < map_height; i++) {
		file.getline(map[i], map_width + 1);
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
	for (size_t i = 0; i < portal_data_size * 2; i += 2)
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
	while (path[index_path] != '\0' && (path[index_path] != '.' || index_path == 0))
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

int continueGame(char* filepath)
{
	char* temporaryFilepath1 = new char[stringLength("./Files/Saves/\0") + stringLength(user.ID)];
	combineCharArray("./Files/Saves/\0", user.ID, temporaryFilepath1);
	combineCharArray(temporaryFilepath1, "_map.txt\0", filepath);
	char* temporaryFilepath2 = new char[stringLength(temporaryFilepath1) + stringLength("_data.txt\0") + stringLength(user.ID)];
	combineCharArray("./Files/Saves/\0", user.ID, temporaryFilepath1);
	combineCharArray(temporaryFilepath1, "_data.txt\0", temporaryFilepath2);
	ifstream playerData(temporaryFilepath2);
	if (!playerData.is_open())
	{
		return 1;//Something went wrong
	}
	size_t line_size = 51;
	char line[51];
	bool user_found = false;
	size_t row_count = 0;
	playerData.getline(line, line_size);
	player.coins = fromString(line);
	playerData.getline(line, line_size);
	player.lives = fromString(line);
	playerData.getline(line, line_size);
	player.key = stringEquals(line, "true");
	playerData.getline(line, line_size);
	levelIndex = fromString(line);
	playerData.close();
	return 0;
}

int loadNextSublevel(char* filepath)
{
	if (levelIndex != -1)
	{
		char levelIndexStr = ('0' + levelIndex);
		rewriteCharArray(filepath, "./Files/Maps/0/Level 0 - 0.txt\0");
		filepath[13] = levelIndexStr;
		filepath[21] = levelIndexStr;
	}

	if (player.played_sub_levels_count == 3)
	{
		player.played_sub_levels[0] = 0;
		player.played_sub_levels[1] = 0;
		player.played_sub_levels[2] = 0;
		player.played_sub_levels_count = 0;
		return 1;
	}
	int randomNumber;
	int counter = 0;
	bool repeated = false;
	while (true)
	{
		repeated = false;
		randomNumber = (rand() % 3) + 1; 
		
		for (; counter < player.played_sub_levels_count; counter++)
		{
			if (player.played_sub_levels[counter] == randomNumber)
			{
				repeated = true;
				break;
			}
		}
		if (!repeated)
		{
			filepath[25] = randomNumber + '0';
			player.played_sub_levels_count++;
			break;
		}
	}
	return 0;
}

#pragma region Menus, and cycles - pauseMenu, step, levelCycle, mainMenuCycle, loginCycle

//Returns
//0 - Resume
//1 - Exit to Main Menu
//2 - Exit Game
int pauseMenu(char* level_name)
{
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
			/*if (saveProgress(level_name) == 0)
			{
				rewriteCharArray(pause_message, "Progress saved\0", max_message_index);
			}
			else
			{
				rewriteCharArray(pause_message, "Progress couldn't be saved\0", max_message_index);
			}*/
			rewriteCharArray(pause_message, "Feature is under development\0", max_message_index);
			break;
		case '3':
			if (displayPrompt("Warning!\nExiting to the Main Menu may cause your progress to be lost!\nMake sure you have saved your progress before continuing.\n\n[1] To Main Menu\t\t[2] Go back\n\0") == 1) {
				return_status = 1;
				input = '1';
			}
			break;
		case '4':
			if (displayPrompt("Warning!\nExiting the game may cause your progress to be lost!\nMake sure you have saved your progress before continuing.\n\n[1] Exit\t\t[2] Go back\n\0") == 1) {
				return_status = 2;
				input = '1';
			}
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
int step(char input,char*level_name)
{
	switch (input)
	{
	case 'w':
	case 'W':
		if (changePlayerPosition(0, -1) == 1)
		{
			return 4;
		}
		break;
	case 's':
	case 'S':
		if (changePlayerPosition(0, 1) == 1)
		{
			return 4;
		}
		break;
	case 'a':
	case 'A':
		if (changePlayerPosition(-1, 0) == 1)
		{
			return 4;
		}
		break;
	case 'd':
	case 'D':
		if (changePlayerPosition(1, 0) == 1)
		{
			return 4;
		}
		break;
	case 'p':
	case 'P':
		switch (pauseMenu(level_name))
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
//0 - End of level
//1 - Return to Main Menu
//2 - Close the game entirely
//3 - Restart level
int levelCycle(char filepath[])
{
	//Try to read map
	if (readMapData(filepath) == 1)
	{
		displayMessage("Something went wrong");
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
		displayPlayerStats();
		displayMap();

		//Print message
		cout << "\n" << message;
		cout << "\n\n\nEnter your input: ";
		cin >> input;
		switch (step(input,level_name))
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
				player.lives = 3;
				player.coins = 0;
				player.key = false;
				delete(level_name);
				return 1;
				break;
			case 4:
				freeMapData();
				freePortalData();
				delete(level_name);
				user.coins += player.coins;
				player.lives = 3;
				player.coins = 0;
				player.key = false;
				//Update leaderboard
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
		if (player.lives == 0)
		{
			if (displayPrompt("You have died!\n\n[1] Restart the level\t\t[2] To Main Menu") == 1)
			{
				freeMapData();
				freePortalData();
				delete(level_name);
				player.lives = 3;
				player.coins = 0;
				player.key = false;
				return 3;
			}
			else
			{
				freeMapData();
				freePortalData();
				player.lives = 3;
				player.coins = 0;
				player.key = false;
				delete(level_name);
				return 1;
			}
		}
		system("cls");
	}
}

void startNewGame(char* filepath)
{
	user.save_file_used = false;
	char sub_level_index = ((int)((rand() % 3) + 1)) + '0';
	rewriteCharArray(filepath, "./Files/Maps/1/Level 1 - 0.txt\0");
	filepath[25] = sub_level_index;
	player.played_sub_levels_count++;
	player.played_sub_levels[0] = sub_level_index - '0';
}

int levelSelectCycle(char* filepath)
{
	char input;
	const size_t max_message_index = 200;
	char message[max_message_index] = "\0";
	while (true)
	{
		system("cls");
		displayLevelSelect();
		cout << endl << message << endl << endl << "Enter input: ";
		cin >> input;
		switch (input)
		{
		case '1':
			rewriteCharArray(filepath, "./Files/Maps/1/Level 1 - 0.txt\0");
			return 0;
		case '2':
			rewriteCharArray(filepath, "./Files/Maps/2/Level 2 - 0.txt\0");
			return 0;
		case '3':
			rewriteCharArray(filepath, "./Files/Maps/3/Level 3 - 0.txt\0");
			return 0;
		default:
			rewriteCharArray(message, "Invalid input. Try again: \0", max_message_index);
			break;
		}
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
			startNewGame(filepath);
			return 0;
			break;
		case '2':
			//Continue;
			/*if (user.save_file_used)
			{
				if(continueGame(filepath) == 0)
				{
						return 0;
				}	
				rewriteCharArray(message, "Error: Couldn't currently read save data\0", max_message_index);
				waitUserInput();
				break;
			}*/
			rewriteCharArray(message, "This feature is under development\0", max_message_index);
			break;
		case '3':
			//Level Select
			if(levelSelectCycle(filepath) == 0)
			{
				if (loadNextSublevel(filepath) == 0) {
					return 0;
				}
				displayMessage("Sub level couldn't be loaded. \nEnter anything to exit the program.\0");
				return 2;
			}
			break;
		case '4':
			//Delete;
			deleteUser();
			logOffUser();
			return 1;
			break;
		case '5':
			//Log out
			return 1;
			break;
		case '6':
			//Exit the game
			return 2;
			break;
		default:
			rewriteCharArray(message, "Invalid input. Try again: \0", max_message_index);
			break;
		}
	}
}

//Returns
//0 - Everything is OK
//1 - Something went wrong
int loginCycle()
{
	char ID[51] = "\0";
	while (true)
	{
		system("cls");
		cout << "Hello user!\n\nPlease enter your ID\n\nIf you don't have a profile, one will be automatically generated\n\nID: ";
		cin.getline(ID,50);
		ID[50] = '\0';
		if (!stringContains(ID, "<>:\"/\\|?*\0"))
		{
			if (logInUser(ID) != 0)
			{
				return 1;
			}
			return 0;
		}
		cin.ignore();
		displayMessage("Invalid ID was entered.\nPlease try again.\nType any key and press Enter to try again\0");
	}
}


int mainMenuSwitch(char* filepath, bool& user_logged_in)
{
	switch (mainMenuCycle(filepath))
	{
	case 0:
		break;
	case 1:
		user_logged_in = false;
		logOffUser();
		cin.ignore();
		break;
	case 2:
		return 0;
		break;
	default:
		system("clr");
		cout << "Something major happened in Main Menu Cycle\0Ending program";
		return 0;
	}
	return 1;
}

int levelSwitch(char* filepath, bool& ignore_main_menu)
{
	switch (levelCycle(filepath))
	{
	case 0:
		system("cls");
		cout << "Sub level finished";
		waitUserInput();
		ignore_main_menu = true;
		switch (loadNextSublevel(filepath))
		{
		case 0:
			//Load the level
			return 1;
			break;
		case 1:
			//The level was completed
			displayMessage("Congratulations! You have cleared the level!");
			levelIndex = -1;
			user.last_unlocked_level++;
			//saveUserData();
			ignore_main_menu = false;
			return 1;
		default:
			displayMessage("Sub level couldn't be loaded.\n Enter anything to stop the program\0");
			return 0;
			break;
		}
		break;
	case 1:
		//Proceed to next level
		system("cls");
		ignore_main_menu = false;
		break;
	case 2:
		//Exit game
		return 0;
		break;
	case 3:
		//Return to main menu cycle
		system("cls");
		ignore_main_menu = true;
		break;
	}
	return 1;
}

#pragma endregion



int main()
{
	srand(time(0));
	char* file = new char[200];
	bool ignore_main_menu = false;
	bool user_logged_in = false;
	while (true)
	{
		if (!user_logged_in)
		{
			if (loginCycle() == 1)
			{
				displayMessage("Login Failed");
				return 0;
			}
			user_logged_in = true;
		}
		if (!ignore_main_menu)
		{
			if (mainMenuSwitch(file, user_logged_in) == 0) {
				return 0;
			}
			if (!user_logged_in)
			{
				continue;
			}
		}
		system("cls");
		if (levelSwitch(file, ignore_main_menu) == 0) {
			return 0;
		}
	}
	return 0;
}