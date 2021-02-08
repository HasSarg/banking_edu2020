#ifndef MANAGEMENT_SYSTEM_H
#define MANAGEMENT_SYSTEM_H

#include <string>
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;
const int options_qty = 7;

class ManagementSystem {
    private:
        std::string m_menu_title;
        std::string m_menu_options[options_qty];
        int m_choice;
        json m_accounts_list;
        std::fstream m_data_file;
        std::string admin_password;
    public:
        ManagementSystem();
        void run();
    private:
        void display_menu();
        void print_by_color(std::string message, std::string color);
        void print_by_color(int message, std::string color);

	void set_accounts_list();
        void make_choice();
        void start();
        void print_line_of(char sign, int quantity, std::string color);
        bool action_by_choice(const int& m_choice);
        void create_account();

        void print_account(json&);
        void print_account();
        bool set_name(json& account);
        bool set_surname(json& account);
        bool set_amount(json& account);
        bool set_password(json& account);
        bool set_id(json& account);
        bool valid_input(std::string& value, const int length);
        bool valid_input(double& value);

        void update_file();
        void update_account();
        void update_account_option(int& id);
        void print_input_instruction(std::string& password);
        void print_input_instruction(int& id);
        void print_input_instruction(int& id, std::string transfer_way);
        bool found_id(int& id);
        bool update_password(json& account);
        bool update_amount(json& account);
        void transaction();
        bool enter_transfer_amount(double& transfer_amount, double& max_amount);
        void transfer(double amount, int from, int to);
        void remove_account();
        void remove_account_option(int id);
        bool enter_correct_id(int& id);
        bool enter_correct_password(int id);
        void read_list();
};
#endif
