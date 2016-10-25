import os, subprocess, sys

AUTOTESTER_PATH = "..\Release\AutoTester.exe"

def printHelp():
    print()
    print("Usage 1: python AutoAutoTester.py [options]")
    print("   => Test all test cases in all the folders")
    print("  Options: debug => look for AutoTester.exe in Debug folder, default is Release")
    print("*Example: python AutoAutoTester.py")

    print()
    print("Usage 2: python AutoAutoTester.py testcase [folder] [query_file] [keyword]")
    print("   => Run the query in [folder]/[query_file].txt that contains the [keyword]")
    print("*Note: It will only run the first query containing that keyword")
    print("*Example: python AutoAutoTester.py testcase iter1_Test1 queries1 \"33 - \"")

def runCommand(command):
    proc = subprocess.Popen(command,shell=True,stdout=subprocess.PIPE)

    (output,err) = proc.communicate()
    return output

try:
    if len(sys.argv)>1:
        if (sys.argv[1]=="Debug" or sys.argv[1]=="debug"):
            AUTOTESTER_PATH = "..\Debug\AutoTester.exe" 

    if not os.path.isfile(AUTOTESTER_PATH):
        print("AutoTester.exe does not exist in " + AUTOTESTER_PATH)


    if len(sys.argv)==1:
        for currentDir, dirs, afiles in os.walk("."):
            for dir in dirs:
                dirPath = os.path.join(currentDir,dir)
                
                files = os.listdir(dirPath)
                
                files = list( filter( lambda x: x.find("queries") != -1 ,files) )
        

                print("_______For SIMPLE source in folder " + dirPath + ":_________________________")

                for f in files:
                    print("++++ " + f + " : ")
                    output = runCommand("%s %s %s output.xml" %( AUTOTESTER_PATH, dirPath + "\source.txt", dirPath + "\\" + f)).decode("utf-8")
                    queryResults = output.split("-QueryDivision-")
                    queryCount = len(queryResults) - 1
                    correctCount = 0

                    for index in range(1,queryCount+1):
                        if queryResults[index].find("Missing:") != -1:
                            evaluatingQueryIndex = queryResults[index-1].find("Evaluating query")
                            newLineIndex = queryResults[index-1].find("\n",evaluatingQueryIndex)
                            queryId = queryResults[index-1][evaluatingQueryIndex+17:newLineIndex]
                            print("Wrong result at query id " + queryId)
                            yourAnswerIndex = queryResults[index].find("Your answer:")
                            missingIndex = queryResults[index].find("Missing:")
                            print(queryResults[index][yourAnswerIndex:missingIndex])
                        else:
                            correctCount += 1

                    print("++++ Result [ " + f + " ]:" + str(correctCount) + "/" + str(queryCount) + " correct")
                    print()
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



    

        