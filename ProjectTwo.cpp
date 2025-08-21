//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jazz Gonzalez
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>

// Data Model

struct Course {
	std::string number;
	std::string title;
	std::vector<std::string> prereqs;
};

struct Node {
	Course course;
	Node* left = nullptr;
	Node* right = nullptr;
	explicit Node(const Course& c) : course(c) {}
};

// Binary Search Tree

class CourseBST {
public:
	CourseBST() : root(nullptr) {}
	~CourseBST() { destroy(root); }

	void clear() { destroy(root); root = nullptr; }
	void insert(const Course& c) { root = insertRec(root, c); }

	const Course* find(const std::string& courseNum) const {
		Node* cur = root;
		while (cur) {
			if (courseNum == cur->course.number) return &cur->course;
			if (courseNum < cur->course.number) cur = cur->left;
			else								cur = cur->right;
		}
		return nullptr;
	}

	void printInOrder() const {
		if (!root) {
			std::cout << "No courses loaded.\n";
			return;
		}
		inOrder(root);
	}

private:
	Node* root;

	static Node* insertRec(Node* node, const Course& c) {
		if (!node) return new Node(c);
		if (c.number < node->course.number) node->left = insertRec(node->left, c);
		else								node->right = insertRec(node->right, c);
		return node;
	}

	static void inOrder(Node* node) {
		if (!node) return;
		inOrder(node->left);
		std::cout << node->course.number << ", " << node->course.title << '\n';
		inOrder(node->right);
	}

	static void destroy(Node* node) {
		if (!node) return;
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
};

// Helpers
// Trim leading whitespace
static inline void trim(std::string& s) {
	auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
	s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
}

// Normalize course IDs
static inline void toUpper(std::string& s) {
	for (char& ch : s) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

// CSV splitter that preserves empty fields
static std::vector<std::string> splitCSV(const std::string& line) {
	std::vector<std::string> out;
	std::string cur;
	for (char ch : line) {
		if (ch == ',') {
			trim(cur);
			out.push_back(cur);
			cur.clear();
		} else {
			cur.push_back(ch);
		}
	}
	trim(cur);
	out.push_back(cur);
	return out;
}

// Loading and validation
bool loadCoursesBST(const std::string& filename, CourseBST& bst) {
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		std::cout << "Error: Could not open file '" << filename << "'.\n";
		return false;
	}

	std::vector<Course> temp;
	std::unordered_set<std::string> ids;

	std::string line;
	int lineNo = 0;

	while (std::getline(fin, line)) {
		++lineNo;
		if (line.empty()) continue;

		auto tokens = splitCSV(line);

		if (tokens.size() < 2) {
			std::cout << "Format error (line " << lineNo
					  << "): fewer than 2 items (need course number, title).\n";
			continue;
		}

		Course c;
		c.number = tokens[0];
		c.title = tokens[1];

		trim(c.number);
		trim(c.title);
		toUpper(c.number);

		if (c.number.empty()) {
			std::cout << "Format error (line " << lineNo << "): empty course number.\n";
			continue;
		}
		if (c.title.empty()) {
			std::cout << "Format error (line " << lineNo << "}: empty course title.\n";
			continue;
		}

		//Remaining tokens are prereqs
		for (size_t i = 2; i < tokens.size(); ++i) {
			std::string p = tokens[i];
			trim(p);
			toUpper(p);
			if (!p.empty()) c.prereqs.push_back(std::move(p));
		}

		ids.insert(c.number);
		temp.push_back(std::move(c));
	}
	fin.close();

	if (temp.empty()) {
		std::cout << "No valid courses found in file.\n";
		return false;
	}

	bool hadErrors = false;
	bst.clear();

	for (const auto& c : temp) {
		for (const auto& p : c.prereqs) {
			if (ids.find(p) == ids.end()) {
				std::cout << "Error: prerequisite '" << p
						  << "' for course '" << c.number
						  << "' does not exist in file.\n";
				hadErrors = true;
			}
		}
		bst.insert(c);
	}

	std::cout << "Loaded " << temp.size() << " courses"
			  << (hadErrors ? " (with errors reported above)." : " successfully.")
			  << '\n';
	return true;
}

// Printing
// Print a specific course by number, including its title and each prereq's number and title
void printCourseWithPrereqs(const CourseBST& bst, const std::string& rawId) {
	std::string id = rawId;
	trim(id);
	toUpper(id);

	const Course* c = bst.find(id);
	if (!c) {
		std::cout << "Course not found: " << id << '\n';
		return;
	}

	std::cout << c->number << ", " << c->title << '\n';

	if (c->prereqs.empty()) {
		std::cout << "Prerequisites: None\n";
		return;
	}

	std::cout << "Prerequisites:\n";
	for (const auto& p : c->prereqs) {
		const Course* pc = bst.find(p);
		if (pc) std::cout << " " << pc->number << ", " << pc->title << '\n';
		else	std::cout << " " << p << " (info not found)\n";
	}
}

// Main
int main() {
	CourseBST bst;
	bool dataLoaded = false;

	while (true) {
		std::cout << "\n=== ABCU Course Advisor ===\n"
				  << "1. Load course data\n"
				  << "2. Print all courses (alphanumeric)\n"
				  << "3. Print a specific course\n"
				  << "9. Exit\n"
				  << "Select an option: ";

		int choice = 0;
		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(10'000, '\n');
			std::cout << "Invalid input. Please enter a number.\n";
			continue;
		}
		std::cin.ignore(10'000, '\n');

		if (choice == 1) {
			std::cout << "Enter filename: ";
			std::string filename;
			std::getline(std::cin, filename);
			trim(filename);

			if (filename.empty()) {
				std::cout << "Error: filename cannot be empty.\n";
				continue;
			}

			if (loadCoursesBST(filename, bst)) {
				dataLoaded = true;
			} else {
				dataLoaded = false;
			}
		} else if (choice == 2) {
			if (!dataLoaded) {
				std::cout << "Please load data first (Option 1).\n";
				continue;
			}

			bst.printInOrder();
		} else if (choice == 3) {
			if (!dataLoaded) {
				std::cout << "Please load data first (Option 1).\n";
				continue;
			}
			std::cout << "Enter course number: ";
			std::string id;
			std::getline(std::cin, id);
			printCourseWithPrereqs(bst, id);
		} else if (choice == 9) {
			std::cout << "Goodbye!\n";
			break;
		} else {
			std::cout << "Invalid option. Please choose 1, 2, 3, or 9.\n";
		}
	}

	return 0;
}
