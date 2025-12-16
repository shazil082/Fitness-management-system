#include <iostream>
#include <fstream>
#include <iomanip>  // For setw table formatting
#include <string>   // For string data type
#include <thread>   // For sleep / delay
#include <chrono>   // For time duration

using namespace std;

/*===================== WORKOUT PLAN CLASSES =======================*/
class WorkoutPlan {
public:

    // Virtual function to show workout plan
    virtual void showPlan() {
        cout << "Default Workout Plan" << endl;
    }
    virtual ~WorkoutPlan() {}
};

// Workout plan for underweight users
class BulkPlan : public WorkoutPlan {
public:

    // Override function
    void showPlan() override {
        cout << "\n--- Workout Plan for Underweight (Bulk) ---\n";
        cout << "- Heavy weight training\n- High calorie diet\n- Protein surplus\n";
    }
};

class MaintainPlan : public WorkoutPlan {
public:
    void showPlan() override {
        cout << "\n--- Workout Plan for Normal (Maintain) ---\n";
        cout << "- Balanced cardio & strength\n- Normal diet\n- Regular routine\n";
    }
};

class CutPlan : public WorkoutPlan {
public:
    void showPlan() override {
        cout << "\n--- Workout Plan for Overweight (Cut) ---\n";
        cout << "- High cardio\n- Calorie deficit\n- Light weights\n";
    }
};

/*========================== USER CLASS ===========================*/
class User {
public:
    string name, username, password;
    int age;
    float height, weight;
    bool subscribed = false;
    string customPlan = "";

    float getBMI() { return weight / (height * height); }

    // Function to get BMI category
    string category() {
        float bmi = getBMI();
        if (bmi < 18.5) return "Underweight";
        if (bmi <= 24.9) return "Normal";
        return "Overweight";
    }

    // Function to display profile
    void displayProfile() {
        cout << "\n--- Profile ---\n";
        cout << "Name: " << name << "\nAge: " << age << "\nHeight: " << height << "\nWeight: " << weight << endl;
        cout << "BMI: " << getBMI() << " (" << category() << ")\n";
        cout << "Subscription: " << (subscribed ? "Active" : "Not Subscribed") << endl;
        if (!customPlan.empty()) cout << "Custom Plan: " << customPlan << endl;
    }
};

/*=================== UTILITY FUNCTIONS ====================*/
// Typing effect
void typeOut(const string& text, int millis = 20) {
    for (char c : text) {       // Loop through characters
        cout << c << flush;     // Print character
        this_thread::sleep_for(chrono::milliseconds(millis));
    }
}

// Helper to extract value after ": "
string getValue(const string& line) {
    size_t pos = line.find(':');
    if (pos == string::npos) return "";
    return line.substr(pos + 2); // skip ": "
}

/*=================== FILE FUNCTIONS ====================*/

// Load user by username from file
bool loadUser(const string& uname, User& user) {
    ifstream file("users.txt");   // Open file
    string line;
    while (getline(file, line)) {
        if (line == "---USER START---") {

            // Read user data
            string name, age, height, weight, username, password, sub, plan, end;
            getline(file, name);
            getline(file, age);
            getline(file, height);
            getline(file, weight);
            getline(file, username);
            getline(file, password);
            getline(file, sub);
            getline(file, plan);
            getline(file, end); // ---USER END---

            if (getValue(username) == uname) {  // Check username
                user.name = getValue(name);
                user.age = stoi(getValue(age));
                user.height = stof(getValue(height));
                user.weight = stof(getValue(weight));
                user.username = getValue(username);
                user.password = getValue(password);
                user.subscribed = (getValue(sub) == "Yes");
                user.customPlan = getValue(plan);
                return true;
            }
        }
    }
    return false;
}

// Update or add a user in file
void updateUserFile(const User& user) {
    ifstream file("users.txt"); // Read file
    ofstream tempFile("temp.txt");    // Temporary file
    bool updated = false;
    string line;

    while (getline(file, line)) {
        if (line == "---USER START---") {

            // Read existing user
            string name, age, height, weight, username, password, sub, plan, end;
            getline(file, name);
            getline(file, age);
            getline(file, height);
            getline(file, weight);
            getline(file, username);
            getline(file, password);
            getline(file, sub);
            getline(file, plan);
            getline(file, end);


            // If same user, update data
            if (getValue(username) == user.username) {

                tempFile << "---USER START---\n";
                tempFile << "Name: " << user.name << "\n";
                tempFile << "Age: " << user.age << "\n";
                tempFile << "Height: " << user.height << "\n";
                tempFile << "Weight: " << user.weight << "\n";
                tempFile << "Username: " << user.username << "\n";
                tempFile << "Password: " << user.password << "\n";
                tempFile << "Subscribed: " << (user.subscribed ? "Yes" : "No") << "\n";
                tempFile << "CustomPlan: " << user.customPlan << "\n";
                tempFile << "---USER END---\n";
                updated = true;
            }
            else {
                // Write old user in temp file
                tempFile << "---USER START---\n" << name << "\n" << age << "\n" << height << "\n"
                    << weight << "\n" << username << "\n" << password << "\n" << sub << "\n"
                    << plan << "\n---USER END---\n";
            }
        }
    }

    // If user not found, add new user
    if (!updated) {
        tempFile << "---USER START---\n";
        tempFile << "Name: " << user.name << "\n";
        tempFile << "Age: " << user.age << "\n";
        tempFile << "Height: " << user.height << "\n";
        tempFile << "Weight: " << user.weight << "\n";
        tempFile << "Username: " << user.username << "\n";
        tempFile << "Password: " << user.password << "\n";
        tempFile << "Subscribed: " << (user.subscribed ? "Yes" : "No") << "\n";
        tempFile << "CustomPlan: " << user.customPlan << "\n";
        tempFile << "---USER END---\n";
    }

    file.close();
    tempFile.close();           // Close temp fil
    remove("users.txt");       // Delete old file
    rename("temp.txt", "users.txt");  // Rename new file
}

// Write all users in table in file
void writeUsersReadable() {
    ifstream file("users.txt");
    ofstream outFile("users_readable.txt");

    outFile << left; // Align text to left

    outFile << setw(15) << "Name"
        << setw(10) << "Age"
        << setw(10) << "Height"
        << setw(10) << "Weight"
        << setw(15) << "Username"
        << setw(12) << "Subscribed"
        << setw(30) << "CustomPlan" << endl;

    outFile << string(102, '-') << endl;        // Print line separator


    string line;
    while (getline(file, line)) {
        if (line == "---USER START---") {     // Check start of a user block

            string name, age, height, weight, username, password, sub, plan, end;

            // Read all user details
            getline(file, name);
            getline(file, age);
            getline(file, height);
            getline(file, weight);
            getline(file, username);
            getline(file, password);
            getline(file, sub);
            getline(file, plan);
            getline(file, end);

            // Write formatted user data to file
            outFile << setw(15) << getValue(name)
                << setw(10) << getValue(age)
                << setw(10) << getValue(height)
                << setw(10) << getValue(weight)
                << setw(15) << getValue(username)
                << setw(12) << getValue(sub)
                << setw(30) << getValue(plan) << endl;
        }
    }
    file.close();
    outFile.close();
    typeOut("Successful!", 30);
}

/*=================== FITNESS SYSTEM ====================*/
class FitnessSystem {
public:

    // Function to return workout plan using polymorphism
    WorkoutPlan* getPlan(string category) {
        if (category == "Underweight") return new BulkPlan();
        if (category == "Normal") return new MaintainPlan();
        return new CutPlan();
    }

    // Function to view workout plan
    void viewWorkout(User& u) {
        if (!u.customPlan.empty()) {

            cout << "\n--- Your Custom Workout Plan ---\n" << u.customPlan << endl;
            return;
        }
        // Get plan according to BMI category
        WorkoutPlan* plan = getPlan(u.category());
        plan->showPlan();
        delete plan;
    }

    void editPlan(User& u) {
        cout << "Enter your custom workout plan (single line): ";
        cin.ignore();
        getline(cin, u.customPlan);
        cout << "Custom workout plan saved." << endl;
    }

    void activateSubscription(User& u) {
        if (u.subscribed) cout << "Subscription already active." << endl;
        else {
            u.subscribed = true;
            cout << "Subscription activated!" << endl;
        }
    }

    void subscriptionStatus(User& u) {
        cout << (u.subscribed ? "Status: Active" : "Status: Not Subscribed") << endl;
    }

    void BMIcalc(User& u) {
        cout << "Your BMI: " << u.getBMI() << endl;
        cout << "Category: " << u.category() << endl;
    }

    void update(User& u) {
        cout << "Enter new height: "; cin >> u.height;
        cout << "Enter new weight: "; cin >> u.weight;
        cout << "Updated successfully." << endl;
    }

    void logExercise(User& u) {
        if (!u.subscribed) {
            cout << "Members only! Activate subscription first." << endl;
            return;
        }
        cout << "Enter exercise (type 'stop' to finish):\n";
        string ex;
        cin.ignore();
        while (true) {
            cout << "> ";
            getline(cin, ex);
            if (ex == "stop") break;
            ofstream log("exercise_log.txt", ios::app);
            log << u.username << ": " << ex << endl;
            cout << "Logged: " << ex << endl;
        }
    }
};

/*============================== MAIN ===============================*/
int main() {


    FitnessSystem system;     // System object
    User currentUser;        // Logged-in user
    int choice;
    bool loggedIn = false;



    // Login / Register loop
    while (!loggedIn) {
        cout << "--- Login / Register ---\n";
        cout << "1. Login\n2. Register\n3. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            string uname, pwd;
            cout << "Username: "; cin >> uname;
            cout << "Password: "; cin >> pwd;
            if (loadUser(uname, currentUser) && currentUser.password == pwd) {
                cout << "Login successful!\n\n";
                loggedIn = true;
            }
            else cout << "Incorrect username or password!\n";
        }
        else if (choice == 2) {
            User newUser;
            cout << "Enter name: "; cin >> newUser.name;
            cout << "Enter age: "; cin >> newUser.age;
            cout << "Enter height (m): "; cin >> newUser.height;
            cout << "Enter weight (kg): "; cin >> newUser.weight;
            cout << "Create username: "; cin >> newUser.username;
            cout << "Create password: "; cin >> newUser.password;
            updateUserFile(newUser);
            writeUsersReadable();
            typeOut("Registration successful! Login now.\n", 30);
        }
        else if (choice == 3) return 0;
    }

    bool run = true;
    while (run) {
        cout << "\n--- Main Menu ---\n";
        cout << "1. View Profile\n2. View Workout Plan\n3. Activate Subscription\n4. View Subscription Status\n";
        cout << "5. Edit Workout Plan\n6. BMI Calculator\n7. Update Weight/Height\n8. Log Exercise\n";
        cout << "9. Sign Out\n10. Exit\nChoose: ";
        cin >> choice;

        switch (choice) {
        case 1:
            currentUser.displayProfile();
            break;
        case 2:
            system.viewWorkout(currentUser);
            break;
        case 3:
            system.activateSubscription(currentUser);
            updateUserFile(currentUser);     // Save user
            writeUsersReadable();               // Update readable file
            break;
        case 4:
            system.subscriptionStatus(currentUser);
            break;
        case 5:
            system.editPlan(currentUser);
            updateUserFile(currentUser);
            writeUsersReadable();
            break;
        case 6:
            system.BMIcalc(currentUser);
            break;
        case 7:
            system.update(currentUser);
            updateUserFile(currentUser);
            writeUsersReadable();
            break;
        case 8:
            system.logExercise(currentUser);
            break;
        case 9:
            loggedIn = false;
            main();
            return 0;
        case 10:
            run = false;
            break;
        default:
            cout << "Invalid choice.\n";
        }
    }

    updateUserFile(currentUser); // Save before exit
    writeUsersReadable();
    return 0;
}
