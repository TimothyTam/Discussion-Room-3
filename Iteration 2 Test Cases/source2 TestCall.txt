1 - Call_1
procedure q;
Select q such that Calls("Elements",q)
Air,Water,Fire,Earth,Magic
5000
2 - Call*_1
procedure q;
Select q such that Calls*("Elements",q)
Air,Water,Fire,Earth,Magic,Mage,Magician,Witch,Wizard,Priest,Necromancer
5000
3 - Call_2
procedure p;
Select p such that Calls(p,"Necromancer")
Mage, Magician
5000
4 - Call*_2
procedure p;
Select p such that Calls*(p,"Necromancer")
Mage, Magician, Elements, Magic
5000
5 - Call_3
procedure p,q;
Select p such that Calls(p,q) with q.procName = "Necromancer"
Mage, Magician
5000
6 - Call_4
procedure p,q;
Select p such that Calls(p,q) with q.procName = "Necromanger"
none
5000
7 - Call_Multiple_1
procedure p;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic")
Elements
5000
8 - Call_Multiple_2
procedure p;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"magic")
none
5000
9 - Call_Multiple_3
procedure p;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic") such that Calls(p,"Mage")
none
5000
10 - Call_Multiple_4
procedure p;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic") such that Calls*(p,"Mage")
Elements
5000
11 - Call_Multiple_5
procedure p; constant c;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic") such that Calls*(p,"Mage") with c.value = 10;
Elements
5000
12 - Call_Multiple_6
procedure p; constant c;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic") such that Calls*(p,"Mage") with c.value = 11;
none
5000
13 - Call_Multiple_7
procedure p; constant c,c1,c2,c3,c4;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic") such that Calls*(p,"Mage") with c.value = 10 with c1.value = 1 with c2.value = 2 with c3.value = 3 with c4.value = 4;
Elements
5000
14 - Call_Multiple_8
procedure p; constant c,c1,c2,c3,c4;
Select p such that Calls(p,"Air") such that Calls(p,"Fire") such that Calls(p,"Water") such that Calls(p,"Earth") such that Calls(p,"Magic") such that Calls*(p,"Mage") with c.value = 10 with c1.value = 1 with c2.value = 2 with c3.value = 3 with c4.value = 11;
none
5000
15 - Call_Multiple_9
procedure p,q,r,s,t;
Select p such that Calls(p,q) such that Calls(p,r) such that Calls(p,s) such that Calls(p,t) with q.procName = "Water" with r.procName = "Air" with s.procName = "Earth" with t.procName="Magic"
Elements
5000
