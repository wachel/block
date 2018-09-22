#coding:cp936

#游戏添加
addToGame = []

#游戏删除
exceptFromGame = []


def isNeedExceptByList(s,ls):
    for x in ls:
        if s.lower().find(x) != -1:
            return True
    return False

def getSubStr(s,prev,after):
    p0 = s.find(prev)
    if p0 != -1:
        p1 = s.find(after,p0 + len(prev))
        if p1 != -1:
            return s[p0 + len(prev):p1]
    return ''

def isSourceFile(s):
    if s.lower().endswith('.c') or s.lower().endswith('.cpp'):
        return True
    return False

def getVcFiles(fileName):
    f = open(fileName)
    ls = f.readlines()
    f.close()
    rlt = []
    for l in ls:
        s = getSubStr(l,'<ClCompile Include="','"')
        if s != "" and isSourceFile(s):
            rlt.append(s)
    return rlt

def convertSrc(ls):
    rlt = addToGame
    rlt.extend(['    $(SRC_PATH)/' + x.replace('\\','/') + '\\\n' for x in ls if not isNeedExceptByList(x,exceptFromGame)])
    rlt[-1] = rlt[-1][:-2] #去掉最后一行的最后一个字符
    rlt.append('\n')
    return rlt
    
    
f = open('Android.mk')
ls = f.readlines()
f.close()
rlt = []
for l in ls:
    if l.find('$(SRC_PATH)') != -1:
        pass
    else:
        rlt.append(l)
    if l.startswith('LOCAL_SRC_FILES = '):
        rlt.extend(convertSrc(getVcFiles('../../win32/RayTrace.vcxproj')))
        
f = open('Android.mk','wt')
f.writelines(rlt)
f.close()