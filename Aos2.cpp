
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>

using namespace std;

class CPU { // all fitches of CPU 
    // and all kinds  of function we can do 
private:
    enum class commands {
        mov, // To transport info in our Accumulator (RA)
        addit,
        save1, save2,save3 // To save in each  register (R0, R1 ,R2 )
    };
    string file_name; // we will take info from this  file 

    static const int our_bitrate = 18; // My varriant is 2

    string current_com;  // Ongoing commands
    string command_type;

    int mov_value;
    int realize_variant;

    int operand_for_nonmov_oper;   // numbers of needed register

    int reg_status = 0;
    int calculate_com = 0;
    int tacts_num = 0;

    void mov(int dec_number);
    void addit(int dec_number);
    int realize_varriant(vector<int> reg_accumulator, vector<int> reg_all_infor);
    int operation0(vector<int> reg_accumulator); // To save info in R0
    int operation1(vector<int> reg_accumulator);// ........In R1
    int operation2(vector<int> reg_accumulator);//.... in R2
    void parse_command(const string& com);
    void variant_command(const string& com);
    void load_commands();
public:
    vector<string> our_commands;
    vector<int> accumulator;
    vector<int> registration;


    vector<int> regist0 = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // 18 bits no more
    vector<int> regist1 = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    vector<int> regist2 = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    static vector<int> trans_to_binary(int dec_number);
    static void trans_to_adit(vector<int>& st_code_number); // convert param to additional code
    
    void print_tact_info();
    explicit CPU(string& info_from_file);
    
    void lets_fu_go();
    void do_command(string& com);

};


CPU::CPU(string& txt_input_file) :
    accumulator(vector <int> {vector<int>(our_bitrate, 0)}), file_name(txt_input_file)
{
    load_commands();
}

void CPU::parse_command(const string& command) {
    int pos_of_space = command.find(' ');
    current_com = command;
    command_type = command.substr(0, pos_of_space);
    if (command_type == "mov") {
        mov_value = stoi(command.substr(pos_of_space + 1));
    } 
}

void CPU::variant_command(const string& command) {
    int pos_of_space = command.find(' ');
    current_com = command;
    command_type = command.substr(0, pos_of_space);
    if (command_type == "addit") {
        realize_variant = stoi(command.substr(pos_of_space + 1));
    }
}



void CPU::mov(int dec_number) { // from binary to additional form 
    accumulator = trans_to_binary(dec_number); 
    reg_status = accumulator[0]; // set sign of number in status register
    if (dec_number < 0) { // if number less than 0 , tranform into additional form
        trans_to_adit(accumulator);  
    }

}
void CPU::addit(int dec_number) { // from binary to additional form 
    registration = trans_to_binary(dec_number);
    reg_status = registration[0]; // set sign of number in status register
    if (dec_number < 0) { // if number less than 0 , tranform into additional form
        trans_to_adit(registration);
    }

}

vector<int> CPU::trans_to_binary(int dec_number) { // Transform to binary form 
    vector<int> res;
    int number_sign = 0;
    assert(dec_number < pow(2, our_bitrate));
    if (dec_number == 0) {
        return vector<int>(our_bitrate, 0);
    } else if (dec_number < 0) {
        dec_number = abs(dec_number);
        number_sign = 1;
    }  while (dec_number != 0) {
        res.push_back(dec_number % 2);
        dec_number = dec_number / 2;
    } while (res.size() < our_bitrate - 1) {
        res.push_back(0);
    }
    res.push_back(number_sign); // it will be first sign bit
    reverse(res.begin(), res.end());
    return res;
}

void CPU::trans_to_adit(vector<int>& st_code_number) {  // convert to aditional form 
    if (st_code_number != vector<int>(our_bitrate, 0)) {
        st_code_number[0] = 0;
        for (int& i : st_code_number) {
            if (i == 0) {
                i = 1;
            }
            else {
                i = 0;
            }
        }
        int j = st_code_number.size() - 1;
        while (st_code_number[j] == 1) {
            st_code_number[j] = 0;
            j--;
        }
        st_code_number[j] = 1;
    }
}


void CPU::load_commands() {
    ifstream fin(file_name);

    char num[64];
    assert(fin.is_open());

    while (!fin.eof()) {
        fin.getline(num, 25);
        our_commands.emplace_back(num);
    }
}

void CPU::print_tact_info() { // what will be client see
    cout << "IR: " << current_com << endl;



    cout << "RAcum:"; // Our Acumulator
    for (int i = 0; i < accumulator.size(); i++) {
        if (i % 6 == 0) {   // Pseudo space
            cout << " ";
        }  cout << accumulator[i];
    }   cout << endl;

    cout << "R0:   "; 
    for (int i = 0; i < accumulator.size(); i++) {
        if (i % 6 == 0) { // Pseudo space
            cout << " ";
        } cout << regist0[i];
    }cout << "           PS: " << reg_status << endl; // More or below zero

    cout << "R1:   ";
    for (int i = 0; i < accumulator.size(); i++) {
        if (i % 6 == 0) { // Pseudo space
            cout << " ";
        } cout << regist1[i];
    }cout << "           TC: " << tacts_num << endl; // Calculate our tacts

    cout << "R2:   ";
    for (int i = 0; i < accumulator.size(); i++) {
        if (i % 6 == 0) { // Pseudo space
            cout << " ";
        } cout << regist2[i];
    } cout << "           PC: " << calculate_com << endl;


    cout << "-wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww-\n" << "\ Press 'Enter' to next  tact";

}

void CPU::do_command(string& command) {
    parse_command(command);
    variant_command(command);

    if (command_type == "mov") {  // Realize our "mov"
        tacts_num++;  // First tact of first command 
        print_tact_info(); //  Output Before CPU start work
        char tmp[1];
        cin.getline(tmp, 1); // wait for view next tact
        mov(mov_value);
        tacts_num++; // Second tact of first command
        print_tact_info();// Output After Command  / After CPU work
        char tmp1[1];
        cin.getline(tmp1, 1); // // "Enter" to see next tact
    } if (command_type == "addit") {  // Realize our "mov"
        tacts_num++;  // First tact of first command 
        print_tact_info(); //  Output Before CPU start work
        char tmp[1];
        cin.getline(tmp, 1); // wait for view next tact
        addit(realize_variant);
        mov_value = realize_varriant(accumulator, registration);
        tacts_num++; // Second tact of first command
        print_tact_info();// Output After Command  / After CPU work
        char tmp1[1];
        cin.getline(tmp1, 1); // // "Enter" to see next tact
    } else if (command_type == "save_R0") { // Realize our "add"
        tacts_num++; //First tact of second command
        print_tact_info(); //  Output Before CPU start work
        char tmp[1];
        cin.getline(tmp, 1);
        mov_value = operation0(accumulator);
        tacts_num++;// Second command of second command
        reg_status = (mov_value > 0) ? 0 : 1;// If more than "0" - 0 ,else 1 
        print_tact_info();// Output After Command  / After CPU work
        char tmp1[1];
        cin.getline(tmp1, 1);
    } else if (command_type == "save_R1") {
        tacts_num++; //First tact of second command
        print_tact_info(); //  Output Before CPU start work
        char tmp[1];
        cin.getline(tmp, 1);
        mov_value = operation1(accumulator);
        tacts_num++;// Second command of second command
        reg_status = (mov_value > 0) ? 0 : 1; // If more than "0" - 0 ,else 1 
        print_tact_info();// Output After Command  / After CPU work
        char tmp1[1];
        cin.getline(tmp1, 1);
    }
    else if (command_type == "save_R2") {
        tacts_num++; //First tact of second command
        print_tact_info(); //  Output Before CPU start work
        char tmp[1];
        cin.getline(tmp, 1);
        mov_value = operation2(accumulator);
        tacts_num++;// Second command of second command
        reg_status = (mov_value > 0) ? 0 : 1; // If more than "0" - 0 ,else 1 
        print_tact_info();// Output After Command  / After CPU work
        char tmp1[1];
        cin.getline(tmp1, 1);
    }
}

void CPU::lets_fu_go() {
    for (auto& com : our_commands) {
        calculate_com++; // Who mutch comands we use
        do_command(com);
        tacts_num = 0;
    }

}

int CPU::operation0(vector<int> acum_reg) { // We realize our Save (From RA to R0)
     regist0 = acum_reg;
    return 0;
}

int CPU::operation1(vector<int> acum_reg) {// We realize our Save (From RA to R1)
    regist1 = acum_reg;
    return 0;
}

int CPU::operation2(vector<int> acum_reg) {// We realize our Save (From RA to R2)
    regist2 = acum_reg;
    return 0;
}


int CPU::realize_varriant(vector<int>  acum_regist, vector<int> data_regist) {
    vector<int> x = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int for_record = 0;

    if (acum_regist < data_regist) {
        cout << "Incorect pair of numbers  . Accum must be greater than Register \n We end our programm ,because some information is incorect " << endl;
        exit(1);
    }
    if (acum_regist >= data_regist) {
        for (int i = 17; i >= 0; i--) {
            if ((-data_regist[i] + acum_regist[i] - for_record) == 0) {
                x[i] = 0;
                for_record = 0;
            }
            else if ((-data_regist[i] + acum_regist[i] - for_record) == 1) {
                x[i] = 1;
                for_record = 0;
            }
            else if ((-data_regist[i] + acum_regist[i] - for_record) == -1) {
                x[i] = 1;
                for_record = 1;
            }
            else if ((-data_regist[i] + acum_regist[i] - for_record) == -2) {
                x[i] = 1;
                for_record = 1;
            }
            accumulator = x;  // Save  in  R_ACUM

        }
    }


    return 0;
}


int main() {

    string init = "E:\\VisualProjects\\Aos2\\ForAos.txt"; //Take info from file ..... 
   
    cout << " Ivan Bzylien K -24 \nOur negative numbers will be convert in supplementary code" << endl;
    CPU p(init);
    p.lets_fu_go();


    return 0;

}
