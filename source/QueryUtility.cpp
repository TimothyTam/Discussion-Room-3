#include "QueryUtility.h"


QueryUtility::QueryUtility(){
	
}

 QueryUtility::SynonymType QueryUtility::getSynonymType(string type) {
	if (type == "assign") return QueryUtility::SYNONYM_TYPE_ASSIGN;
	else if (type == "call")return QueryUtility::SYNONYM_TYPE_CALL;
	else if (type == "constant") return QueryUtility::SYNONYM_TYPE_CONSTANT;
	else if (type == "if") return QueryUtility::SYNONYM_TYPE_IF;
	else if (type == "procedure") return QueryUtility::SYNONYM_TYPE_PROCEDURE;
	else if (type == "prog_line") return QueryUtility::SYNONYM_TYPE_PROG_LINE;
	else if (type == "stmt") return QueryUtility::SYNONYM_TYPE_STMT;
	else if (type == "variable") return QueryUtility::SYNONYM_TYPE_VARIABLE;
	else if (type == "while") return QueryUtility::SYNONYM_TYPE_WHILE;
	else if (type == "stmtlst") return QueryUtility::SYNONYM_TYPE_STMTLST;

	else return QueryUtility::SYNONYM_TYPE_NULL;
}
string QueryUtility::getString(SynonymType type){
	switch (type) {
		case(QueryUtility::SYNONYM_TYPE_ASSIGN):
			return "assign"  ;
		case(QueryUtility::SYNONYM_TYPE_CALL):
			return "call";
		case(QueryUtility::SYNONYM_TYPE_CONSTANT):
			return "constant";
		case(QueryUtility::SYNONYM_TYPE_IF):
			return "if";
		case(QueryUtility::SYNONYM_TYPE_PROCEDURE):
			return "procedure";
		case(QueryUtility::SYNONYM_TYPE_PROG_LINE):
			return "prog_line";
		case(QueryUtility::SYNONYM_TYPE_STMT):
			return "stmt";
		case(QueryUtility::SYNONYM_TYPE_VARIABLE):
			return "variable";
		case(QueryUtility::SYNONYM_TYPE_WHILE):
			return "while";
		case(QueryUtility::SYNONYM_TYPE_STMTLST):
			return "stmtlst";
		default:
			return "";
	}
}