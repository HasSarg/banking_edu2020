#include "management_system.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

const std::string blue = "\033[34m";
const std::string green = "\033[32m";
const std::string red = "\033[31m";
const std::string reset = "\033[0m";
const double min = 0;
const double max = std::numeric_limits<double>::max();

ManagementSystem::ManagementSystem()
                 : m_menu_title {"CUSTOMER ACCOUNT BANKING MANAGEMENT SYSTEM\nWELCOME TO THE MAIN MENU\n\n"},
                   m_menu_options {"1. Create new account\n",
                                   "2. Update information of existing account\n",
                                   "3. For transactions\n",
                                   "4. Check the details of existing account\n",
                                   "5. Removing existing account\n",
                                   "6. View customer's list\n",
                                   "7. Exit\n"}
{
    set_accounts_list();
    admin_password = "admin";
}

void ManagementSystem::run()
{
    display_menu();
    make_choice();
    print_line_of('*', 50, green);
    print_by_color("THE APPLICATION HAS BEEN CLOSED\n", green);
}

void ManagementSystem::set_accounts_list()
{
    m_data_file.open("data.json");
    if (m_data_file.is_open()) {
        m_data_file >> m_accounts_list;
        m_data_file.close();
    }
}

void ManagementSystem::display_menu()
{
    print_by_color(m_menu_title, blue);
    for(int i = 0; i < options_qty; ++i) {
        print_by_color(m_menu_options[i], blue);
    }
}

void ManagementSystem::print_by_color(std::string message, std::string color)
{
    std::cout << color << message << reset;
}

void ManagementSystem::print_by_color(int message, std::string color)
{
    std::cout << color << message << reset;
}

void ManagementSystem::make_choice()
{
    int choice;
    print_by_color("Enter your choice: ", green);
    std::cin >> choice;
    while (std::cin.fail()) {
        print_by_color("Error, please enter an integer instead of character/string.\n", red);
        print_by_color("Please try again: ", green);
        std::cin.clear();
        std::cin.ignore(256,'\n');
        std::cin >> choice;
    }
    m_choice = choice;
    start();
}

void ManagementSystem::print_line_of(char ch, int qty, std::string color)
{
    std::cout << color;
    for (int i = 0; i < qty; ++i) {
        std::cout << ch;
    }
    std::cout << reset << std::endl;
}

bool ManagementSystem::action_by_choice(const int& m_choice)
{
    set_accounts_list();
    if(m_choice == 1) {
        create_account();
    } else if (m_choice == 2) {
        if (m_accounts_list.size() > 0) {
            update_account();
        } else {
            print_by_color("NO ACCOUNTS TO UPDATE\n", red);
        }
    } else if (m_choice == 3) {
        if (m_accounts_list.size() > 1) {
            transaction();
        } else {
            print_by_color("THERE AREN'T 2 ACCOUNTS TO MAKE TRANSACTION\n", red);
        }
    } else if (m_choice == 4) {
        print_account();
    } else if (m_choice == 5) {
        remove_account();
    } else if (m_choice == 6) {
        read_list();
    } else if (m_choice == 7) {
        return false;
    }
    return true;
}

void ManagementSystem::start()
{
    print_by_color("You have entered: ", green);
    bool repeat = false;
    if (m_choice > 0 && m_choice < 8) {
        print_by_color(m_menu_options[m_choice - 1],  blue);
        repeat = action_by_choice(m_choice);
    } else {
        print_by_color(" It is out of [1, 7] range.\n" , red);
        repeat = true;
    }
    if(repeat) {
        print_line_of('*', 50, green);
        display_menu();
        make_choice();
    }
}

void ManagementSystem::create_account()
{
    json account;
    print_by_color("To quit this /create account/ option, please enter '-1'.\n\n", green);
    if (!set_name(account) || !set_surname(account) ||
        !set_amount(account) || !set_password(account) ||
        !set_id(account)) {
      print_by_color("You have entered -1 to quit this /create account/ option.\n", red);
      print_by_color("THE ACCOUNT IS NOT CREATED\n", red);
      return;
    }
    m_accounts_list.push_back(account);
    update_file();
    set_accounts_list();
    print_by_color("THE ACCOUNT HAS BEEN CREATED\n", green);
    print_account(account);
}

bool ManagementSystem::set_name(json& account)
{
    std::string name;
    do {
        print_by_color("Please enter name. The name must be at least 2 letters long: ", green);
        std::cin >> name;
        if (name == "-1") {
            return false;
        }
    } while (!valid_input(name, 2));
    account["name"] = name;
    return true;
}

bool ManagementSystem::set_surname(json& account)
{
    std::string surname;
    do {
        print_by_color("Please enter surname. The surname must be at least 2 letters long: ", green);
        std::cin >> surname;
        if (surname == "-1") {
            return false;
        }
    } while (!valid_input(surname, 2));
    account["surname"] = surname;
    return true;
}

bool ManagementSystem::set_amount(json& account)
{
    double input;
    do {
        print_by_color("Please enter amount. The amount should be positive number: ", green);
        std::cin >> input;
        while (std::cin.fail()) {
            print_by_color("Error, please enter an integer instead of character/string: ", red);
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> input;
        }
        if (input == -1) {
            return false;
        }
    } while (!valid_input(input));
    account["amount"] = input;
    return true;
}

bool ManagementSystem::set_password(json& account) 
{
    std::string password;
    do {
        print_by_color("Please enter password at least 5 letters long: ", green);
        std::cin >> password;
        if (password == "-1") {
           return false;
        }
    } while (!valid_input(password, 5));
    account["password"] = password;
    return true;
}

bool ManagementSystem::set_id(json& account)
{
    int max_id = 0;
    for (auto& account : m_accounts_list) {
       if (max_id < account["id"]) {
           max_id = account["id"];
       }
    }
    ++max_id;
    if (max_id == 0) {
        return false;
    }
    account["id"] = max_id;
    return true;
}

bool ManagementSystem::valid_input(std::string& value, const int length)
{
    if (value.length() >= length) {
        return true;
    }
    return false;
}

bool ManagementSystem::valid_input(double& value)
{
    if (value > min and value < max) {
        return true;
    }
    return false;
}

void ManagementSystem::update_file()
{
    m_data_file.open("data.json", std::ios::out);
    if (m_data_file.is_open()) {
        m_data_file << std::setw(4) << m_accounts_list;
        m_data_file.close();
    } else {
        print_by_color("Can't open file.", red);
    }
}

void ManagementSystem::print_account(json& account)
{
    std::cout << red;
    std::cout << "Account ID: " << account["id"]
              << "\nName: " << account["name"]
              << "\nSurname: " << account["surname"]
              << "\nAmount: " << account["amount"] << std::endl;
    std::cout << reset;
}

void ManagementSystem::print_account()
{
    std::string password;
    int id;
    bool negative = false;
    print_by_color("Enter id of the account: ", green);
    std::cin >> id;
    while (std::cin.fail() && negative) {
        print_by_color("Error, please enter an integer instead of character/string.\n", red);
        print_by_color("Please try again: ", green);
        std::cin.clear();
        std::cin.ignore(256,'\n');
        std::cin >> id;
        if (0 >= id) {
            negative = true;
        }
    }
    for (auto& account : m_accounts_list) {
        if (account["id"] == id) {
            print_by_color("Please enter account password: ", green);
            std::cin >> password;
            if (password == account["password"]) {
                print_account(account);
            } else {
                print_by_color("INCORRECT PASSWORD\n", red);
            }
            return;
        }
    }
    print_by_color("Wrong ID.There isn't a such account.\n", red);
}

bool ManagementSystem::found_id(int& id)
{
    for (auto& account : m_accounts_list) {
       if (id == account["id"]) {
          return true;
       }
    }
    print_by_color("The entered id is not found. Try again.\n", red);
    return false;
}

void ManagementSystem::update_account()
{
    int id;
    do {
        print_by_color("To quit this /update account/ option, please enter '-1'.\n\n" \
                       "To change account password, amount, please enter account ID: ", green);
        std::cin >> id;
        while (std::cin.fail()) {
            print_by_color("Error, please enter an integer instead of character/string: ", red);
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> id;
        }
        if (-1 == id) {
            print_by_color("You have entered '-1' to quit this option.\n", red);
            return;
        }
    } while (!found_id(id));
    update_account_option(id);
}

void ManagementSystem::update_account_option(int& id)
{
    bool make_update = false;
    std::string password;
    set_accounts_list();
    for (auto& account : m_accounts_list) {
        if (account["id"] == id) {
            print_by_color("Please enter the account password: ", green);
            std::cin >> password;
            while (password != account["password"] && "-1" != password) {
                print_by_color("INCORRECT PASSWORD. To quit this option, please enter -1.\n", red);
                print_by_color("Or try again: ", green);
                std::cin >> password;
            }
            if ("-1" == password) {
                print_by_color("You have entered -1 to quit this option.\n", red);
                return;
            }
            if (update_password(account)) {
                make_update = true;
                print_by_color("The account's password has been updated.\n", green);
            }
            if (update_amount(account)) {
                make_update = true;
                print_by_color("The account's amount has been updated.\n", green);
            }
        }
    }
    if (make_update) {
        update_file();
    } else {
        print_by_color("THERE IS NOTHING TO UPDATE\n", red);
    }
}

void ManagementSystem::print_input_instruction(std::string& password)
{
    print_by_color("Please enter account password: ", green);
    std::cin >> password;
}

void ManagementSystem::print_input_instruction(int& id)
{
    do {
        print_by_color("To quit this option, please enter '-1'.\n\n" \
                       "To change account password, amount, please enter account ID: ", green);
        std::cin >> id;
        while (std::cin.fail()) {
            print_by_color("Error, please enter an integer instead of character/string: ", red);
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> id;
        }
    } while (-1 == id);
    print_by_color("You have entered '-1' to quit this option.\n", red);
}

void ManagementSystem::print_input_instruction(int& id, std::string transfer_way)
{
    bool negative = false;
    print_by_color("To quit the application, please enter '-1'.\n\n" \
                   "Please enter account ID to transfer amount ", green);
    print_by_color(transfer_way, green);
    std::cin >> id;
    if (-1 == id) {
        print_by_color("You have quit this /transaction/ option.\n", red);
        return;
    }
    if (0 >= id) {
        negative = true;
    }
    while (std::cin.fail() or negative) {
        print_by_color("Please enter a positive integer instead of negative integer or character/string.\n", red);
        print_by_color("Please try again: ", green);
        std::cin.clear();
        std::cin.ignore(256,'\n');
        std::cin >> id;
        if (-1 == id) {
            print_by_color("You have quit this /transaction/ option.\n", red);
            return;
        }
        if (0 >= id) {
            negative = true;
            print_by_color("The id should be a positive number. Try again: \n", red);
        }
        if (0 < id) negative = false;
    }
}

bool ManagementSystem::update_password(json& account)
{
    std::string answer;
    std::string password;
    print_by_color("To change the password, please enter 'yes'.\n"\
                   "Or press any key to continue... ", green);
    std::cin >> answer;
    if ("yes" == answer) {
        print_by_color("To quit this option please enter '-1'.\n\n"\
                       "Please enter new password: ", green);
        std::cin >> password;
        if ("-1" == password) {
            return false;
        }
        while (password.length() < 5) {
            print_by_color("The entered password should be at least 5 letters long."\
                           "Please try again: ", green);
            std::cin >> password;
            if (password == "-1") {
                return false;
            }
        }
        if (password == account["password"]) {
            print_by_color("You have enterted the same password.\n", green);
            return false;
        } else {
            account["password"] = password;
            return true;
        } 
    }
    return false;
}

bool ManagementSystem::update_amount(json& account)
{
    std::string answer;
    double amount;
    print_by_color("To change the amount, please enter 'yes'.\n"\
                   "Or press any key to continue... ", green);
    std::cin >> answer;
    if ("yes" == answer) {
        print_by_color("To quit this option, please enter '-1'\n"\
                       "Please enter new amount: ", green);
        do {
            print_by_color("The amount should be positive number: ", green);
            std::cin >> amount;
            while (std::cin.fail()) {
                print_by_color("Error, please enter an integer instead of character/string: ", red);
                std::cin.clear();
                std::cin.ignore(256,'\n');
                std::cin >> amount;
            }
            if (-1 == amount) {
                return false;
            }
        } while (!valid_input(amount));
        if (amount == account["amount"]) {
            print_by_color("You have entered the same amount.\n", green);
            return false;
        } else {
            account["amount"] = amount;
            return true;
        }
    }
    return false;
}

void ManagementSystem::transaction()
{
    set_accounts_list();
    int account_id_from;
    std::string password_from;
    int account_id_to;
    double transfer_amount;
    double max_amount;

    print_input_instruction(account_id_from, "from: ");
    for (auto& account : m_accounts_list) {
        if (account_id_from == account["id"]) {
            print_input_instruction(password_from);
            if (password_from == account["password"]) {
                print_input_instruction(account_id_to, "to: ");
                if (account_id_to == account_id_from ) {
                    print_by_color("The accounts id numbers should be differ.\n", red);
                    print_by_color("The transaction option has been closed.\n", green);
                    return;
                }
                max_amount = account["amount"];
                enter_transfer_amount(transfer_amount, max_amount);
                transfer(transfer_amount, account_id_from, account_id_to);
                return;
            } else {
                print_by_color("Wrong password.\nThe transaction option has been interrupted\n", red);
                return;
            }
        }
    }
}

bool ManagementSystem::enter_transfer_amount(double& amount, double& max_amount)
{
    print_by_color ("The account amount is: ", green);
    print_by_color (max_amount, green);
    print_by_color ("\nPlease enter amount to transfer: ", green);

    do {
        print_by_color("The amount should be positive number and not more"\
                       " than the amount in the mentioned account: ", green);
        std::cin >> amount;
        while (std::cin.fail() or amount > max_amount) {
            print_by_color("Please enter an integer instead of character/string.\n", red);
            print_by_color("The amount can't be more than the account amount.\n"\
                           "Please try again: ", red);
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> amount;
        }
        if (-1 == amount) {
            print_by_color("You have closed transaction option.\n", green);
            return false;
        }
    } while (!valid_input(amount));
}

void ManagementSystem::transfer(double amount, int from, int to)
{
    int sum;
    for (auto& account : m_accounts_list) {
        if (account["id"] == from) {
            sum = account["amount"];
            sum -= amount;
            account["amount"] = sum;
        }
        if (account["id"] == to) {
            sum = account["amount"];
            sum += amount;
            account["amount"] = sum;
        }
    }
    update_file();
    set_accounts_list();
    print_by_color("Amount transfer has successfully done.\n", green);
}

void ManagementSystem::read_list()
{
    std::string input_password;
    print_by_color("Please enter admin password: ", green);
    std::cin >> input_password;
    if (input_password == admin_password) {
        for (auto& account : m_accounts_list) {
            print_line_of('-', 50, red);
            print_account(account);
        }
    } else {
       print_by_color("Wrong admin password.\n", red);
    }
}

void ManagementSystem::remove_account()
{
    int id;
    std::string password;
    print_by_color("To close the application, please enter '-1'\n\n"\
                   "Please enter account id, that you need to remove: ", green);
    if(!enter_correct_id(id)) {
        return;
    }
    print_by_color("Please enter the account password: ", green);
    if (enter_correct_password(id)) {
        remove_account_option(id);
    } else {
        print_by_color("INCORRECT PASSWORD\n"\
                       "THE ACCOUNT WASN'T REMOVED\n", red);
    }
}

void ManagementSystem::remove_account_option(int id)
{
    set_accounts_list();
    json accounts_list;
    bool confirm_to_remove = false;
    for (auto& account : m_accounts_list) {
        if (account["id"] == id) {
            confirm_to_remove = true;
            continue;
        }
        accounts_list.push_back(account);
    }
    if (confirm_to_remove) {
        m_accounts_list = accounts_list;
        update_file();
        set_accounts_list();
        print_by_color("THE ACCOUNT HAS BEEN REMOVED\n", green);
    }
}

bool ManagementSystem::enter_correct_id(int& id)
{
    std::cin >> id;
    while (true) {
        while (std::cin.fail()) {
            print_by_color("Error, please enter an integer instead of character/string.\n", red);
            print_by_color("Please try again: ", green);
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> id;
        }
        if (-1 == id) {
            return false;
        }
        if (found_id(id)) {
            break;
        } else {
            print_by_color("The entered account id doesn't exist.\n", red);
            print_by_color("Please try again: ", green);
            std::cin >> id;
        }
    }
    return true;
}

bool ManagementSystem::enter_correct_password(int id)
{
    set_accounts_list();
    std::string password;
    std::cin >> password;
    for (auto& account : m_accounts_list) {
        if (id == account["id"]) {
            if (password == account["password"]) {
                return true;
            }
            break;
        }
    }
    return false;
}
