#include "validation.h"

// Constructor for the class
// Sets up the entity types into a list and Relationship Table
validation::validation() {
	entityType = { "procedure","variables","assign","while","if","stmt","constant","stmtLst","prog_line" };
	table = RelTable();
}
unordered_map<string, string> validation::getDeclaration() {
	return synonym;
}
// Main validation method
bool validation::isValidQuery(string query) {
	if (!checkSynonym(query)) {
		cout << "Check Synonym fails\n";
		return false;
	}
	std::smatch m;
	std::regex e("(Select )[A-Za-z0-9]+");
	while (std::regex_search(query, m, e)) {
		if (!checkSelect(m[0].str().substr(7))) {
			cout << "Check regex_search fails\n";
			return false;
		}
		query = m.suffix().str();
	}
	//tuple
	/*e = ("(Select )[<]+[a-zA-Z0-9, ]+[>]+");
	while (std::regex_search(query, m, e)) {
		if (!checkTuple(m[0].str().substr(7))) {
			return false;
		}
		query = m.suffix().str();
	}*/

	// all ?(?,?)
	e = ("[a-zA-Z0-9*]+\\([a-zA-Z0-9\"_ ]+(,[a-zA-Z0-9\"_ ]+)+\\)");
	while (std::regex_search(query, m, e)) {
		if (!isRelationshipValid(m[0].str())) {
			cout << "check isRalationshipValid fails\n";
			return false;
		}
		query = m.suffix().str();
	}
	//with clause
/*	e = ("[A-Za-z0-9.#]+[ ]*[=]{1}[ ]*[A-Za-z0-9.\"]+");
	while (std::regex_search(query, m, e)) {
		if (!withClause(m[0].str())) {
			return false;
		}
		query = m.suffix().str();
	}*/
	return true;
}
// Check Multiple Synonym declaration is valid
bool validation::checkSynonym(string query) {
	std::smatch m;
	std::regex e("[a-zA-Z0-9]+[ a-zA-Z0-9,][^;]+[;]");
	while (std::regex_search(query, m, e)) {
		//cout << "m[0].str=" << m[0].str() << "_\n";
		if (m[0].str().substr(0, 6) == "Select" || m[0].str().substr(0, 6) == "select") return true;
		if (!isValidSynonym(m[0].str())) {
			return false;
		}
		query = m.suffix().str();
		//cout << "query = " << query << "\n";
	}
	return true;
}
// Check if one Synonym used is valid and adds to synonym into a table for reference later
// returns true if valid and false otherwise
bool validation::isValidSynonym(string entity) {
	string type = entity.substr(0, entity.find_first_of(' '));
	entity = entity.substr(entity.find_first_of(' ') + 1);
	if (entityType.find(type) == entityType.end()) {
		return false;
	}
	std::smatch m;
	std::regex e("[a-z0-9]+[^,; ]*");
	while (std::regex_search(entity, m, e)) {
		string temp = m[0].str();
		synonym.insert(std::pair<string, string>(temp, type));
		entity = m.suffix().str();
	}
	return true;
}
// Check Select clause in query is valid
// return : true if select used is valid and false otherwise
bool validation::checkSelect(string select) {
	if (synonym.find(select) != synonym.end()) {
		return true;
	}else if (select.compare("BOOLEAN") == 0) {
		return true;
	}
	return false;
}
// Check Select clause in query is valid for tuple
// return : true if select used is valid and false otherwise
bool validation::checkTuple(string select) {
	std::smatch m;
	std::regex e("[A-Za-z0-9]+");
	while (std::regex_search(select, m, e)) {
		if (!checkSelect(m[0].str())) {
			return false;
		}
		select = m.suffix().str();
	}
	return true;

}
// Check entity relationship those in form of ?(?,?)
// return : true if relationship used is valid and false otherwise
bool validation::isRelationshipValid(string relationship) {
	string type = table.getIndex(relationship);
	//cout << "relationship: _" << relationship << "\n";
	
	//remove spaces;
	string temp = "";
	for (int i = 0; i < relationship.length(); i++)
		if (relationship[i] != ' ') temp += relationship[i];
	relationship = temp;

	if (type.size() == 0) {
		type = patternCheck(relationship);
		if (type.size() == 0) {
			cout << "fails at type.size() == 0 \n";
			return false;
		}
	}
	//cout << "type of relationship: " << type << "\n";

	if (!checkNumOfArgument(type, relationship)) {
		cout << "Fails number of args\n";
		return false;
	}
	string arg1 = getArgument(relationship.substr(relationship.find('(') + 1, relationship.find(',') - relationship.find('(') - 1));
	string arg2;
	if (type.compare("passign") == 0) {
		//cout << "passign\n";
		arg2 = getArgumentAssign(relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1));
	}
	else {
		arg2 = getArgument(relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1));
	}

	//cout << "arg1 = " << arg1 << " arg2 = " << arg2 << "\n";
	if (arg1.size() == 0 || arg2.size() == 0) {
		cout << "Fails arg1/2.size()==0 \n";
		return false;
	}
	return table.isValidArgument(type, arg1, arg2);
}
// Finds if query uses pattern clause
// return : type of pattern clause used if valid otherwise empty string is returned
string validation::patternCheck(string query) {
	string type = query.substr(0, query.find_first_of('('));
	unordered_map<string, string>::const_iterator found = synonym.find(type);
	if (found == synonym.end()) {
		return string();
	}
	else {
		return "p" + found->second;
	}
}
// Check no. of argument in the relationship entity
// return : true if argument no. used is matches expected and false otherwise
bool validation::checkNumOfArgument(string type, string relationship) {
	int arg = std::count(relationship.begin(), relationship.end(), ',') + 1;
	return table.checkNumOfArgument(type, arg);
}
// Get the type argument for the query
// return : the type of argument the ref is
string validation::getArgument(string query) {
	std::string string = "\"";
	std::string underscore = "_";
	if (query.find(string) != std::string::npos) {
		return "string";
	}
	else if ((query.find(underscore) != std::string::npos) && (query.size() == 1)) {
		return "_";
	}
	else if (std::all_of(query.begin(), query.end(), ::isdigit)) {
		return "prog_line";
	}
	else if (synonym.find(query) != synonym.end()) {
		return synonym.find(query)->second;
	}
	else {
		return std::string();
	}
}
// Get the type argument for pattern assign
// return : the type of argument the ref is
string validation::getArgumentAssign(string query) {
	std::string string = "\"";
	//cout << "getting argument Assign of --" << query << "--\n";
	int arg = std::count(query.begin(), query.end(), '_');
	if ((arg == 1)) {
		if (query.size() == 1) {
			return "_";
		}
		else {
			return std::string();
		}
	}
	else if (arg == 2) {
		char first = query.at(0);
		char second = query.at(query.size() - 1);
		//cout << "first,second = " << first << second << "\n";
		if ((first == '_') && (second == '_')) {
			return "string";
		}
	}
	else if (query.find(string) != std::string::npos) {
		return "string";
	}
	return std::string();
}

// Checks if the query uses a valid with clause
// return : true is the with clause used is valid and false otherwise
/*bool validation::withClause(string query) {
	int first = isString(query.substr(0, query.find('=') - 1));
	int second = isString(query.substr(query.find('=') + 1));
	if ((first == second) && first != -1) {
		return true;
	}
	return false;
}

/Helper method for withClause, gets the type of argument in comparison
return : 1 if the ref is string
0 if the ref is int
-1 if the ref is neither string nor int
int validation::isString(string arg) {
	if (arg.find_first_not_of("0123456789") == std::string::npos) {
		return 0;
	}
	if (arg.find("\"") != std::string::npos) {
		return 1;
	}
	string syn = arg.substr(0, arg.find('.'));
	if (synonym.find(syn) != synonym.end()) {
		syn = synonym.find(syn)->second;
	}
	if ((arg.find(".stmt#") != string::npos) && ((syn.compare("stmt") == 0) || (syn.compare("assign") == 0) || (syn.compare("while") == 0) || (syn.compare("if") == 0))) {
		return 0;
	}
	if ((arg.find(".value") != string::npos) && (syn.compare("constant") == 0)) {
		return 0;
	}
	if ((arg.find(".procName") != string::npos) && (syn.compare("procedure") == 0)) {
		return 1;
	}
	if ((arg.find(".varName") != string::npos) && (syn.compare("variables") == 0)) {
		return 1;
	}
	return -1;
}*/