# =========================================================================
# Compiler output processing
#
# Write a python function which takes model compiler output as an input and outputs error statistics.
#
# First argument is a list of strings of the following format:
#
#  `filename:line error (code)'
#
#  For example:
#
#  "a/b/main.c:12 Invalid variable reference (123)"
#
# Second argument is one of the following strings:
#   * "first": Function should return a list of strings indicating the first error in each file which contains error. Note, that input lines might not be sorted.
#   * "total": Function should return a list of strings indicating the total number of errors in each file.
#   * "code": Function shodul return a list of all filenames and lines with the given error code (supplied via named argument CODE).
#   * Error message should be printed in all other cases.
#
# For example:
# % input = ["a/b.c:12 Invalid jump (12)", "a/d.c:98 Multiple gotos (45)", "a/b.c:3 Undefined variable (05)"]
# % out1 = ProcessOutput(input, "first")
# % out1 == ["a/b.c Undefined variable (05)", "a/d.c Multiple gotos (45)"]
# % out2 = ProcessOutput(input, "total")
# % out2 == ["a/b.c:2", "a/d.c:1"]
# % out3 = ProcessOutput(input, "code", CODE = 12)
# % out3 == "a/b.c:12"
# =========================================================================

import re

InputData1 = ["a/b.c:12 Invalid jump (12)",
              "a/d.c:98 Multiple gotos (45)",
              "a/b.c:3 Undefined variable (05)"]

InputData2 = ["a/b.c:12 Invalid jump (12)",
         "a/d.c:98 Multiple gotos (45)",
         "a/b.c:3 Undefined variable (05)",
         "!QAZXSW@#EDC.*&^:1234567890 Invalid long jump (1221)",
         "asdrfs|jh\vbn/*.%$#:987 Multiple black gotos (5)",
         "a/b\q.c:333 Undefined first variable (05)",
         "qwertyiop.c:13332 Invalid jump (1332)",
         "a/duc.c:9898 Multiple fotos (0005)",
         "alfa/b.c:000 Undefined 3 variable (00)",
         "a/b.c:12 2345612 (122)",
         "a/d.c:98 Multiple gotos (45)",
         "a/b.c:3 Undefined variable (05)",
         "a/b.c:22 Invalid jump (12)",
         "a/d.c:98 Multiple gotos (45)",
         "a/b.c:3 Undefined variable (05)",
         "a/b.c:12 Invalid jump (12)",
         "a/d.c:98 Multiple gotos (45)",
         "a/b.c:3 Undefined variable (05)",
         "a/b.c:12 Invalid jump (12)",
         "a/d.c:98 Multiple gotos (45)",
         "a/b.c:3 Undefined variable (05)"]

def AnalyzeString(String):
    '''parses the String and returns a list of the form
[ file name , line number , error text , error code ]
   in case of an error generate Exception'''
    if type(String) != str:
        raise Exception('Wrong input string (1)!!!')
    result=re.match('(.+):(\d+) (.+\((\d+)\))',String)
    if result == None:
        raise Exception('Wrong input string (2)!!!')
    return [ result.group(1),result.group(2),result.group(3),result.group(4) ]

def ProcessOutput(inp,md,CODE=None):
    ''' analyzes the list of text values according
to the task described above '''
    res=[]
    D1 = {}
    params=None
    if type(inp) != list:
        raise Exception('Wrong first operand (inp)!!!')
    if md=='first': # should return a list of strings indicating the first error in each file which contains error
        for elem in inp:
            params=AnalyzeString(elem)
            if params[0] in D1:
                if D1[params[0]][0] > int(params[1]):
                    D1[params[0]]=[int(params[1]),params[2]]
            else:
                D1[params[0]]=[int(params[1]),params[2]]
        for elem in D1:
            res.append(elem+' '+D1[elem][1])
        return res;
    elif md=='total': # should return a list of strings indicating the total number of errors in each file.
        for elem in inp:
            params=AnalyzeString(elem)
            if params[0] in D1:
                if int(params[1]) not in D1[params[0]][1:]:
                    D1[params[0]][0]+=1
                    D1[params[0]].append(int(params[1]))
            else:
                D1[params[0]]=[1,int(params[1])]
        for elem in D1:
            res.append(elem+':'+str(D1[elem][0]))
        return res;
    elif md=='code': # Function shodul return a list of all filenames and lines with the given error code
        if CODE == None:
            raise Exception('CODE operand not specified!!!')
        for elem in inp:
            params=AnalyzeString(elem)
            if int(params[3]) == CODE:
                if params[0] in D1:
                    if int(params[1]) not in D1[params[0]]:
                        D1[params[0]].append(int(params[1]))
                else:
                    D1[params[0]]=[int(params[1])]
        for elem in D1:
            for elem1 in D1[elem]:
                res.append(elem+':'+str(elem1))
        return res;
    else:
        raise Exception('Wrong second operand (md)!!!')

if __name__ == '__main__':
    def test1(inp,md,CODE=None):
        '''test function accepts all the same
    parameters as ProcessOutput '''
        OutputData=[]
        try:
            print('------------------------------------------')
            print('test1','md =',md,'CODE =',CODE)
            print('InputData =',inp)
            OutputData=ProcessOutput(inp,md,CODE)
            print('OutputData = ',OutputData)
        except Exception as e:
            print('ERROR:',e)

    test1(InputData1,'first')
    test1(InputData1,'total')
    test1(InputData1,'code',CODE=12)
    test1(InputData1,'code')
    test1(InputData2,'first')
    test1(InputData2,'total')
    test1(InputData2,'code',CODE=12)


