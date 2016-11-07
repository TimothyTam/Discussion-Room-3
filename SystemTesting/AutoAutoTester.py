import os, subprocess, sys

AUTOTESTER_PATH = "..\Release\AutoTester.exe"
# AUTOTESTER_PATH = "..\Debug\AutoTester.exe"

def printHelp():
    print()
    print("Usage 1: python AutoAutoTester.py")
    print("   => Test all test cases in all the folders")
    print("*Note: it will look for AutoTester.exe in Release folder")
    print("*Example: python AutoAutoTester.py")

    print("Usage 2: python AutoAutoTester.py testFolder")
    print("   => Test a specific folder")
    print("*Example: python AutoAutoTester.py iter3_Test1")

    print()
    print("Usage 3: python AutoAutoTester.py testcase [folder] [query_file] [keyword]")
    print("   => Run the query in [folder]/[query_file].txt that contains the [keyword]")
    print("*Note: It will only run the first query containing that keyword")
    print("*Example: python AutoAutoTester.py testcase iter1_Test1 queries1 \"33 - \"")

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
    
    files = list( filter( lambda x: x.find("q") != -1 ,files) )
    print("_______For SIMPLE source in folder " + dirPath + ":_________________________")

    for f in files:
        if f[0]!='q':
            continue
        print("++++ " + f + " : ")
        outfile = "output__" + dirPath[2:] + "__" + f[:-4] + ".xml"
        outfile = "output.xml"
        output = runCommand( ("%s %s %s " + outfile) %( AUTOTESTER_PATH, dirPath + "\source.txt", dirPath + "\\" + f)).decode("utf-8")
        f2 = open(outfile, encoding="utf-8")
        
        output = f2.read()

        queryResults = output.split("</query>")[:-1]

        queryCount = len(queryResults)
        correctCount = 0
        timeoutCount = 0

        for index in range(0,queryCount):

            if queryResults[index].find("<passed/>") == -1:
                idIndex = queryResults[index].find("<id")
                idBracketIndex = queryResults[index].find(">",idIndex)
                bracketIndex = queryResults[index].find("<",idBracketIndex)

                queryId = queryResults[index][idBracketIndex+1:bracketIndex]

                if queryResults[index].find("<timeout/>") != -1:
                    print("      " + queryId + ": TIMEOUT")
                    if printDetails:
                        print()
                    timeoutCount += 1
                    continue
                
                print("      " + queryId + ": Wrong result, time = " + getAttr("time_taken",queryResults[index]) )
                if printDetails:
                    
                    print("            SPA answer:     " + getAttr("stuans",queryResults[index]))
                    print("            Correct answer: " + getAttr("correct",queryResults[index]))
                    print()
                
            else:
                correctCount += 1
                if not printDetails:
                    print("      " + queryId + ":      " + getAttr("time_taken",queryResults[index]))

        print("---- Result [ " + f + " ]:" + str(correctCount) + "/" + str(queryCount) + " correct, " + 
            str(timeoutCount) + "/" + str(queryCount) + " TIMEOUT")
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

        for currentDir, dirs, afiles in os.walk(".\ManyMoreTests\TestCases2"):
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



    

        