#include "QueryValidation.h"

// Constructor for the class
// Sets up the entity types into a list and Relationship Table
QueryValidation::QueryValidation() {
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
	query = regex_replace(query, regex("\\s+"), " "); // replace all whitespaces with space
	smatch m;
	regex decl("[a-zA-Z0-9]+[ a-zA-Z0-9,][^;]+[;]");
	//Declaration
	string searchquery = query;
	int lastindex = query.find_last_of(";") +1;
	if (!checkDeclaration(query.substr(0, lastindex))) {
		return false;
	}
	query = query.substr(lastindex);
//	while (regex_search(searchquery, m, decl)) {
		//if (!checkDeclaration(m[0].str())) {
			//cout << "Check Declaration fails\n";
		//	return false;
		//}
	//	searchquery = m.suffix().str();
//	}
	int end;
	//Select 
	query = query.substr(query.find_first_not_of(" "));
	string selectWord = query.substr(0, 6); //check if "select"
	transform(selectWord.begin(), selectWord.end(), selectWord.begin(), ::tolower);
	if (selectWord != "select") {
		return false;
	}
	end = query.find_first_not_of(' ', 6);
	query = query.substr(end);
	if (query.at(0) == '<') {
		int endIndex = query.find(">");
		if (!checkTuple(query.substr(0,endIndex+1))) {
			cout << "Check Tuple fails\n";
			return false;
		}
		query = query.substr(endIndex+1);
	}
	if (selectList.length() == 0) {
		string clause;
//		int index = query.find_first_of(" ");
		clause = query;
		transform(clause.begin(), clause.end(), clause.begin(), ::tolower);
		int index = getIndex(clause.find("such"), clause.find("pattern"), clause.find("with"));
		if (index == string::npos) {
			clause = query;
			end = query.length() - 1;
		}
		else {
			end = index;
			clause = query.substr(0, index);
		}
		if (clause.find(".") != string::npos) {
			if (!checkattrRef(clause)) {
				cout << "Check attrRef Fails";
				return false;
			}
			query = query.substr(clause.length());
		}
		else {
			if (!checkSelect(clause)) {
				cout << "Check Synonym fails\n";
				return false;
			}
			query = query.substr(clause.length());
		}
	}
	
/*
	//attref
	bool attref = false;
	std::regex sela("(Select){1}[ ]*([a-zA-Z0-9]+(\\.){1}[a-zA-Z#]+)", ECMAScript | icase); 
	while (regex_search(searchquery, m, sela)) {
		attref = true;
		if (!checkattrRef(m[0].str().substr(6))) {
			cout << "Check attrRef Fails";
			return false;
		}
		searchquery = m.suffix().str();
	}
	if (!attref) {
		searchquery = query;
		//tuple
		bool tuple = false;
		std::regex selt("(Select){1}[ ]*((<){1}( )*([A-Za-z0-9#]|([a-zA-Z0-9]+(\\.){1}[a-zA-Z#]+))+( )*(,( )*([\.A-Za-z0-9#]+|([a-zA-Z0-9]+(\\.){1}[a-zA-Z#]+)))*( )*(>){1})", ECMAScript | icase);
		while (regex_search(searchquery, m, selt)) {
			tuple = true;
			if (!checkTuple(m[0].str().substr(6))) {
				cout << "Check Tuple fails\n";
				return false;
			}
			searchquery = m.suffix().str();
		}
		if (!tuple) {
			searchquery = query;
			//syn, boolean
			std::regex sel("(Select){1}[ ]*([A-Za-z0-9#]+|(BOOLEAN){1})", ECMAScript | icase);
			while (regex_search(searchquery, m, sel)) {
				if (!checkSelect(m[0].str().substr(6))) {
					cout << "Check Synonym fails\n";
					return false;
				}
				searchquery = m.suffix().str();
			}
		}
	}
	*/
	if (selectList.length() == 0) {
		return false;
	}
	//Clauses
	//query = searchquery;
	if ((query.length() == 0) || query.find_first_not_of(" ") == string::npos) {
		return true;
	}
	while (1) {
		bool breakTrue = false;
		if ((query.size() == 0) || (query.find_first_not_of(" ") == string::npos)) {
			return true;
		}
		query = query.substr(query.find_first_not_of(" "));
		string word = query.substr(0, query.find(" "));
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		if (word == "such") {  //such that
			string next = query.substr(query.find(" ") + 1, 4);
			transform(next.begin(), next.end(), next.begin(), ::tolower);
			if (next != "that") { //not such that -> error
				return false;
			}
			else {
				int index = getIndex(query.find("such",1), query.find("pattern"), query.find("with"));
				string temp = query.substr(0, index);
				if (!isValidSuchThat(temp)) {
					cout << "Check Such that fails\n";
					return false;
				}
				else {
					query = query.substr(temp.length());
					breakTrue = true;
				}
				if (!breakTrue) {
					return false;
				}
			}
		}
		else if (word == "pattern") {	//pattern
			int index = getIndex(query.find("such"), query.find("pattern", 1), query.find("with"));
			string temp = query.substr(0,index);
			if (!isValidPattern(temp)) {
				cout << "Check pattern fails\n";
				return false;
			}
			else {
				query = query.substr(temp.length());
				breakTrue = true;
			}
			if (!breakTrue) {
				return false;
			}
		}
		else if (word == "with") {//with
			int index = getIndex(query.find("such"), query.find("pattern"), query.find("with",1));
			string temp = query.substr(0, index);
			if (index != string::npos) {
				temp = temp.substr(0, index);
			}
			if (!isValidWith(temp)) {
				cout << "Check With fails\n";
				return false;
			}
			else {
				query = query.substr(temp.length());
				breakTrue = true;
			}
			if (!breakTrue) {
				return false;
			}
		}
		else { //spelling error
			return false;
		}
	}
	return true;
}
int QueryValidation::getIndex(int suchindex, int patternindex, int withindex) {
	int index = string::npos;
	if ((suchindex == string::npos)) {
		if (patternindex == string::npos) {
			if (withindex == string::npos) {
				index = string::npos;
			}
			else {
				index = withindex;
			}
		}
		else {
			if (withindex == string::npos) {
				index = patternindex;
			}
			else {
				index = (withindex < patternindex) ? withindex : patternindex;
			}
		}
	}
	else if (patternindex == string::npos) {
		if (withindex == string::npos) {
			index = suchindex;
		} else {
			index = (suchindex < withindex) ? suchindex : withindex;
		}
	}
	else { //patternindex present
		if (withindex == string::npos) {
			index = (suchindex < patternindex) ? suchindex : patternindex;
		}
		else {
			index = (suchindex < patternindex) ? suchindex : patternindex;
			index = (index < withindex) ? index : withindex;
		}
	}
	return index;
}

// Check Multiple Synonym declaration is valid
bool QueryValidation::checkDeclaration(string declarations) {
	smatch m;
	regex e("[a-zA-Z0-9]+[ a-zA-Z0-9,][^;]+[;]");
	while (regex_search(declarations, m, e)) {
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
	transform(type.begin(), type.end(), type.begin(), ::tolower);
	decl = decl.substr(decl.find_first_of(' ') + 1);
	QueryUtility::SynonymType synType = QueryUtility::getSynonymType(type);
	if (synType == QueryUtility::SYNONYM_TYPE_NULL) {
		return false;
	}
	smatch m;
	regex e("[a-z0-9]+[^,; ]*");
	while (regex_search(decl, m, e)) {
		string temp = m[0].str();
		if (declarationList.find(temp) != declarationList.end()) { //not used for others types
			return false;
		}
		declarationList.insert(std::pair<string, QueryUtility::SynonymType>(temp, synType));
		decl = m.suffix().str();
		int find = decl.find_first_not_of(" ,");
		int find1 = decl.find_first_of(";,");
		//0<find1 < find
		if ((find1 > find) || (find1 < 0)) {
			return false;
		}
	}
	return true;
}

// Check Select clause in query is valid
// return : true if select used is valid and false otherwise
bool QueryValidation::checkSelect(string select) {
	select.erase(remove(select.begin(), select.end(), ' '), select.end());
	if (declarationList.find(select) != declarationList.end()) { //synonym
		selectList = select;
		return true;
	} else {
		transform(select.begin(), select.end(), select.begin(), ::tolower);
		if (select.compare("boolean") == 0) { //BOOLEAN
			selectList = "BOOLEAN";
			return true;
		}
	}
	return false;
}

bool QueryValidation::checkattrRef(string select) {
	select.erase(remove(select.begin(), select.end(), ' '), select.end());
	attrName attr = stringToAttrName(select);
	string syn = select.substr(0, select.find("."));
	if (declarationList.find(syn) == declarationList.end()) {
		return false;
	}
	QueryUtility::SynonymType type = declarationList.at(syn);
	// procName, varName, value, stmtNo, none
	switch (attr) {
	case procName://procedure or call
	{
		if (type == QueryUtility::SYNONYM_TYPE_PROCEDURE) {
			selectList = syn;
			return true;
		}
		else if (type == QueryUtility::SYNONYM_TYPE_CALL) { 
			selectList = select;
			return true;
		}
		break;
	}
	case varName: //variable
	{
		if (type == QueryUtility::SYNONYM_TYPE_VARIABLE) {
			selectList = syn;
			return true;
		}
		break;
	}
	case value: // constant
		if (type == QueryUtility::SYNONYM_TYPE_CONSTANT) {
			selectList = syn;
			return true;
		}
		break;
	case stmtNo: //assign, stmt,call,if,while
		if ((type == QueryUtility::SYNONYM_TYPE_ASSIGN) || (type == QueryUtility::SYNONYM_TYPE_STMT) ||
			(type == QueryUtility::SYNONYM_TYPE_CALL) || (type == QueryUtility::SYNONYM_TYPE_IF) ||
			(type == QueryUtility::SYNONYM_TYPE_WHILE) ) {
			selectList = syn;
			return true;
		}
		break;
	case none:
		break;
	}
	return false;
}
// Check Select clause in query is valid for tuple
// return : true if select used is valid and false otherwise
bool QueryValidation::checkTuple(string select) {
	smatch m;
	regex e("[A-Za-z0-9\\.#]+");
	select.erase(remove(select.begin(), select.end(), ' '), select.end());
	string temp = select;
	select = "<";
	while (regex_search(temp, m, e)) {
		if (declarationList.find(m[0].str()) == declarationList.end()) {
			if (!checkattrRef(m[0].str())) {
				return false;
			}
			select = select + selectList + ",";
		} else {
			select = select +  m[0].str() + ",";
		}
		temp = m.suffix().str();
	}
	select = select.substr(0,select.length() - 1);
	selectList =  select + ">";
	return true;
}

// Check if such that clause is valid, able to handle clauses with 'and'
// return true if such that clause valid and false otherwise
bool QueryValidation::isValidSuchThat(string suchthat) {
	smatch m;
	regex e("[a-zA-Z0-9*]+[ ]*\\([a-zA-Z0-9\"_ ]+(,[a-zA-Z0-9\"_ ]+)+\\)");
	while (regex_search(suchthat, m, e)) {
		if (!isRelationshipValid(m[0].str())) {
			return false;
		}
		suchthat = m.suffix().str();
	}
	suchthat.erase(remove(suchthat.begin(), suchthat.end(), ' '), suchthat.end());
	if(suchthat.length() !=0){
		return false;
	}
	return true;
}

// Check if pattern used is valid, able to handle usage of 'and' 
// return true if the pattern is valid and false otherwise
bool QueryValidation::isValidPattern(string pattern) {
	smatch m;
	regex e("[a-zA-Z0-9*]+[ ]*\\([a-zA-Z0-9\"_ +*\\-]+(,[a-zA-Z0-9\"_ +*\\-()]+)+\\)");
	while (regex_search(pattern, m, e)) {
		string temp = m[0].str();
		string extra = "";
		if (temp.find(" and ") != string::npos) {
			extra = temp.substr(temp.find("and"));
			temp = temp.substr(0, temp.find("and"));
		}

		string next = getPatternType(temp.substr(0, temp.find('('))) + temp.substr(temp.find('('));
		//a(1,2) -> passign(1,2)
		if (!isRelationshipValid(next)) {
			return false;
		}
		pattern = extra + m.suffix().str();
	}
	pattern.erase(remove(pattern.begin(), pattern.end(), ' '), pattern.end());
	if (pattern.length() != 0) {
		return false;
	}
	return true;
}
// Finds the type of pattern used
// returns the type of pattern used, assign, if or while. 
// Empty string returned if it is not a valid pattern
string QueryValidation::getPatternType(string clause) {
	clause.erase(remove(clause.begin(), clause.end(), ' '), clause.end());
	if (declarationList.find(clause) != declarationList.end()) {
		patternSyn = clause;
		QueryUtility::SynonymType type = declarationList.find(clause)->second;
		switch (type) {
		case QueryUtility::SYNONYM_TYPE_ASSIGN:
			return "passign";
		case QueryUtility::SYNONYM_TYPE_IF:
			return "pif";
		case QueryUtility::SYNONYM_TYPE_WHILE:
			return "pwhile";
		default:
			return string();
		}
	}
	return string();
}
// Check entity relationship of such that and pattern clauses (one clause)
// return true if relationship used is valid and false otherwise
bool QueryValidation::isRelationshipValid(string relationship) {
	relationship.erase(remove(relationship.begin(), relationship.end(), ' '), relationship.end());
	QueryUtility::ClauseType type = table.getIndex(relationship.substr(0, relationship.find('(')));
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
	relationship = relationship.substr(relationship.find(',') + 1, relationship.find_last_of(')') - relationship.find(',') - 1);
	string arg2;
	switch (type) {
	case QueryUtility::CLAUSETYPE_PATTERN_IF:
	{//cout << "pattern if" << "\n"
		arg2 = relationship.substr(0, relationship.find(','));
		param2 = arg2;
		string arg3 = relationship.substr(relationship.find(',') + 1, relationship.length() - relationship.find(',') - 1);
		if (!((arg2 == "_") && (arg3 == "_"))) {
			return false;
		}
		break;
	}
	case QueryUtility::CLAUSETYPE_PATTERN_WHILE:
	{	//cout << "pattern while" << "\n"
		arg2 = relationship.substr(0, relationship.find(','));
		param2 = arg2;
		if (!(arg2 == "_")) {
			return false;
		}
		break;
	}
	case QueryUtility::CLAUSETYPE_PATTERN_ASSIGN:
	{	//cout << "pattern assign" << "\n"
		param2 = relationship.substr(relationship.find(',') + 1, relationship.length() - relationship.find(',') - 1);
		arg2 = getArgumentAssign(param2);
		break;
	}
	default:
	{	param2 = relationship.substr(relationship.find(',') + 1, relationship.find(')') - relationship.find(',') - 1);
	arg2 = getArgument(param2);
	break;
	}
	}
	//cout << "arg1 = " << arg1 << " arg2 = " << arg2 << "\n";
	if (arg1.size() == 0 || arg2.size() == 0) {
		cout << "Fails arg1/2.size()==0 \n";
		return false;
	}
	if (table.isValidArgument(type, arg1, arg2)) {
		clauseEnum.push_back(type);
		vector<string> args;
		switch (type) {
		case QueryUtility::CLAUSETYPE_PATTERN_ASSIGN:
			args = { patternSyn,param1,param2 };

			break;
		case QueryUtility::CLAUSETYPE_PATTERN_IF:
			args = { patternSyn,param1,param2,"_" };
			break;
		case QueryUtility::CLAUSETYPE_PATTERN_WHILE:
			args = { patternSyn,param1,param2 };
			break;
		default:
			args = { param1,param2 };
			break;
		}
		clauseParam.push_back(args);
		return true;
	}
	return false;
}

// Get the type argument for the query
// return the type of argument the ref is
string QueryValidation::getArgument(string query) {
	string invertedComma = "\"";
	string underscore = "_";
	if (query.find(invertedComma) != string::npos) {
		return "string";
	}
	else if ((query.find(underscore) != string::npos) && (query.size() == 1)) {
		return "_";
	}
	else if (std::all_of(query.begin(), query.end(), ::isdigit)) {
		return "prog_line";
	}
	else if (declarationList.find(query) != declarationList.end()) {
		return QueryUtility::getString(declarationList.find(query)->second);
	}
	else {
		return string();
	}
}
// Get the type argument for pattern assign
// return : the type of argument the ref is
string QueryValidation::getArgumentAssign(string query) {
	string invertedComma = "\"";
	//cout << "getting argument Assign of --" << query << "--\n";
	int arg = count(query.begin(), query.end(), '_');
	if ((arg == 1)) {
		if (query.size() == 1) {
			return "_";
		}
		else {
			return string();
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
	else if (query.find(invertedComma) != string::npos) {
		return "string";
	}
	return string();
}
// Check if the with clause used is valid, able to handle 'and'
// returns true if valid and false otherwise
bool QueryValidation::isValidWith(string withs) {
	smatch m;
	regex e("[A-Za-z0-9.\"# ]+( )*=( )*[A-Za-z0-9.\"# ]+");
	withs = withs.substr(4);
	while (regex_search(withs, m, e)) {
		string temp = m[0].str();
		string extra = string();
		if (temp.find(" and ") != string::npos) {
			extra = temp.substr(temp.find("and")+3);
			temp = temp.substr(0,temp.find("and"));
		}
		if (!checkWithClause(temp)) {
			return false;
		}
		withs = extra + m.suffix().str();

	}
	if (withs.length() != 0) {
		return false;
	}
	return true;
}
// Check if the with clause used is valid (only 1 clause)
// return true if the with clause is valid and false otherwise
bool QueryValidation::checkWithClause(string with) {
	with.erase(remove(with.begin(), with.end(), ' '), with.end());
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
	if (arg.find_first_not_of("0123456789") == string::npos) {//int
		return 0;
	}
	if (arg.find("\"") != string::npos) { //string
		int counter = std::count(arg.begin(), arg.end(), '\"');
		return (counter==2)? 1 : -1;
	}
	QueryUtility::SynonymType syn;
	//syn
	if (arg.find('.') == string::npos) {
		if (declarationList.find(arg) != declarationList.end()) {
			syn = declarationList.find(arg)->second;
			if (syn == QueryUtility::SYNONYM_TYPE_PROG_LINE) {
				return 0;
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	//attrRef
	string synonym = arg.substr(0, arg.find('.'));
	if (declarationList.find(synonym) != declarationList.end()) {
		syn = declarationList.find(synonym)->second;
	}
	else {
		return -1;
	}
	QueryValidation::attrName name = stringToAttrName(arg);
	switch (name) {
	case none:
		return -1;
	case stmtNo:
		if ((syn == QueryUtility::SYNONYM_TYPE_STMT) || (syn == QueryUtility::SYNONYM_TYPE_CALL) || (syn == QueryUtility::SYNONYM_TYPE_ASSIGN) || (syn == QueryUtility::SYNONYM_TYPE_WHILE) || (syn == QueryUtility::SYNONYM_TYPE_IF)) {
			return 0;
		}
	case value:
		if (syn == QueryUtility::SYNONYM_TYPE_CONSTANT) {
			return 0;
		}
	case procName:
		if (syn == QueryUtility::SYNONYM_TYPE_PROCEDURE || syn == QueryUtility::SYNONYM_TYPE_CALL) {
			return 1;
		}
	case varName:
		if (syn == QueryUtility::SYNONYM_TYPE_VARIABLE) {
			return 1;
		}
	default:
		return -1;
	}
}
// Converts the attrName use in query to attrName(enum)
// returns enum of attrName
QueryValidation::attrName QueryValidation::stringToAttrName(string clause) {
	if (clause.find(".stmt#") != string::npos) {
		return stmtNo;
	}
	else if (clause.find(".value") != string::npos) {
		return value;
	}
	else if (clause.find(".procName") != string::npos) {
		return procName;
	}
	else if (clause.find(".varName") != string::npos) {
		return varName;
	}
	else {
		return none;
	}
}