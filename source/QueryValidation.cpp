#include "QueryValidation.h"

// Constructor for the class
// Sets up the entity types into a list and Relationship Table
QueryValidation::QueryValidation() {
	entityType = { "procedure","stmtLst","stmt","assign","call","while","if","variable","constant","prog_line"};
	table = RelTable();
}
//returns the declaration list
unordered_map<string, QueryUtility::SynonymType> QueryValidation::getDeclaration() {
	return declarationList;
}
//returns the selectList as string
string QueryValidation::getSelect() {
	return selectList;
}
//returns the clauses 
vector<QueryUtility::ClauseType> QueryValidation::getClauseEnum() {
	return clauseEnum;
}
//returns the clauses
vector<vector<string>> QueryValidation::getClauseParam() {
	return clauseParam;
}

// Main QueryValidation method
bool QueryValidation::isValidQuery(string query) {
	using namespace std::regex_constants;
	std::smatch m;
	std::regex e("set", ECMAScript | icase);//set to case insensitive
	//Declaration
	std::string searchquery = query;
	e = ("[a-zA-Z0-9]+[ a-zA-Z0-9,][^;]+[;]");
	while (std::regex_search(searchquery, m, e)) {
		if(!checkDeclaration(m[0].str())){
			cout << "Check Declaration fails\n";
			return false;
		}
		searchquery = m.suffix().str();
	}

	//Select
	searchquery = query;
	e = ("(Select){1}[ ]*([A-Za-z0-9#]+|(BOOLEAN){1}|(<){1}( )*[A-Za-z0-9#]+( )*(,( )*[A-Za-z0-9#]+)*( )*(>){1})");
	while (std::regex_search(searchquery, m, e)) {
		if(!checkSelect(m[0].str())){
			cout << "Check Synonym fails\n";
			return false;
		}
		searchquery = m.suffix().str();
	}
	query = searchquery;
	
	while (1) {
		if ((query.size() == 0) || (query.find_first_not_of(" ") == std::string::npos)) {
			return true;
		}
		query = query.substr(query.find_first_not_of(" "));
		string word = query.substr(0,query.find(" "));
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		if (word == "such") {  //such that
			std::string next = query.substr(query.find(" ") + 1, 4);
			if (next != "that") { //not such that -> error
				return false;
			}else {
				e = ("(such that){1}( )*([A-Za-z0-9*]+[(]{1}[A-Za-z0-9,_\" ]+[)]{1})([ ]*(and)[ ]*[A-Za-z0-9*]+[(]{1}[A-Za-z0-9,_\" ]+[)]{1})*");
				while (std::regex_search(query, m, e)) {
					if (!isValidSuchThat(m[0].str())) {
						cout << "Check Such that fails\n";
						return false;
					} else {
						query = m.suffix().str();
						break;
					}
				}
			}
		} else if (word == "pattern") {	//pattern
			e = ("(pattern){1}([ ]*[A-Za-z0-9]+[(]{1}[A-Za-z0-9\",_ +\\-*]+[)])([ ]*(and)[ ]*[A-Za-z0-9]+[(]{1}[A-Za-z0-9\",_ +*\\-]+[)])*");
			while (std::regex_search(query, m, e)) {
				if (!isValidPattern(m[0].str())) {
					cout << "Check pattern fails\n";
					return false;
				} else {
					query = m.suffix().str();
					break;
				}
			}
		} else if (word == "with") {//with
			e = ("(with)([ ]*[A-Za-z0-9.#\"]+[ ]*=[ ]*[A-Za-z0-9.#\"]+)([ ]*(and){1}[ ]*[A-Za-z0-9.#\"]+[ ]*=[ ]*[A-Za-z0-9.#\"]+)*");
			while (std::regex_search(query, m, e)) {
				string temp = m[0].str();
				if (!isValidWith(m[0].str())) {
					cout << "Check With fails\n";
					return false;
				} else {
					query = m.suffix().str();
					break;
				}
			}
		}else {
			return false;
		}
	}
	return true;
}
// Check Multiple Synonym declaration is valid
bool QueryValidation::checkDeclaration(string declarations) {
	std::smatch m;
	std::regex e("[a-zA-Z0-9]+[ a-zA-Z0-9,][^;]+[;]");
	while (std::regex_search(declarations, m, e)) {
		//cout << "m[0].str=" << m[0].str() << "_\n";
		if (!isValidDeclaration(m[0].str())) {
			//cout << "not valid m[0].str()\n";
			return false;
		}
		declarations = m.suffix().str();
		//cout << "declaration = " << decalaration << "\n";
	}
	return true;
}
// Check if one declaration used is valid and adds to declaration into a table 
// returns true if valid and false otherwise
bool QueryValidation::isValidDeclaration(string decl) {
	string type = decl.substr(0, decl.find_first_of(' '));
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	decl = decl.substr(decl.find_first_of(' ') + 1);
	QueryUtility::SynonymType synType = QueryUtility::getSynonymType(type);
	if (synType == QueryUtility::SYNONYM_TYPE_NULL) {
		return false;
	}
	std::smatch m;
	std::regex e("[a-z0-9]+[^,; ]*");
	while (std::regex_search(decl, m, e)) {
		string temp = m[0].str();
		if (declarationList.find(temp) != declarationList.end()) {
			return false;
		}
			declarationList.insert(std::pair<string, QueryUtility::SynonymType>(temp, synType));
			decl = m.suffix().str();
		
	}
	return true;
}

// Check Select clause in query is valid
// return : true if select used is valid and false otherwise
bool QueryValidation::checkSelect(string select) {
	select = select.substr(6);
	select.erase(std::remove(select.begin(), select.end(), ' '), select.end());
	if (select.at(0) == '<') {
		//tuple
		return checkTuple(select);
	}else if (declarationList.find(select) != declarationList.end()) {
		//synonym
		selectList = select;
		return true;
	}else if (select.compare("BOOLEAN") == 0) {
		//BOOLEAN
		selectList = "BOOLEAN";
		return true;
	}
	return false;
}
// Check Select clause in query is valid for tuple
// return : true if select used is valid and false otherwise
bool QueryValidation::checkTuple(string select){
	std::smatch m;
	std::regex e("[A-Za-z0-9]+");
	string temp = select;
	while (std::regex_search(temp, m, e)) {
		if (declarationList.find(m[0].str()) == declarationList.end()) {
			return false;
		}
		temp = m.suffix().str();
	}
	selectList = select;
	return true;
}

bool QueryValidation::isValidSuchThat(string suchthat){
	std::smatch m;
	std::regex e("[a-zA-Z0-9*]+\\([a-zA-Z0-9\"_ ]+(,[a-zA-Z0-9\"_ ]+)+\\)");
	while (std::regex_search(suchthat, m, e)) {
		if (!isRelationshipValid(m[0].str())) {
			return false;
		}
		suchthat = m.suffix().str();
	}
	return true;
}
// Finds if query uses pattern clause
// return : type of pattern clause used if valid otherwise empty string is returned
bool QueryValidation::isValidPattern(string pattern) {
	string temp = "adsf";
	std::smatch m;
	std::regex e("[a-zA-Z0-9*]+\\([a-zA-Z0-9\"_ +*\\-]+(,[a-zA-Z0-9\"_ +*\\-]+)+\\)");
	while (std::regex_search(pattern, m, e)) {
		string next = getPatternType(m[0].str().substr(0, m[0].str().find('('))) + m[0].str().substr(m[0].str().find('('));
		//a(1,2) -> passign(1,2)
		if (!isRelationshipValid(next)) {
			return false;
		 }
		pattern = m.suffix().str();
	}
	return true;
}
string QueryValidation::getPatternType(string clause) {
	if (declarationList.find(clause) != declarationList.end()) {
		QueryUtility::SynonymType type = declarationList.find(clause)->second;
		switch (type) {
		case QueryUtility::SYNONYM_TYPE_ASSIGN:
			return "passign";
		case QueryUtility::SYNONYM_TYPE_IF:
			return "pif";
		case QueryUtility::SYNONYM_TYPE_WHILE:
			return "pwhile";
		}
	}
	return "";
 }
// Check entity relationship those in form of ?(?,?)
// return : true if relationship used is valid and false otherwise
bool QueryValidation::isRelationshipValid(string relationship) {
	relationship.erase(std::remove(relationship.begin(), relationship.end(), ' '), relationship.end());
	QueryUtility::ClauseType type = table.getIndex(relationship.substr(0,relationship.find('(')));
	//cout << "type of relationship: " << type << "\n";
	if (type == QueryUtility::CLAUSETYPE_NULL) {
		return false;
	}
	if (!table.checkNumOfArgument(type, relationship)) {
		cout << "Fails number of args\n";
		return false;
	}
	string param1 = relationship.substr(relationship.find('(') + 1, relationship.find(',') - relationship.find('(') - 1);
	string param2;

	string arg1 = getArgument(param1);
	relationship = relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1);
	string arg2;
	if (type == QueryUtility::CLAUSETYPE_PATTERN_IF) {
		//cout << "pattern if" << "\n"
		arg2 = relationship.substr(0, relationship.find(','));
		param2 = arg2;
		string arg3 = relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1);
		if(!((arg2 == "_") && (arg3 == "_"))) {
			return false;
		}
	}else if (type == QueryUtility::CLAUSETYPE_PATTERN_WHILE) {
		//cout << "pattern while" << "\n"
		arg2 = relationship.substr(0, relationship.find(','));
		param2 = arg2;
		if (!(arg2 == "_")) {
			return false;
		}
	} else if (type == QueryUtility::CLAUSETYPE_PATTERN_ASSIGN) {
		//cout << "pattern assign" << "\n"
		param2 = relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1);
		arg2 = getArgumentAssign(param2);
	} else {
		param2 = relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1);
		arg2 = getArgument(param2);
	}

	//cout << "arg1 = " << arg1 << " arg2 = " << arg2 << "\n";
	if (arg1.size() == 0 || arg2.size() == 0) {
		cout << "Fails arg1/2.size()==0 \n";
		return false;
	}
	if (table.isValidArgument(type, arg1, arg2)) {
		clauseEnum.push_back(type);
		vector<string> args = { param1,param2};
		if (type == QueryUtility::CLAUSETYPE_PATTERN_IF) {
			args.push_back("_");
		}
		clauseParam.push_back(args);
		return true;
	}
	return false;
}

// Get the type argument for the query
// return : the type of argument the ref is
string QueryValidation::getArgument(string query) {
	std::string string = "\"";
	std::string underscore = "_";
	if (query.find(string) != std::string::npos) {
		return "string";
	} else if ((query.find(underscore) != std::string::npos) && (query.size() == 1)) {
		return "_";
	} else if (std::all_of(query.begin(), query.end(), ::isdigit)) {
		return "prog_line";
	} else if (declarationList.find(query) != declarationList.end()) {
		return QueryUtility::getString(declarationList.find(query)->second);
	} else {
		return std::string();
	}
}
// Get the type argument for pattern assign
// return : the type of argument the ref is
string QueryValidation::getArgumentAssign(string query) {
	std::string string = "\"";
	//cout << "getting argument Assign of --" << query << "--\n";
	int arg = std::count(query.begin(), query.end(), '_');
	if ((arg == 1)) {
		if (query.size() == 1) {
			return "_";
		} else {
			return std::string();
		}
	} else if (arg == 2) {
		char first = query.at(0);
		char second = query.at(query.size() - 1);
		//cout << "first,second = " << first << second << "\n";
		if ((first == '_') && (second == '_')) {
			return "string";
		}
	} else if (query.find(string) != std::string::npos) {
		return "string";
	}
	return std::string();
}
bool QueryValidation::isValidWith(string withs){
	std::smatch m;
	std::regex e("[A-Za-z0-9.\"#]+( )*=( )*[A-Za-z0-9.\"#]+");
	while (std::regex_search(withs, m, e)) {
		if (!checkWithClause(m[0].str())) {
			return false;
		}
		withs = m.suffix().str();
	}
	return true;
}
bool QueryValidation::checkWithClause(string with){
	with.erase(std::remove(with.begin(), with.end(), ' '), with.end());
	int first = isString(with.substr(0, with.find('=')));
	int second = isString(with.substr(with.find('=') + 1));
	if ((first == second) && first != -1) {
		QueryUtility::ClauseType type = QueryUtility::ClauseType::CLAUSETYPE_WITH;
		clauseEnum.push_back(type);
		vector<string> param = { with.substr(0, with.find('=')) ,with.substr(with.find('=') + 1) };
		clauseParam.push_back(param);
		return true;
	}
	return false;
}
//Helper method for withClause, gets the type of argument in comparison
//return : 1 if the ref is string
//0 if the ref is int
//-1 if the ref is neither string nor int
int QueryValidation::isString(string arg) {
	//int
	if (arg.find_first_not_of("0123456789") == std::string::npos) {
		return 0;
	}
	//string
	if (arg.find("\"") != std::string::npos) {
		return 1;
	}
	string syn = arg.substr(0, arg.find('.'));
	if (declarationList.find(syn) != declarationList.end()) {
		syn = QueryUtility::getString(declarationList.find(syn)->second);
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
}