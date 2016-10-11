1 - Procedure_1
procedure p;
Select p such that Modifies(p,"ghost")
Phantom
5000
2 - Procedure_2
procedure p;
Select p such that Uses(p,"mahou")
Magic
5000
3 - Procedure_3
procedure p;
Select p such that Modifies(p,"amazement")
Elements,Magic
5000
4 - Procedure_4
procedure p;
Select p such that Uses(p,"amazement")
Magic
5000
5 - Procedure_5
procedure p; variable v;
Select v such that Uses(p,v) with p.procName = "Fire"
wood,log,fire,match,water
5000
6 - Procedure_6
procedure p; variable v;
Select v such that Modifies(p,v) with p.procName = "Fire"
wood,log,match,fire
5000
7 - Call_stmt_1
variable v;
Select v such that Modifies(4,v)
wood,log,match,fire
5000
8 - Call_stmt_2
variable v;
Select v such that Uses(4,v)
wood,log,match,fire
5000
9 - Call_stmt_3
variable v;
Select v such that Modifies(2,v)
hydrogen,oxygen,water,nitrogen,others,air
5000
10 - Call_stmt_4
call callstmt;
Select callstmt with callstmt.procName = "Magic"
7
5000
11 - Call_stmt_5
call callstmt; constant c;
Select callstmt with callstmt.procName = "Priest" with c.value = 1
39,43
5000
12 - Call_stmt_5
call callstmt; constant c; variable v,v1,v2;
Select callstmt with callstmt.procName = "Priest" with c.value = 1 with v.varName = "confusement" with v1.varName = "soil" with v2.varName = "potion"
39,43
5000
13 - Call_stmt_6
call callstmt; constant c; variable v,v1,v2;
Select callstmt with callstmt.procName = "Priest" with c.value = 1 with v.varName = "confusement" with v1.varName = "soil" with v2.varName = "poison"
none
5000
