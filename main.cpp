#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using std::cout;
using std::cin;

//  functions
void displayIntro();
void displayRoundStats(int pHP, int eHP, std::string inv[], int size);
int generateDamage(int baseDMG, int rngMax);
void enemyLootDrop(int& playerXP, std::string inv[], int size);
void displayPlayerStats(int pHP, int pDMG);
void displayEnemyStats(int eHP, int eDMG);
void showInventory(std::string inv[10], int size);
void displayStoryIntro(std::string name);
void enchantingTable(int playerHP, int& playerXP, int swordDurability, bool& hasInfinity, char& gameChoice);

int main() {
	srand(static_cast<unsigned int>(time(0))); // fixes C4244 with static_cast

	// Stats
	int playerHP = 100;
	int playerDMG = 20;
	int enemyHP = 120;
	int enemyDMG = 15;

	int maxBaseHP = 100;
	int playerXP = 0;
	bool hasInfinity = false;
	int swordDurability = 55;
	// user name
	std::string playerName = "Hero";
	cout << "======================================================\n";
	cout << "Welcome, travaler.Enter your character`s name: ";
	cin >> playerName;

	// cinematic function for story
	displayStoryIntro(playerName);
	// battle choices
	std::string action = "0";

	char gameChoice = 'y'; // for game loop
	// declaring an inventory
	std::string inventory[10] = { "Basic Sword","Empty","Empty" , "Empty","Empty","Empty","Empty","Empty","Empty","Empty" };
	int invSize = sizeof(inventory) / sizeof(inventory[0]); // used for better inventory coding

	// game loop
	while (gameChoice == 'y' || gameChoice == 'Y') {
		system("cls"); // new enemy new clean window
		// resets enemy`s hp for the new battle
		int enemyHP = 120;
		// displays intro
		displayIntro();
		// displays player stats
		displayPlayerStats(maxBaseHP, playerDMG);
		// adds 2 healing pots in the first empty slots
		int potionsAdded = 0;
		for (int i = 0; i < invSize; i++) {
			if (inventory[i] == "Empty" && potionsAdded < 2) {
				inventory[i] = "Healing potion";
				potionsAdded++;
			}
		}
		cout << "--------------------------------------------------\n";
		cout << "Bonus items added: " << potionsAdded << " healing potions\n";
		// adds bonus hp and overheal limit
		int overhealLimit = maxBaseHP * 3;
		if (playerHP < overhealLimit && playerHP != maxBaseHP) {
			playerHP = (playerHP / 3) + playerHP;
			if (playerHP > overhealLimit) {
				playerHP = overhealLimit; // stops hp from exceding overheallimit
				cout << "[SYSTEM ALERT]: MAX OVERHEAL! Your HP reached the absolute limit of " << overhealLimit << "!\n";
			}

			cout << "Bonus HP added: " << playerHP / 2 << '\n';
			cout << "--------------------------------------------------\n";
		}
		cout << "An enemy apeared!\n";
		cout << "--------------------------------------------------\n";
		displayEnemyStats(enemyHP, enemyDMG); // displays enemy stats, after enemy apears

		// while loop, the battle
		while (playerHP > 0 && enemyHP > 0) {
			// checks # of pots in inv before displaying
			int currentPotionsCount = 0;
			for (int i = 0; i < invSize; i++) {
				if (inventory[i] == "Healing potion" || inventory[i] == "Healing potion") {
					currentPotionsCount++;
				}
			}

			displayRoundStats(playerHP, enemyHP, inventory, invSize);

			// your choices
			cout << "\nChoose your action:\n";
			cout << "1. Attack (20 DMG + 10 random bonus)\n";
			cout << "2. Heal (+40 HP)\n";
			cout << "3. Check Inventory (turn doesn`t get wasted)\n";
			cout << "--------------------------------------------------\n";
			cout << "Your choice (1-3): ";
			cin >> action;
			cout << "--------------------------------------------------\n";

			if (action == "1") {
				// Your turn
				bool hasSword = false;
				// checks inventory to see if player has more swords
				for (int i = 0; i < invSize; i++) {
					if (inventory[i] == "Basic Sword") {
						hasSword = true;
						break;
					}
				}
				if (hasSword) {
					int pDmgDealt = generateDamage(playerDMG, 10);
					enemyHP -= pDmgDealt;
					if (!hasInfinity) {
						swordDurability--;
					}
					cout << "\n You hit the enemy with " << pDmgDealt << " DMG!\n";
					cout << "(Sword Duarability: " << swordDurability << "/55)\n";
					// checks if sword durability is 0 if so destory the sword
					if (swordDurability <= 0) {
						// checks inv and deletes sword
						for (int i = 0; i < invSize; i++) {
							if (inventory[i] == "Basic Sword") {
								inventory[i] = "Empty";
								break; // deletes only 1 sword
							}
						}
						playerDMG = 5; // pDMG decreases
						cout << "[SYSTEM ALERT]: BROKEN WEAPON: Your Basic Sword shattered! Base DMG reduced to 5!\n";
						cout << "----------------------------------------------------------------------------------\n";
					}
				}
				else {
					// if no sword in inv u use punches to fight (5 dmg)
					int punchDmg = generateDamage(5, 2);
					enemyHP -= punchDmg;
					cout << "No weapon equipped! you punch the enemy for " << punchDmg << " DMG!\n";
				}
			}
			else if (action == "2") {
				bool potionFound = false;
				//searches inventory from slot 0-9 for potion
				for (int i = 0; 1 < invSize; i++) {
					if (inventory[i] == "Healing potion" || inventory[i] == "Healing potion") {
						playerHP += 40;
						inventory[i] = "Empty"; // deletes healing pot after use and it becomes empty slot
						potionFound = true; // found and used potion with succes
						int overhealLimit = maxBaseHP * 3;
						if (playerHP > overhealLimit) {
							playerHP = overhealLimit;
						}
						system("cls");
						cout << "\n You drank a potion from slot #" << (i + 1) << " and restored 40 HP!\n";
						if (playerHP == overhealLimit) {
							cout << "[SYSTEM ALERT]: MAX OVERHEAL! Your HP reached the absolute limit of " << overhealLimit << "!\n";
						}
						displayRoundStats(playerHP, enemyHP, inventory, invSize);
						break; // stops for loop, to not drink all pots
					}
				}
				// if there`s no healing pot
				if (!potionFound) {
					cout << "\n You don`t have any potions left in your inventory!\n";
					cout << "Turn wasted!\n";
					cout << "--------------------------------------------------\n";
				}
			}
			else if (action == "3") {
				system("cls"); // only shows inventory
				showInventory(inventory, invSize);
				std::string discardChoice = "0";
				cout << "\nEnter the slot number to DISCARD an item (1-10), or 0 to go back to battle: ";
				cin >> discardChoice;
				if (discardChoice == "1" || discardChoice == "2" || discardChoice == "3" || discardChoice == "4" || discardChoice == "5" || discardChoice == "6" || discardChoice == "7" || discardChoice == "8" || discardChoice == "9" || discardChoice == "10") {
					int arrayIndex = 0;
					if (discardChoice == "10") arrayIndex = 9; // manualy converts the string in index of vector (1 becomes index 0, 2 becomes 1 etc.)
					else arrayIndex = discardChoice[0] - '1'; // makes '1' - '9' into numbers from 0 to 8
					if (inventory[arrayIndex] != "Empty") {
						cout << "[DISCARD]: You trew away " << inventory[arrayIndex] << " from Slot #" << discardChoice << "!\n";
						inventory[arrayIndex] = "Empty"; // deletes item
					}
					else {
						cout << "That slot is already empty!\n";
					}
					cout << "--------------------------------------------------\n";
				}
				else if (discardChoice == "0") {
					cout << "Returning to battle...\n";
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					system("cls");
				}
				else {
					cout << "Invalid choice! Turning back to battle...\n";
				}
				continue; // gets u back in the fighting menu without using a turn
			}
			else {
				cout << "\n Invalid choice! You wasted your turn!\n";
			}

			// Verifies if enemy is dead
			if (enemyHP <= 0) {
				cout << "\n Enemy defeated!\n";
				cin.clear();
				cin.ignore(100, '\n');
				cout << "\n[LOOT SYSTEM]: Press Enter to collect your rewards!";
				cin.get();
				enemyLootDrop(playerXP, inventory, invSize);
				bool hasSwordAcquired = false;
				for (int i = 0; i < invSize; i++) {
					if (inventory[i] == "Basic Sword") {
						hasSwordAcquired = true;
						break;
					}
				}
				if (hasSwordAcquired && playerDMG == 5) {
					playerDMG = 20; // resets dmg
					swordDurability = 55; // resets sword durability
					cout << "[WEAPON EQUIPPED]: You equipped a Basic Sword from your inventory! Base DMG restored to 20!\n";
					cout << "----------------------------------------------------------------------------------------------\n";
				}
				cout << "[SYSTEM]: Press 1 to search for new enemy\n";
				cout << "[SYSTEM]: Press 2 to exit game\n";
				cout << "[SYSTEM]: Press 3 to enter enchanting table\n";
				cin >> action;
				if (action == "1") {
					action = 'y' || 'Y';
				}
				else if (action == "2") {
					gameChoice = 'n';
				}
				else if (action == "3") {
					enchantingTable(playerHP, playerXP, swordDurability, hasInfinity, gameChoice);
				}
				else {
					cout << "Invalid choice Try again!\n";
					cin >> action;
				}
				break;
			}

			// enemy turn
			int eDmgDealt = generateDamage(enemyDMG, 5);
			playerHP -= eDmgDealt;
			cout << " Enemy hit you with " << eDmgDealt << " DMG!\n";
			cout << "--------------------------------------------------\n\n\n";
			displayRoundStats(playerHP, enemyHP, inventory, invSize);
			// small break
			cout << "[SYSTEM]: Press Enter to continue to the next round...";
			cin.ignore();
			cin.get();
			system("cls"); // after button pressed, window gets cleaned
		}

		// Verifies if player died
		if (playerHP <= 0) {
			cout << "\n" << playerName << " died... Skill issue.\n";
			cout << "Game over!\n";
			break;
		}
	}

		return 0;
	}

	// defines the functions
	void displayIntro() {
		cout << "========== [Delta Dungeon RPG v0.8.5] ==========\n\n";
	}

	void displayPlayerStats(int pHP, int pDMG) {
		cout << "PLAYER INITIAL STATS: HP: " << pHP << " | BASE DMG: " << pDMG << '\n';
		cout << "--------------------------------------------------\n";
	}

	void displayEnemyStats(int eHP, int eDMG) {
		cout << "ENEMY INITIAL STATS: HP: " << eHP << " | BASE DMG: " << eDMG << '\n';
		cout << "--------------------------------------------------\n";
	}

	void displayRoundStats(int pHP, int eHP, std::string inv[], int size) {
		cout << "\n=== YOUR HP: " << pHP << "  |   ENEMY HP: " << eHP << " ===\n";
		int currentPotionsCount = 0;
		for (int i = 0; i < size; i++) {
			if (inv[i] == "Healing potion" || inv[i] == "Healing potion") {
				currentPotionsCount++;
			}
		}
		cout << "\n=== YOUR POTIONS: " << currentPotionsCount << " ===\n";
		cout << "---------------------------------------\n";
	}

	int generateDamage(int baseDMG, int rngMax) {
		return baseDMG + (rand() % rngMax);
	}

	void enemyLootDrop(int& playerXP, std::string inv[], int size) {
		int roll = (rand() % 100) + 1;
		cout << "--------------------------------------------------\n";
		cout << "[ENEMY DROPPED LOOT]\n";
		cout << "[SYSTEM]: Rolling loot chance...(Result: " << roll << ")\n";
		int xpGained = (rand() % 5) + 5; // gives 5-10 XP
		playerXP += xpGained;
		cout << "--------------------------------------------------\n";
		cout << "Drop: +" << xpGained << " XP! (Total XP now: " << playerXP << ")\n";
		if (roll <= 10) {
			cout << "Drop: [Basic Sword]\n";
			bool itemStored = false;
			for (int i = 0; i < size; i++) {
				if (inv[i] == "Empty") {
					inv[i] = "Basic Sword";
					cout << "[INVENTORY]: Stored Basic Sword in slot #" << (i + 1) << "!\n";
					itemStored = true;
					break;
				}
			}
			if (!itemStored) cout << "[SYSTEM ERROR]: Inventory full! You couldn`t carry the Basic sword!\n";
		}
		else if (roll <= 50) {
			cout << "Drop: [Healing potion]\n";
			bool itemStored = false;
			for (int i = 0; i < size; i++) {
				if (inv[i] == "Empty") {
					inv[i] = "Healing potion";
					cout << "[INVENTORY]: Stored Healing Potion in slot #" << (i + 1) << "!\n";
					itemStored = true;
					break;
				}
			}
			if (!itemStored) cout << "[SYSTEM ERROR]: Inventory full! You couldn`t carry the healing potion!\n";
		}
		else {
			cout << '\n';
		}
		cout << "--------------------------------------------------\n";
		cout << "[SYSTEM]: Processing loot drop and rewards...\n\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	}

	void showInventory(std::string inv[], int size) {
		cout << "\n==========[YOUR INVENTORY]==========\n";
		for (int i = 0; i < size; i++) {
			cout << "Slot #" << (i + 1) << ": " << inv[i] << '\n';
		}
		cout << "--------------------------------------------------\n";
	}

	void displayStoryIntro(std::string name) {
		system("cls"); // cleans the window so only the story is visible
		cout << "[THE LEGEND OF DELTA DUNGEON]\n";
		cout << "--------------------------------------------------\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // cinematic pause
		cout << name << ", a brave warrior...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "From Delta kingdom...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "Delta kingdom got invaded by monsters, but all of it`s warriors were fighting a war\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "Then..." << name << " got lost in the war and he travaled the world\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "He had no idea that he entered Delta kingdom...wich was full of monsters\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "He is armed with a Basic Sword and 2 Healing Potions...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << name << " is trying to survive...that`s the goal...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "[SYSTEM]: DO NOT GO IN THE DELTA DUNGEON!...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "[SYSTEM]: Press any button to begin your journey...";
		cin.ignore();
		cin.get();
		system("cls");
	}

	void enchantingTable(int playerHP, int& playerXP, int swordDurability, bool& hasInfinity, char& gameChoice) {
		// if player alive, asks for new enemey
		if (playerHP > 0) {
			// enchanting table
			system("cls");
			std::string enchantChoice = "0";
			cout << "==========[ENCHANTING TABLE]==========\n";
			cout << "Current XP: " << playerXP << " | Sword Durability: " << swordDurability << "/55\n";
			if (hasInfinity == true) cout << "CURRENT ENCHANTS: [INFINITY]\n";
			else cout << "CURRENT ENCHANTS: None\n";
			cout << "--------------------------------------------------\n";
			cout << "Available options:\n";
			cout << "1. Buy [INFINITY] for Basic Sword (costs 50 XP)\n";
			cout << "2. Skip and move forward\n";
			cout << "Your choice: ";
			cin >> enchantChoice;
			if (enchantChoice == "1") {
				if (hasInfinity == true) {
					cout << "Your weapon aleardy has the Infinity enchant!\n";
					std::this_thread::sleep_for(std::chrono::milliseconds(2500));
					cout << "\nDo you want to search for another enemy? ( y/n ): ";
					cin >> gameChoice;
				}
				else if (playerXP >= 50) {
					playerXP -= 50;
					hasInfinity = true; // activates the enchant
					cout << "[SYSTEM:ENCHANT]: Your Basic Sword was enchanted with INFINITY! It will never break again!\n";
					std::this_thread::sleep_for(std::chrono::milliseconds(2500));
					cout << "\nDo you want to search for another enemy? ( y/n ): ";
					cin >> gameChoice;
				}
				else if (playerXP < 50 && enchantChoice == "1") {
					cout << "\nNot enough XP! You need 50 XP to afford this enchantment.\n";
					cout << "--------------------------------------------------\n";
					std::this_thread::sleep_for(std::chrono::milliseconds(2500));
					cout << "\nDo you want to search for another enemy? ( y/n ): ";
					cin >> gameChoice;
				}
			}

			else if (enchantChoice != "1") {
				cout << "\n Leaving the Enchanting Table...\n";
				cout << "\nDo you want to search for another enemy? ( y/n ): ";
				cin >> gameChoice;
			}
		}
		else {
			gameChoice = 'n'; // stops game if player dies
			cout << "\nTotalXP farmed: " << playerXP << "\n";
		}
	}


	// CHANGE LOG
	/*
	cout << "\n=======================================\n";
	cout << " [CHANGE LOG -Delta Dungeon RPG]\n";
	cout << "v0.0 -> Created basic battle loop mechanics.\n";
	cout << "v0.1 -> Added Initial Stats & delimiters.\n";
	cout << "v0.2 -> REWROTE THE ENTIRE CODE  WITH USER FUNCTIONS (void & return).\n";
	cout << "     -> Added interactive user choice for Attack / Heal.\n";
			 v0.3 -> Added potion limit for healing (only 2 per battle)
				  -> Improved 'UI'
			 v0.4 -> Added enemy loot drop
					drops: basic sword, haeling pots, XP
			v0.4.1-> Added game loop
			v0.4.2-> Added a player hp restore thing [ current hp/2 + playerHP] and for healing pots too
					 Trying fixing bug fixes (2)
			v0.4.3-> improved 'UI' for bonus items
					 improved math for bonus itmes/HP
					 Buffed healing pot [30 heal -> 40 heal] for better long term experience
			v0.4.4->Fixed visual bug by making 2 extra functions
			v0.4.5->Fixed 1 weird bug
			v0.5  -> Added inventory (max 10 slots)
			v0.5.1-> Made healing pots update in the inventory after use
			v0.5.1.1-> Fixed 1 small bug
			v0.5.1.2-> Fixed 1 samll bug
			v0.5.1.3-> Fixed 1 big bug
			v0.5.1.4-> 1 bug fix
			v0.5.1.5-> 1 bug fix
			v0.5.2 -> optimizing code/memory coruption managment
			v0.5.3 -> droped loot from enemy now updates in the inventory
					  health potion drop rate from enemys : 10% -> 40%
			v0.5.4 -> making initial player HP
					  making overheal limit [initial player HP * 3]
			v0.5.4.1 -> changed bonus hp after new enemy (new math: pHP = (pHP/3) + pHP
			v0.5.4.2 -> fixed 1 bug
						fixed 1 visual bug
			v0.6 -> Added durability to basic sword
			v0.6.1 -> Fixed code logic for Basic Sword
			v0.6.2 -> Added a discarding option in inventory tab
			v0.6.2.1 -> Fixed huge bug in inventory tab
			v0.7 -> Added enchanting tab
					added enchant infinity (for swords, gives them infinite druability)
			v0.7.1 -> 4 bug fixes
			v0.8 -> Added user name
					Added story
			v0.8.1 -> Added the user`s name when he dies
			v0.8.2 -> Huge UI improvement
			v0.8.2.1 -> 1 bug fix
			v0.8.3 -> improved encahting table`s UI
			v0.8.4 -> 8 bug fixes
			v0.8.5 -> 8 bug fix
			          changed code and UI
	cout << "=======================================\n";
	*/