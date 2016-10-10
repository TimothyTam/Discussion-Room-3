#pragma once
#include <string>
using namespace std;

class QueryUtility {
	
public:
	QueryUtility();
	enum ClauseType {
		CLAUSETYPE_MODIFIES,
		CLAUSETYPE_USES,
		CLAUSETYPE_FOLLOWS,
		CLAUSETYPE_FOLLOWS_STAR,
		CLAUSETYPE_PARENT,
		CLAUSETYPE_PARENT_STAR,
		CLAUSETYPE_CALLS,
		CLAUSETYPE_CALLS_STAR,
		CLAUSETYPE_NEXT,
		CLAUSETYPE_NEXT_STAR,
		CLAUSETYPE_AFFECTS,
		CLAUSETYPE_AFFECTS_STAR,

		CLAUSETYPE_PATTERN_ASSIGN,
		CLAUSETYPE_PATTERN_IF,
		CLAUSETYPE_PATTERN_WHILE,

		CLAUSETYPE_WITH,				// default
		CLAUSETYPE_WITH_SYNONYM,
		CLAUSETYPE_WITH_INT,
		CLAUSETYPE_WITH_PROCNAME,
		CLAUSETYPE_WITH_VARNAME,
		CLAUSETYPE_WITH_VALUE,
		CLAUSETYPE_WITH_STMTNO,
		CLAUSETYPE_WITH_STRING,

		CLAUSETYPE_NULL
	};
	enum SynonymType {
		SYNONYM_TYPE_ASSIGN,
		SYNONYM_TYPE_BOOLEAN,//not design entity 
		SYNONYM_TYPE_CALL,
		SYNONYM_TYPE_CONSTANT,
		SYNONYM_TYPE_IF,
		SYNONYM_TYPE_PROCEDURE,
		SYNONYM_TYPE_PROG_LINE,
		SYNONYM_TYPE_STMT,
		SYNONYM_TYPE_VARIABLE,
		SYNONYM_TYPE_WHILE,
		SYNONYM_TYPE_STMTLST,

		SYNONYM_TYPE_NULL
	};
	enum ParamType {
		PARAMTYPE_SYNONYM,
		PARAMTYPE_ENT_NAME,
		PARAMTYPE_PATTERN_STRING_EXACT,
		PARAMTYPE_PATTERN_STRING_LEFT_OPEN,
		PARAMTYPE_PATTERN_STRING_RIGHT_OPEN,
		PARAMTYPE_PATTERN_STRING_BOTH_OPEN,
		PARAMTYPE_PLACEHOLDER,

		PARAMTYPE_NULL
	};
	public :
		static SynonymType getSynonymType(string type);
		static string getString(SynonymType type);
};