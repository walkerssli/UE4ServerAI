import sys
import re

if __name__ == "__main__":
	try:
		f = open("ConsoleApplication1/BehaviorTree/BehaviorTreeManager.cpp", "r")
		# print(f.read())
		content = f.read()
		p1 = content.find("ExportServerNodes_Start")
		p2 = content.find("ExportServerNodes_End")
		content = content[p1:p2]
		pattern = re.compile(r'\[\"(\w+)\"\]')
		classList = pattern.findall(content)

		f2 = open("../../BnS/Content/AI/ServerOnly/ServerNodeList.txt", "w")
		for i in range(0, len(classList)):
			className = classList[i]
			f2.write(className+"\n")
			f2.write(className+"_C\n")
			s = "Export class name {0} : {1}".format(i, className)
			print(s)
		f2.close()


	finally:
		if f:
			f.close()