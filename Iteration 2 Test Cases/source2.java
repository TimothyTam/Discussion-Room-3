procedure Elements {
    number = 4;
    call Air;
    call Water;
    call Fire;
    call Earth;
    amazement = 10*10;
    call Magic;}

procedure Air {
    call Water;
    oxygen = 2;
    nitrogen = 7;
    others = 1;
    air = oxygen + nitrogen + others;}

procedure Water {
    hydrogen = 2;
    oxygen = 1;
    water = hydrogen * oxygen;}

procedure Fire {
    wood = 1;
    log = 1+2*3;
    match = 8;
    fire = wood * log;
    fire = fire + match - water;}

procedure Earth {
    grass = 2;
    soil = 7 - 1 + 2;
    earth = grass + soil;
    earth = earth + air + water;
    call Fire;}

procedure Magic {
    amazement = amazement + 1;
    mahou = 5;
    call Mage;
    call Magician;
    call Witch;
    call Wizard;
    confusement = 9;
    call Fire;
    call Water;}

procedure Mage {
    warlock = 1;
    sorcerer = 1;
    call Witch;
    call Wizard;
    call Priest;
    call Necromancer;}

procedure Magician {
    call Witch;
    call Wizard;
    call Priest;
    call Necromancer;
    potion = 3;}

procedure Witch {
    witchcraft = 1;
    call Water;
    call Fire;}

procedure Wizard {
    wizardry = 1;
    call Water;
    call Fire;}

procedure Priest {
    heart = 3;
    support = 2;}

procedure Necromancer {
    soul = 10;
    zombie = 2;
    cursed = 1;}

procedure Phantom {
    ghost = 10;}
