#-*- coding: UTF-8 -*-
import os
import sqlite3

StoryMapHero=1
StoryMapNPC=2
StoryMapObject=3
StorySceneAnimation=4#过场动画
StoryMapArticle=5#只可以拾取一次的物品
StoryMapExit=6
NPCSideStoryNotOn=7#支线剧情未触发
NPCSideStoryOnNotOK=8#支线剧情被触发但是未完成
NPCSideStoryOnAndOK=9#支线剧情完成但是未关闭
NPCSideStoryOKOff=10#支线剧情完成并且关闭
StoryMapCash=11#在map中获得若干金钱,上限是65536

AniStart=1#动画开始,无参数
#以下的参数前两位均为xy
AniViewMoveTo=2#view快速移动到某处,参数为中心点地图坐标如10#20#,如果是-1#-1#则设置为当前主角位置
AniRoleSetPath=3#为某角色设定path,假定该角色为npc或者enemy或者friend类.参数亏如2#3#4##3#第一个点是role所在位置
AniRoleMove=4#普通移动,参数为role坐标,若为-1,-1则令targetRole开始移动
AniRoleFastMove=5#攻击时的移动,更快,参数同上
AniRoleSweat=6#流汗
AniRoleAttack=7#role开始攻击动画,参数为坐标
AniRoleRecover=8#role开始恢复,参数为坐标#原始体力#恢复体力#满体力
AniRoleHurt=9#role开始受伤害,参数为坐标#原始体力#扣减体力#满体力
AniRoleDie=10#role死亡,参数为坐标
AniRoleDodge=11#role闪避,参数为坐标

AniViewStartGetGoods=12#参数是要显示的所有文字,无#
AniViewStartDialog=13#开始对话,参数为dialog参数
AniViewStartDelay=14#延时,参数为毫秒无#
AniShowBattleStart=15#显示"战斗开始"字样
AniShowRoundX=16#显示"第X回合"字样,参数为X(无#)
AniShowBattleFail=17#显示"战斗失败"字样
AniShowBattleWin=18#显示"胜利"字样
AniShowSelfAction=19#我军行动
AniShowEnemyAction=20#敌军行动
AniShowFriendAction=21#友军行动
AniStartBattle=23#开始战斗,参数是battleID无#
AniSwitchScene=24#切换场景,参数是storyID#mapID#viewX#viewY#
AniEnd=25#动画结束,无参数
AniCursorMoveToViewPoint=26#参数是点坐标200#320#
AniFocusRoleBeInactive=27#无参数,令focusRole停止活动
AniRoleDisappear=28#在story场景中消失,若为-1#-1#则是野怪坐标
AniStartTrade=29#开始贸易,参数是articleID#articleID#articleID#
AniRoleLevelUp=30#参数是2#2#4#3#,每两个进行一个分割
AniRoleChangeJob=31#参数是x#y#
AniRoleAttention=32#参数是x#y#
AniRoleDisable=33#参数是x#y#
AniRolePoison=34#参数是x#y#
AniRoleAttackDown=35
AniRoleAccurcyDown=36
AniRoleCritDown=37
AniRoleDefenceDown=38
AniRoleDodgeDown=39
AniRoleSpeedDown=40
AniRoleRotate=41
AniRoleCure=42
AniRoleUseMagic=43
AniAddRole=44#参数是新加入的roleID,无#
AniSetStoryIDTo=45#设置storyID,无#
AniRoleFlash=46#闪烁,注意,它是单参数的,就是说,它后面必须是1#2#不能是1#2#3#4#
AniGetCash=48#无#
AniGetArticle=49#无#
AniQuitToMainMenu=51#无参数
AniSetSideStoryIDTo=52#设置sideStoryID(关闭sideStory置0即可),默认清零右二和右三字节的标志位,参数无#
AniSetSideStoryOn=53#无参数,打开支线剧情(右二字节置1)
AniSetSideStoryOK=54#无参数,支线剧情完成(右三字节置1)
AniSetSideStoryOff=55#无参数,关闭支线剧情(右二字节置0)
AniMultiTalkFlag=222#这个标志位标志了NPC的动画分为多段,无参数.如果最后一个元素是它,向前搜索到上一个AniEnd,将之后的script全部砍掉

def idToName(id):
	if id==1:
		return u"夏小风"
	elif (id==2 or id==3002):
		return u"冯小鱼"
	elif id==3:
		return u"邵克克"
	elif id==4:
		return u"陈小中"
	elif id==5:
		return u"蔡大宝"
	elif id==6:
		return u"李小莹"
	elif id==7:
		return u"武豆浆"
	elif id==8:
		return u"烧饼大叔"
	elif id==9:
		return u"郭小新"
	elif id==10:
		return u"唐小糖"
	elif id==11:
		return u"李小宁"
	elif id==12:
		return u"闫小龙"
	elif id==13:
		return u"保安大叔"
	elif id==14:
		return u"二大妈"
	elif id==15:
		return u"于阿姨"
	elif id==16:
		return u"人造人16号"
	elif id==17:
		return u"人造人17号"
	elif id==18:
		return u"人造人18号"
	elif id==19:
		return u"人造人19号"
	elif id==20:
		return u"人造人20号"
	elif id==101:
		return u"机械馆大爷"
	elif id==102:
		return u"神秘商人"
	elif id==103:
		return u"詹小妮"
	elif id==151:
		return u"喵星人"
	elif id==501 or id==2001:
		return u"僵尸"
	elif id==502:
		return u"邪恶士兵"
	elif id==2002:
		return u"邪恶士兵"
	elif id==503:
		return u"幽灵士兵"
	elif id==2003:
		return u"幽灵士兵"
	elif id==504 or id==2004:
		return u"僵尸士兵"
	elif id==505 or id==2005:
		return u"杨士豪"
	elif id==506 or id==2006:
		return u"铁面将军"
	elif id==507 or id==2007:
		return u"幽灵士兵"
	elif id==508 or id==2008:
		return u"僵尸"
	elif id==512 or id==2012:
		return u"死神士兵"
	elif id==523 or id==2023:
		return u"王二狗"
	else:
		return u'未知'
		print id
		assert 0

def scriptParamToScript(scriptParam):
	'''
	convert ScriptParam to a string list
	'''
	assert isinstance(scriptParam,unicode)
	#assert scriptParam[-1]==u'$'
	li=scriptParam.split(u'$')
	resultList=[];
	i=0
	while i<len(li)-1:
		op=int(li[i])
		if op==AniStart:
			resultList.append(u'动画开始')
			i+=1
		elif op==AniViewMoveTo:
			x=int(li[i+1].split(u'#')[0])
			y=int(li[i+1].split(u'#')[1])
			resultList.append(u'视角移动到%d,%d' %(x,y))
			i+=2
		elif op==AniRoleSetPath:
			pathList=li[i+1].split(u'#')
			tempstr=u''
			for j in range(0,len(pathList)-1,2):#last element is empty string
				tempStr+=u'(%s,%s)'%(pathList[j],pathList[j+1])
			resultList.append(u'为(%s,%s)处的角色设定路径:%s'%(pathList[0],pathList[1],tempStr))
			i+=2
		elif op==AniRoleMove:
		#普通移动,参数为一个或多个role坐标,若为-1,-1则令targetRole开始移动
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				if int(roleList[j])==-1:
					resultList.append(u'targetRole开始移动')
				else:
					resultList.append(u'(%s,%s)角色移动'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniRoleFastMove:
		#快速移动,参数为一个或多个role坐标,若为-1,-1则令targetRole开始快速移动
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				if int(roleList[j])==-1:
					resultList.append(u'targetRole开始快速移动')
				else:
					resultList.append(u'(%s,%s)角色快速移动'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniRoleSweat:
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				resultList.append(u'(%s,%s)角色流汗'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniRoleAttack:
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				resultList.append(u'(%s,%s)角色攻击'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniRoleRecover:
			assert 0
			i+=2
		elif op==AniRoleHurt:
			assert 0
			i+=2
		elif op==AniRoleDie:
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				resultList.append(u'(%s,%s)角色死亡'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniRoleDodge:
			assert 0
			i+=2
		elif op==AniViewStartGetGoods:
			resultList.append(li[i+1])
			i+=2
		elif op==AniViewStartDialog:
			dialogList=li[i+1].split(u'#')
			tempStr=idToName(int(dialogList[0]))+u'说:'
			for j in range(1,len(dialogList)):
				tempStr+=u'\n\t'
				tempStr+=dialogList[j]
			resultList.append(tempStr)				
			i+=2
		elif op==AniViewStartDelay:
			resultList.append(u'延时%s毫秒' %li[i+1])
			i+=2
		elif op==AniStartBattle:
			resultList.append(u'开始战斗,id为%s' %li[i+1])
			i+=2
		elif op==AniSwitchScene:
			resultList.append(u'切换场景,参数:%s' %li[i+1])
			i+=2
		elif op==AniEnd:
			resultList.append(u'动画结束')
			i+=1
		elif op==AniCursorMoveToViewPoint:
			resultList.append(u'视角移动到(%s,%s)'%(li[i+1].split(u'#')[0],li[i+1].split(u'#')[1]))
			i+=2;
		elif op==AniFocusRoleBeInactive:
			assert 0
		elif op==AniRoleDisappear:
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				resultList.append(u'(%s,%s)角色消失'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniStartTrade:
			resultList.append(u'开始贸易,物品列表是:%s' %li[i+1])
			i+=2
		elif op==AniRoleAttention:
			roleList=li[i+1].split(u'#')
			for j in range(0,len(roleList)-1,2):
				resultList.append(u'(%s,%s)角色注意力被吸引'%(roleList[j],roleList[j+1]))
			i+=2
		elif op==AniAddRole:
			resultList.append(u'%s加入我方'%idToName(int(li[i+1])))
			i+=2
		elif op==AniSetStoryIDTo:
			resultList.append(u'story id 变为%s' %li[i+1])
			i+=2
		elif op==AniGetCash:
			resultList.append(u'获得%s金钱' %li[i+1])
			i+=2
		elif op==AniGetArticle:
			resultList.append(u'获得物品:%s' %li[i+1])
			i+=2
		elif op==AniSetSideStoryIDTo:
			resultList.append(u'支线 story id 变为%s' %li[i+1])
			i+=2
		elif op==AniSetSideStoryOn:
			resultList.append(u'打开支线剧情')
			i+=1
		elif op==AniSetSideStoryOK:
			resultList.append(u'完成支线剧情')
			i+=1
		elif op==AniSetSideStoryOff:
			resultList.append(u'关闭支线剧情')
			i+=1
		elif op==AniMultiTalkFlag:
			resultList.append(u'占位符')
			i+=1
		else:
			print '不应该出现的操作:%d' %op
			assert 0
			
	return resultList



def storySceneObjectToString(resultTuple):
	'''
返回值是三个元素的list,描述这个物体的字符串,与该物体触发的剧情list,该物体的初始路径字符串

Type=storymaphero1时,param参数为ID@
Type=storymapnpc2时,param参数为
ID@(canbecontacteddirectly 0/1)@scriptparam@pathparam@
Type=mapobject3时,参数为
ID@(canbecontacteddirectly 0/1)@scriptparam@
Type=maparticle5时,参数为
ID@scriptParam@
Type=mapexit6时，参数为出口对应的nextID没有#


	'''
	type=resultTuple[0]#int
	x=resultTuple[1]#int
	y=resultTuple[2]#int
	param=resultTuple[3]#unicode
	#print resultTuple
	li=param.split(u'@')
	resultList=[]
	if type==StoryMapHero:
		resultList.append(u'Hero位于(%d,%d)' %(x,y))
		resultList.append([])
		resultList.append(u'')
	elif type==StoryMapNPC:
		resultList.append(u'NPC%s位于(%d,%d),%s直接触发剧情'%(idToName(int(li[0])),x,y,(li[1] and [u'能'] or [u'不能'])[0]))
		resultList.append(scriptParamToScript(li[2]))
		pathList=li[3].split(u'#')
		pathString=u'初始路径是:'
		for i in range(0,len(pathList)-1,2):
			pathString+=u'(%s,%s)'%(pathList[i],pathList[i+1])
		resultList.append(pathString)
	elif type==StoryMapObject:
		resultList.append(u'id为%s的物品,%s直接触发剧情' %(li[0],(li[1] and [u'能'] or [u'不能'])[0]))
		resultList.append(scriptParamToScript(li[2]))
		resultList.append(u'')
	elif type==StorySceneAnimation:
		resultList.append(u'过场动画')
		resultList.append(scriptParamToScript(param))
		resultList.append(u'')
	elif type==StoryMapArticle:
		resultList.append(u'只可以拾取一次的物品:%s'%param)
		resultList.append([])
		resultList.append(u'')
	elif type==StoryMapExit:
		assert 0
		#在global中才会出现
	elif type==NPCSideStoryNotOn:
		resultList.append(u'未触发支线剧情时的NPC%s位于(%d,%d),%s直接触发剧情'%(idToName(int(li[0])),x,y,(li[1] and [u'能'] or [u'不能'])[0]))
		resultList.append(scriptParamToScript(li[2]))
		pathList=li[3].split(u'#')
		pathString=u'初始路径是:'
		for i in range(0,len(pathList)-1,2):
			pathString+=u'(%s,%s)'%(pathList[i],pathList[i+1])
		resultList.append(pathString)
	elif type==NPCSideStoryOnNotOK:
		resultList.append(u'触发支线剧情未完成时的NPC%s位于(%d,%d),%s直接触发剧情'%(idToName(int(li[0])),x,y,(li[1] and [u'能'] or [u'不能'])[0]))
		resultList.append(scriptParamToScript(li[2]))
		pathList=li[3].split(u'#')
		pathString=u'初始路径是:'
		for i in range(0,len(pathList)-1,2):
			pathString+=u'(%s,%s)'%(pathList[i],pathList[i+1])
		resultList.append(pathString)
	elif type==NPCSideStoryOnAndOK:
		resultList.append(u'支线剧情完成未关闭时的NPC%s位于(%d,%d),%s直接触发剧情'%(idToName(int(li[0])),x,y,(li[1] and [u'能'] or [u'不能'])[0]))
		resultList.append(scriptParamToScript(li[2]))
		pathList=li[3].split(u'#')
		pathString=u'初始路径是:'
		for i in range(0,len(pathList)-1,2):
			pathString+=u'(%s,%s)'%(pathList[i],pathList[i+1])
		resultList.append(pathString)	
	elif type==NPCSideStoryOKOff:
		resultList.append(u'支线剧情完成关闭时的NPC%s位于(%d,%d),%s直接触发剧情'%(idToName(int(li[0])),x,y,(li[1] and [u'能'] or [u'不能'])[0]))
		resultList.append(scriptParamToScript(li[2]))
		pathList=li[3].split(u'#')
		pathString=u'初始路径是:'
		for i in range(0,len(pathList)-1,2):
			pathString+=u'(%s,%s)'%(pathList[i],pathList[i+1])
		resultList.append(pathString)
	elif type==StoryMapCash:
		resultList.append(u'获得%s金钱' %param)
		resultList.append([])
		resultList.append(u'')
	else:
		print '无效的type:%d' %type
		assert 0
	return resultList
	

def storyDatabaseToText(databasePathName):
	if isinstance(databasePathName,str)==False:
		print 'type of param is wrong!'
		os.system("pause")
		return None
	
	if os.path.exists(databasePathName)==False:
		print 'file not exist!'
		os.system("pause")
		return None
		
	fileName=databasePathName.split(u'\\')[-1].split(u'.')[0]+'.txt'
	print fileName
	#story.txt
	
	outputFileName=u'%s\\%s' %(u'\\'.join(databasePathName.split(u'\\')[0:-1]),fileName)
	print outputFileName

	outputFile=open(outputFileName,'w')	
	
	databaseCon=sqlite3.connect(databasePathName)
	cur=databaseCon.cursor()
	cur.execute("select name from sqlite_master where type='table'")
	result=cur.fetchall()
	for row in result:
	#row is a typle
		cur.execute(u'select type,x,y,param from %s' %row[0])
		tableResult=cur.fetchall()
		outputFile.write(row[0])
		outputFile.write(u'\n')
		for tableRow in tableResult:
		#tableRow is a tuple where data is (type,x,y,param)
			
			resultList=storySceneObjectToString(tableRow)
			#print tableRow
			#print resultList
			outputFile.write(resultList[0].encode('utf8'))
			outputFile.write(u'\n')
			if resultList[1]:
				assert isinstance(resultList[1],list)
				for s in resultList[1]:
					u=u'\t'+s
					outputFile.write(u.encode('utf8'))
					outputFile.write(u'\n')
			if resultList[2]:
				u=u'\t'+resultList[2]
				outputFile.write(u.encode('utf8'))
				outputFile.write(u'\n')
		outputFile.write(u'\n')
	
	
	
storyDatabaseToText(r'.\story.dat')
raw_input()
