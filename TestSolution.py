import re
import sys

solutionLines = open("solution.log", "r").readlines()
dataLines = open(sys.argv[1],"r").readlines()

dataSpace = []

for dataLine in dataLines:
   data = []
   for character in dataLine:
      if character in ["1","0"]:
         data.append(int(character))
   dataSpace.append(data)

def UpdateCurrentSolution(current, addition):
   index = 0;
   while index < len(current):
      current[index] = current[index] + addition[index]
      index = index + 1

def CheckFinalSolution(current):
   for n in current:
      if n != 1:
         return False
   return True

def CheckSolutionSpace(solutionLine):
   solutionGood = False
   currentSolution = [0 for x in range(len(dataSpace[0]))]
   matchData = re.findall("\w+:(\d+) ", solutionLine)
   for match in matchData:
      UpdateCurrentSolution(currentSolution, dataSpace[int(match)])
      print match, ":", dataSpace[int(match)]

   solutionGood = CheckFinalSolution(currentSolution)
   if solutionGood == False:
      print "Solution did not cover all solution space"
      print currentSolution

   return solutionGood

for solutionLine in solutionLines:
   result = CheckSolutionSpace(solutionLine)
   if result:
      print "Solution", solutionLine, "good"
   else:
      print "Solution", solutionLine, "invalid"
