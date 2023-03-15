
#include <iostream>

#include <string>

#include <cmath>
#include <fstream>
#include <utility>
#include <vector>
#include <regex>

using namespace std;
time_t now = time(nullptr);
tm *ltm = localtime(&now);
int currentYear = 1900 + ltm->tm_year;
int currentMonth = 1 + ltm->tm_mon;
int currentDay = ltm->tm_mday;

class Train {
public:
    string id;
    int numberStations;
    int day;
    int month;
    int year;
    vector<string> stations;
    vector<string> time;

    Train(string id, int nrStations, int d, int m, int y) {
        this->id = std::move(id);
        this->numberStations = nrStations;
        this->day = d;
        this->month = m;
        this->year = y;
    }

    void addStation(const string& st, const string& t) {
        stations.push_back(st);
        time.push_back(t);
    }

    void display() {
        cout << "ID: " << id << endl;
        cout << "Date: " << day << '/' << month << '/' << year << endl;
        cout << "STATIONS: " << endl;
        for (int i = 0; i < numberStations; i++) {
            cout << stations[i] << "   " << time[i] << endl;
        }
    }

    void printValid(const string& station1, const string& station2, int *ok) {
        int pos1 = -1, pos2 = -1, i;
        for (i = 0; i < numberStations; i++) {
            if (stations[i] == station1)pos1 = i;
            if (stations[i] == station2)pos2 = i;
        }
        if (pos1 < pos2 && pos1 != -1 && pos2 != -1) {
            *ok = 1;
            cout << "ID:" << id << endl;
            cout << "STATIONS:" << endl;
            for (i = pos1; i <= pos2; i++) {
                cout << stations[i] << "   " << time[i] << endl;
            }
        }

    }
};

class User {
private:
    string username;
    string password;
public:
    string email;
    string phone;

    User(string u, string p) {
        this->username = std::move(u);
        this->password = std::move(p);
    }

    User(string u, string p, string e, string ph) {
        this->username = u;
        this->password = p;
        this->email = e;
        this->phone = ph;
    }

    void display() {
        cout << "Username:" << this->username << endl;
        cout << "Password:" << this->password << endl;
        cout << "Email:" << this->email << endl;
        cout << "Phone number:" << this->phone << endl;
    }

    string getUsername() {
        return this->username;
    }

    string getPassword() {
        return this->password;
    }

    void emailValid() {
        const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
        if (!regex_match(this->email, pattern))throw "Invalid email!";
    }

    void passwordValid(string p1, string p2) {
        if (p1 != p2)throw "Passwords don't match";
        else {
            int ok1 = 0, ok2 = 0, ok3 = 0, ok4 = 0, ok5 = 0;
            for (int i = 0; i < p1.size(); i++) {
                if (p1[i] >= 'a' && p1[i] <= 'z')ok1 = 1;
                else if (p1[i] >= 'A' && p1[i] <= 'Z')ok2 = 1;
                else if (p1[i] >= '0' && p1[i] <= '9')ok3 = 1;
                else ok4 = 1;
            }
            if (p1.size() > 7)ok5 = 1;
            if (!(ok1 && ok2 && ok3 && ok4 && ok5))throw "Password too weak";

        }
    }

    int logIn() {
        ifstream userData;
        userData.open("userData.csv");
        string line = "";
        int ok = 0;
        while (getline(userData, line)) {
            string u;
            string p;
            string e;
            string ph;
            stringstream inputString(line);
            getline(inputString, u, ',');
            getline(inputString, p, ',');
            getline(inputString, e, ',');
            getline(inputString, ph);
            if (this->username == u && this->password == p) {
                this->email = e;
                this->phone = p;
                userData.close();
                ok = 1;
                return 1;
            }
            line = "";
        }
        userData.close();
        if (ok == 0)throw "Username or password not valid!";
    }

    void add() {
        ofstream userData;
        userData.open("userData.csv", ios::app);
        userData << this->username << ',' << this->password << ',' << this->email << ',' << this->phone << endl;
        userData.close();
    }

    void searchTrain(string start, string stop, vector<Train> trains, int d, int m, int y) {
        int count = 0;
        for (auto train: trains) {
            if (train.day == d && train.month == m && train.year == y) {
                train.printValid(start, stop, &count);
            }
        }
        if (count == 0) {
            throw "No matching train";
        }
    }

    void buyTicket(string start, string stop, string currentId, vector<Train> trains) {
        for (auto train: trains) {
            if (train.id == currentId) {
                cout << "<---------TICKET---------->" << endl;
                cout << "Email:" << this->email << endl;
                int i;
                for (i = 0; i < train.numberStations; i++) {
                    if (train.stations[i] == start)
                        cout << "Start:" << train.stations[i] << "===>" << "Time:" << train.time[i] << endl;
                    if (train.stations[i] == stop)
                        cout << "Stop:" << train.stations[i] << "===>" << "Time:" << train.time[i] << endl;
                }
                cout << "<---------TICKET---------->" << endl;
            }
        }
    }

    void validDate(int d, int m, int y) {
        if (y < currentYear || m < currentMonth && y <= currentYear ||
            d < currentDay && m <= currentMonth && y <= currentYear) {
            throw "Past date!";
        }
        if ((m < 0 || m > 12) || (d > 31 || d < 0) && m % 2 == 1 || (d > 30 || d < 0) && m % 2 == 0 ||
            d > 29 && m == 2 && y % 4 == 0 || d > 28 && m == 2 && y % 4 != 0) {
            throw "Invalid date!";
        }
    }
};

class Operator {
private:
    string username;
    string password;
public:
    Operator(string u, string p) {
        this->username = u;
        this->password = p;
    }

    void logIn() {
        ifstream file;
        file.open("operatorData.csv");
        string realUsername, realPassword;
        getline(file, realUsername, ',');
        getline(file, realPassword, ',');
        file.close();
        if (!(this->password == realPassword && this->username == realUsername))
            throw "Username or password not correct!";
    }

    void addTrain(int d, int m, int y) {
        string id, station, time;
        int nrStations;
        cout << "ID:";
        cin >> id;
        int day, month, year;
        cout << "[DATE]" << endl;
        cout << "Day:";
        cin >> day;
        cout << "Month:";
        cin >> month;
        cout << "Year:";
        cin >> year;
        if (year < y || month < m && year <= y || day < d && month <= m && year <= y) {
            throw "Past date!";
        }
        if ((month < 0 || month > 12) || (day > 31 || day < 0) && month % 2 == 1 ||
            (day > 30 || day < 0) && month % 2 == 0 || day > 29 && month == 2 && year % 4 == 0 ||
            day > 28 && month == 2 && year % 4 != 0) {
            throw "Incorrect date!";
        } else {
            cout << "Number of stations:";
            cin >> nrStations;
            ofstream trainData;
            trainData.open("trainData.csv", ios::app);
            string aux;
            aux = id + ',' + to_string(day) + ',' + to_string(month) + ',' + to_string(year) + ',' +
                  to_string(nrStations) + ',';
            int i;
            for (i = 1; i <= nrStations; i++) {
                cout << "Station " << i << ':';
                cin.get();
                getline(cin, station);
                int ok = 1;
                for (int j = 0; j < station.size(); j++)
                    if (!(station[j] >= 'a' && station[j] <= 'z' || station[j] >= 'A' && station[j] <= 'Z' ||
                          station[j] == ' ' || station[j] >= '0' && station[j] <= '9'))
                        ok = 0;
                if (ok == 0)
                    throw "Station name invalid! Station name must contain only alphabetic and numeric characters!";
                else {
                    cout << "Time:";
                    cin >> time;
                    if (i < nrStations)aux = aux + station + ',' + time + ',';
                    else aux = aux + station + ',' + time + '\n';
                }
            }
            trainData << aux;
            trainData.close();
        }
    }

    void deleteTrain(string chosenId, vector<Train> trains) {
        ofstream trainData;
        trainData.open("trainData.csv");
        int ok = 0;
        for (auto train: trains) {
            if (train.id != chosenId) {
                trainData << train.id << ',' << train.day << ',' << train.month << ',' << train.year << ','
                          << train.numberStations << ',';
                int i;
                for (i = 0; i < train.numberStations - 1; i++)
                    trainData << train.stations[i] << ',' << train.time[i] << ',';
                trainData << train.stations[i] << ',' << train.time[i] << endl;
            } else ok = 1;
        }
        if (ok == 0)throw ("Invalid ID!");
        trainData.close();
    }
};

void readTrains(vector<Train> *trains) {
    ifstream trainData;
    trainData.open("trainData.csv");
    string line = "";
    while (getline(trainData, line)) {
        string id, nrStations, station, time, day, month, year;
        stringstream inputString(line);
        getline(inputString, id, ',');
        getline(inputString, day, ',');
        getline(inputString, month, ',');
        getline(inputString, year, ',');
        getline(inputString, nrStations, ',');
        int count = stoi(nrStations);
        int d = stoi(day);
        int m = stoi(month);
        int y = stoi(year);
        Train train(id, count, d, m, y);
        for (int i = 1; i < count; i++) {
            getline(inputString, station, ',');
            getline(inputString, time, ',');
            train.addStation(station, time);
        }
        getline(inputString, station, ',');
        getline(inputString, time);
        train.addStation(station, time);
        trains->push_back(train);
        line = "";
    }
    trainData.close();
}

string rsa(string x) {
    string y;
    for (int i = 0; i < x.size(); i++) {
        auto message = double(x[i]);
        if (message == 99)message = 199;
        double c = pow(message, 7); //encrypt the message
        c = fmod(c, 13 * 11);
        y.push_back(char(c));
    }
    return y;
}

int main() {
    int choice;
    cout << "[1]User          [2]Operator" << endl;
    cin >> choice;
    switch (choice) {

        case 1: {

            int choice1;
            cout << "[1]Log in          [2]Register" << endl;
            cin >> choice1;
            switch (choice1) {

                case 1: {
                    vector<Train> trains;
                    readTrains(&trns);
                    string currentUsername, currentPassword;
                    cout << "Username:";
                    cin >> currentUsername;
                    cout << "Password:";
                    cin >> currentPassword;
                    string encryptedCurrent;
                    encryptedCurrent = rsa(currentPassword);
                    User user(currentUsername, encryptedCurrent);
                    try {
                        user.logIn();
                        cout << "Success!" << endl;
                        cout << "[1]Book a train     [2]See all trains available" << endl;
                        int choice2;
                        cin >> choice2;
                        switch (choice2) {
                            case 1: {
                                string station1, station2;
                                cout << "From:";
                                cin.get();
                                getline(cin, station1);
                                cout << "To:";
                                getline(cin, station2);
                                int d, m, y;
                                cout << "[Date]" << endl;
                                cout << "Day:";
                                cin >> d;
                                cout << "Month:";
                                cin >> m;
                                cout << "Year:";
                                cin >> y;
                                try {
                                    user.validDate(d, m, y);
                                    try {
                                        user.searchTrain(station1, station2, trains, d, m, y);
                                        cout << "[1]Buy ticket      [2]Exit" << endl;
                                        int choice3;
                                        cin >> choice3;
                                        if (choice3 == 1) {
                                            string currentId;
                                            cout << "ID:";
                                            cin >> currentId;
                                            user.buyTicket(station1, station2, currentId, trains);
                                        } else if (choice3 == 2) {
                                            return 0;
                                        }
                                    }
                                    catch (const char *msg) {
                                        cerr << msg << endl;
                                    }
                                }
                                catch (const char *msg) {
                                    cerr << msg << endl;
                                }
                                break;
                            }
                            case 2:
                                for (auto train: trains) {
                                    train.display();
                                    cout << endl;
                                }
                                break;
                        }
                    } catch (const char *msg) {
                        cerr << msg << endl;
                    }
                    break;
                }
                case 2:
                    string u, p, e, ph, cp;
                    cout << "Username:";
                    cin >> u;
                    cout << "Password:";
                    cin >> p;
                    cout << "Confirm password:";
                    cin >> cp;
                    cout << "Email:";
                    cin >> e;
                    cout << "Phone:";
                    cin >> ph;
                    string encryptedPassword;
                    encryptedPassword = rsa(p);
                    User user(u, encryptedPassword, e, ph);
                    try {
                        user.emailValid();
                        user.passwordValid(p, cp);
                        cout << "Success!";
                        user.add();
                    }
                    catch (const char *msg) {
                        cerr << msg << endl;
                    }
                    break;
            }
            break;
        }
        case 2: {
            cout << "[LOG IN]" << endl;
            string currentUsername, currentPassword;
            cout << "Username:";
            cin >> currentUsername;
            cout << "Password:";
            cin >> currentPassword;
            Operator op(currentUsername, currentPassword);
            try {
                op.logIn();
                cout << "Success!" << endl;
                int choiceOp;
                cout << "[1]Add train           [2]Delete train" << endl;
                cin >> choiceOp;
                switch (choiceOp) {
                    case 1: {
                        try {
                            op.addTrain(currentDay, currentMonth, currentYear);
                        }
                        catch (const char *msg) {
                            cerr << msg << endl;
                        }
                        break;
                    }
                    case 2: {
                        try {
                            vector<Train> trains;
                            readTrains(&trains);
                            string id;
                            cout << "Choose ID:";
                            cin >> id;
                            op.deleteTrain(id, trains);
                        }
                        catch (const char *msg) {
                            cerr << msg << endl;
                        }
                        break;
                    }
                }
            }
            catch (const char *msg) {
                cerr << msg << endl;
            }
            break;
        }
    }
    return 0;
}