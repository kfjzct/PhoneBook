#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cctype>
#include <algorithm>

using std::vector;
using std::string;
using std::map;
using std::cout;
using std::cin;
using std::endl;

typedef struct {
  string lastname, firstname;
} Name;


typedef map<string, string> PhoneNumberCollection;

/* Create a lower-case version of a string.
   param s is the original string
   return a lower-case equivalent
*/
string str_to_lower(const string &s) {
  string result = "";
  for (int i = 0; i < (int) s.length(); i++) {
    result += std::tolower(s.at(i));
  }
  return result;
}

/* Compare two Name structures, ignoring capitalization.
   return true if left comes before right alphabetically
   using lastname as the primary comparison, breaking ties with firstname.
   return false otherwise
*/
bool operator<(const Name &left, const Name &right) {
  string templl;
  string temprl;
  string templf;
  string temprf;
  templl = str_to_lower(left.lastname);
  temprl = str_to_lower(right.lastname);
  templf = str_to_lower(left.firstname);
  temprf = str_to_lower(right.firstname);
  if (templl < temprl) {
    return true;
  }
  if (templl == temprl) {
    if (templf < temprf) {
      return true;
    }
  }
  return false;
}

bool goodnumber (const string number) {
  for (int k = 0; k < (int) number.length(); k++) {
    int numberlength = number.length();
    if (number[k] != '-' && isdigit(number[k]) < 1) {
      return false;
    }
    if (isdigit(number[0]) < 1 || isdigit(number[numberlength - 1]) < 1) {
      return false;
    }
  }
  return true;
}


int main() {
  cout<<"Info: Welcome to the Phone Database!"<<endl;
  map<Name, PhoneNumberCollection> phone_db;
  string ch;
  char useless;
  do {
    cout<<"Info: Please enter a command"<<endl;
    cin >> ch;
    if (ch == "C") {
      string tempfirst;
      string templast;
      cin >> templast >> tempfirst;
      Name temp = {templast, tempfirst};
      if (phone_db.find(temp) == phone_db.end()) {
        phone_db[temp];
        cout << "Result: Contact created" << endl;
      }
      else {
        cout<<"Error: Contact already exists"<<endl;
      }
    }
    if (ch == "D") {
      string tempfirst;
      string templast;
      cin >> templast >> tempfirst;
      Name temp = {templast, tempfirst};
      if (phone_db.find(temp) == phone_db.end()) {
        cout<<"Error: Contact not found"<<endl;
      }
      else {
        phone_db.erase(temp);
        cout<<"Result: Contact deleted"<<endl;
      }
    }
    if (ch == "L") {
      int count = 0;
      for (std::map<Name, PhoneNumberCollection>::iterator i = phone_db.begin(); i != phone_db.end(); i++) {
        cout << "Result: " << i->first.lastname << "," << i->first.firstname << endl;
        count++;
      }
      cout << "Info: There are " << count << " contact(s)" << endl;
    }
    if (ch == "P") {
      int numberpresent = 1;
      string tempfirst;
      string templast;
      cin >> templast >> tempfirst;
      Name temp = {templast, tempfirst};
      int counter = 0;
      if (phone_db.find(temp) == phone_db.end()) {
        cout<<"Error: Contact not found"<<endl;
      }
      else {
        for (std::map<Name, PhoneNumberCollection>::iterator i = phone_db.begin(); i != phone_db.end(); i++) {
          if (str_to_lower(i->first.lastname) == str_to_lower(temp.lastname) && str_to_lower(i->first.firstname) == str_to_lower(temp.firstname)) {
            if (i->second.empty()) {
              cout << "Info: There are no phone numbers for this contact" << endl;
              numberpresent = 0;
            }
            else {
              for (std::map<string, string>::iterator j = i->second.begin(); j != i->second.end(); j++) {
                cout << "Result: " << j->first << "," << j->second << endl;
                counter++;
              }
            }
          }
        }
        if (numberpresent == 1)cout << "Info: Found " << counter << " phone number(s) for this contact" << endl;
      }
    }
    if (ch == "N") {
      string tempfirst;
      string templast;
      string type;
      string number;
      int present = 0;
      cin >> templast >> tempfirst >> type >> number;
      Name temp = {templast, tempfirst};
      for (std::map<Name, PhoneNumberCollection>::iterator i = phone_db.begin(); i != phone_db.end(); i++) {
        if (str_to_lower(i->first.lastname) == str_to_lower(temp.lastname) && str_to_lower(i->first.firstname) == str_to_lower(temp.firstname)) {
          int numexist = 1;
          if (i->second.find(type) == i->second.end()) {
            numexist = 0;
          } 
          if (type != "CELL" && type != "WORK" && type != "HOME" && type != "FAX" && type != "VOIP") {
            cout << "Error: Invalid phone number type" << endl;
            break;
          } 
          if (!goodnumber(number)) {
            cout << "Error: Not a valid phone number" << endl;
            break;
          }
          i->second[type] = number;
          if (numexist == 0) {
            cout << "Result: Phone number added" << endl;
          }
          else {
            cout << "Result: Phone number replaced" << endl;
          }
        present = 1;
        }
      }
      if (present == 0) {
        cout << "Error: Contact not found" << endl;
      }
    }
    if (ch == "X") {
      string tempfirst;
      string templast;
      string type;
      int present = 0;
      cin >> templast >> tempfirst >> type;
      Name temp = {templast, tempfirst};
      for (std::map<Name, PhoneNumberCollection>::iterator i = phone_db.begin(); i != phone_db.end(); i++) {
        if (str_to_lower(i->first.lastname) == str_to_lower(temp.lastname) && str_to_lower(i->first.firstname) == str_to_lower(temp.firstname)) {
          i->second.erase(type);
          cout << "Result: Phone number deleted" << endl;
          present = 1;
        }
      }
      if (present == 0) {
          cout << "Error: Contact not found" << endl;
        }
    }
    if (ch == "S") {
      string filename;
      cin >> filename;
      std::ofstream ofile(filename);
      if (!ofile.is_open()) {
        cout << "Could not open output file" << endl;
      }
      ofile << phone_db.size() << endl;
      for (std::map<Name, PhoneNumberCollection>::iterator i = phone_db.begin(); i != phone_db.end(); i++) {
        ofile << i->first.lastname << " " << i->first.firstname << " " << i->second.size() << " ";
        for (std::map<string, string>::iterator j = i->second.begin(); j != i->second.end(); j++) {
          ofile << j->first << " " << j->second << " ";
        }
        ofile << endl;
      }
    }
    if (ch == "R") {
      string filename;
      int numbercontacts;
      cin >> filename;
      std::ifstream ifile(filename);
      if (!ifile.is_open()) {
        cout << "Could not open output file" << endl;
      }
      ifile >> numbercontacts;
      for (int i = 0; i < numbercontacts; i++) {
        string tempfirst;
        string templast;
        int numberphones;
        PhoneNumberCollection temporary;
        ifile >> templast >> tempfirst;
        Name temp = {templast, tempfirst};
        phone_db[temp];
        ifile >> numberphones;
        if (numberphones > 0) {        
          for (int j = 0; j < numberphones; j++) {
            string type;
            string number;
            ifile >> type;
            ifile >> number;
            temporary[type] = number;
          }
        }
        phone_db[temp] = temporary; 
      }
    }
    cin.get(useless);
  }
  while (ch != "Q");
  cout<<"Info: Thank you for using the Phone Database!"<<endl;
  return 0;
}