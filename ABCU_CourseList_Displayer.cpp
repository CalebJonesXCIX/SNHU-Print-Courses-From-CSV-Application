#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<algorithm>
#include <sstream>
using namespace std;

// === Creating the Course Object ===
class Course {
public:
	Course();
	Course(string courseId, string courseName);
	void setCourseId(string courseId);
	void setCourseName(string courseName);
	void setPreReq(string course);
	string getCourseId();
	string getCourseName();
	vector<string> getPreReqs();
private:
	string courseId;
	string courseName;
	vector<string> preReqs;
};

Course::Course() {

}
Course::Course(string courseId, string courseName)
{
	setCourseId(courseId);
	setCourseName(courseName);
}

void Course::setCourseId(string courseId)
{
	this->courseId = courseId;
}

void Course::setCourseName(string courseName)
{
	this->courseName = courseName;
}

void Course::setPreReq(string course)
{
	preReqs.push_back(course);
}

string Course::getCourseId()
{
	return courseId;
}

string Course::getCourseName()
{
	return courseName;
}

vector<string> Course::getPreReqs()
{
	return preReqs;
}

// Overload the '<' operator to allow the native std::sort to sort Course Objects by ID
bool operator<(Course& courseA, Course& courseB) {
	return courseA.getCourseId() < courseB.getCourseId();
}
// === End of Course Object ===

// Global Variables for program use
vector<vector<string>> fileContents;// Holds the initial file data 
vector<Course> courseVector;// Holds the constructed course objects from the file data

void readCSVFile(string csvFile) {
	// Open a file and add the contents to a vector of vectors of strings for each line of content

	vector<string> row;
	string line = "";
	string word = "";

	// Open and begin reading file
	fstream file(csvFile, ios::in);
	if (file.is_open())
	{
		fileContents.clear();// Clear fileContents to fill with new data
		// Gather each line of the file
		while (getline(file, line))
		{
			row.clear();// Clear past row data

			// Convert line to file object to parse with delimiters
			stringstream str(line);
			while (getline(str, word, ',')) {
				row.push_back(word);// Build row with comma separated values
			}

			// Validate file contents:
			if (row.size() < 2) {
				// Row does not contain both required information: CourseID and Course Name
				continue;
			}

			fileContents.push_back(row);// Add comma separated values to fileContents
		}
	}
	else {
		cout << "Error opening file" << endl;
	}

}

void buildCourseVector() {
	// Create course object with information from the file

	for (int i = 0; i < fileContents.size(); i++) {
		string courseId = fileContents[i][0];
		string courseName = fileContents[i][1];

		// Create the course for adding to course vector later
		Course tempCourse = Course(courseId, courseName);

		// Check if there are prerequisistes to add
		if (fileContents[i].size() > 2) {
			for (int j = 2; j <= fileContents[i].size() - 1; j++) {
				string preReq = fileContents[i][j];
				tempCourse.setPreReq(preReq);
			}
		}

		// Add new Course to Course Vector
		courseVector.push_back(tempCourse);
	}
}

void printCourseList() {
	// Sort the courseVector and print the newly ordered CourseId and CourseName

	sort(courseVector.begin(), courseVector.end());
	for (auto x : courseVector) {
		cout << x.getCourseId() << ", " << x.getCourseName() << endl;
	}
}

void printCourse(string courseId) {
	// Search for course from list by courseId

	transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);// Convert string to upper case since that's how the CourseId are formatted
	for (auto course : courseVector) {
		string id = course.getCourseId();

		// Check if id of current course is equal to the searched term
		if (id.compare(courseId) == 0) {
			// If true print the course information and any prerequisites
			cout << course.getCourseId() << ", " << course.getCourseName() << endl;

			// If there are prerequisites to print THEN
			if (course.getPreReqs().size() > 0) {
				cout << "Prerequisites: ";

				for (int i = 0; i < course.getPreReqs().size(); i++) {
					if (i != course.getPreReqs().size() - 1) {
						cout << course.getPreReqs().at(i) << ", ";
					}
					else {
						cout << course.getPreReqs().at(i) << endl;
					}
				}
			}
			return;// End method because search was successful
		}
	}
	// Else search was unsuccessful
	cout << "Could not find course: " << courseId << endl;

}

void displayMenu() {
	// Main navigation menu

	cout << "1. Load Data Structure." << endl;
	cout << "2. Print Course List." << endl;
	cout << "3. Print Course." << endl;
	cout << "9. Exit" << endl << endl;
	cout << "What would you like to do?" << endl;
}

int validateMenuInput(string choice) {
	// Receive menu choice as string validate it and attempt to convert to int
	
	// Seeking ONLY single digit input values
	if (choice.size() > 1) {
		return -1;
	}

	// Check if single input char is a digit
	if (isdigit(choice.at(0))) {
		return stoi(choice);
	}

	else {// Input was not a valid integer
		return -1;
	}

}
int main()
{
	// *** Modify if input file changes ***
	string csvFileName = "ABCU_Advising_Program_Input.csv";
	string userInput = "";
	bool exitLoop = false;

	while (exitLoop != true) {
		cout << endl;
		displayMenu();
		cin >> userInput;
		int menuSelection = validateMenuInput(userInput);

		switch (menuSelection)
		{
		case(1):// 1. Load Data Structure.
			cout << "Loading from file..." << endl;
			readCSVFile(csvFileName);
			buildCourseVector();
			cout << "Load Complete!" << endl;
			break;

		case(2):// 2. Print Course List.
			cout << "Here is a sample schedule:" << endl << endl;
			printCourseList();
			break;

		case(3):// 3. Print Course.
			cout << "What course would you like to know about?" << endl;
			cin >> userInput;
			printCourse(userInput);
			break;

		case(9):
			exitLoop = true;
			cout << "Thank you for using the course planner!" << endl;
			break;
		
		default:
			cout << userInput << " is not a valid option." << endl;
		}

		
		
	}
	return 0;
}