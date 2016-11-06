import os, subprocess, sys

AUTOTESTER_PATH = "..\Release\AutoTester.exe"
# AUTOTESTER_PATH = "..\Debug\AutoTester.exe"

def printHelp():
    print()
    print("Usage 1: python TestTimeout.py")
    print("   => Test all test cases in all the folders")
    print("*Note: it will look for AutoTester.exe in Release folder")
    

    print("Usage 2: python TestTimeout.py testFolder")
    print("   => Test a specific folder")
    print("*Example: python TestTimeout.py iter3_Test1")

    print()
    print("Usage 3: python TestTimeout.py testcase [folder] [query_file] [keyword]")
    print("   => Run the query in [folder]/[query_file].txt that contains the [keyword]")
    print("*Note: It will only run the first query containing that keyword")
    print("*Example: python TestTimeout.py testcase iter1_Test1 queries1 \"33 - \"")

def runCommand(command):
    proc = subprocess.Popen(command,shell=True,stdout=subprocess.PIPE)

    (output,err) = proc.communicate()
    return output

def getAttr(attr, string):
    startIndex = string.find("<"+attr+">")
    endIndex = string.find("</"+attr+">")
    return string[startIndex+len(attr)+2:endIndex]

def runTestFolder(dirPath, printDetails):
    
    files = os.listdir(dirPath)
    
    files = list( filter( lambda x: x.find("source.txt")==-1 ,files) )
    print("_______For SIMPLE source in folder " + dirPath + ":_________________________")

    for f in files:
        if f=='source.txt':
            continue
        print("++++ " + f + " : ")
        outfile = "output__" + dirPath[2:] + "__" + f[:-4] + ".xml"
        output = runCommand( ("%s %s %s " + outfile) %( AUTOTESTER_PATH, dirPath + "\source.txt", dirPath + "\\" + f)).decode("utf-8")
        f2 = open(outfile, encoding="utf-8")
        
        output = f2.read()

        queryResults = output.split("</query>")[:-1]

        queryCount = len(queryResults)
        correctCount = 0
        timeoutCount = 0
        maxTime = -1.1
        maxId = 0
        timeOut = []

        for index in range(0,queryCount):

            idIndex = queryResults[index].find("<id")
            idBracketIndex = queryResults[index].find(">",idIndex)
            bracketIndex = queryResults[index].find("<",idBracketIndex)

            queryId = queryResults[index][idBracketIndex+1:bracketIndex]

            if queryResults[index].find("<passed/>") == -1:
                if queryResults[index].find("<timeout/>") != -1:
                    timeOut.append(queryId)
                    timeoutCount += 1
                    continue
            else:
                correctCount += 1

            thisTime = float( getAttr("time_taken",queryResults[index]) )
            if (maxTime < thisTime):
                maxTime = thisTime
                maxId = queryId
                
        #print("Result:" + str(timeoutCount) + "/" + str(queryCount) + " TIMEOUT")
        print("      Queries with TIMEOUT: " + str(timeOut) + "  ( " + str(timeoutCount) + " out of " + str(queryCount) + " queries )" )
        print("      Max time taken (that is not TimeOut): " + str(maxTime) + " by query " + str(maxId))
        print()

try:
    # if len(sys.argv)>1:
    #     if (sys.argv[1]=="Debug" or sys.argv[1]=="debug"):
    #         AUTOTESTER_PATH = "..\Debug\AutoTester.exe" 

    if not os.path.isfile(AUTOTESTER_PATH):
        print("AutoTester.exe does not exist in " + AUTOTESTER_PATH)


    if len(sys.argv)<=3:
        if len(sys.argv)>=2:
            printDetails = True
            if len(sys.argv)==3:
                printDetails = False
            
            runTestFolder(sys.argv[1], printDetails)
            sys.exit()

        for currentDir, dirs, afiles in os.walk("."):
            for dir in dirs:
                dirPath = os.path.join(currentDir,dir)
                runTestFolder(dirPath, True)
                
                # output = proc.stdout.read()

        sys.exit()

    if sys.argv[1]=="testcase":
        folder = sys.argv[2]
        qfile = sys.argv[3]
        keyword = sys.argv[4]
        f = open(folder+"\\" + qfile + ".txt", encoding="utf-8")
        filecontent = f.read()
        
        print(len(filecontent))

        count = 0
        curIndex = 0
        sindex = 0
        allqueries = []
        while 1:
            curIndex = filecontent.find("\n",curIndex)
            if curIndex==-1:
                break
            count += 1
            if count%5 ==0:
                allqueries.append(filecontent[sindex:curIndex])
                sindex = curIndex+1
            curIndex += 1

        for query in allqueries:
            if query.find(keyword) != -1:
                with open("q.txt","w") as f:
                    f.write(query)
                break

        os.system("%s %s q.txt output.xml" %(AUTOTESTER_PATH, folder+"/source.txt") )
        
    elif sys.argv[1]=="help":
        printHelp()
    else:
        printHelp()
except Exception as e:

    print("Some unexpected error: ", e)
    print("Check your syntax : ")
    printHelp()



    

        